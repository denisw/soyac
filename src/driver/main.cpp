/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include <algorithm>
#include <cerrno>
#include <cstring>
#include <iostream>
#include <string>
#include <popt.h>

#include <analysis/ModulesRequiredException.hpp>
#include "config.hpp"
#include "FileProcessor.hpp"
#include "link.hpp"
#include "ProblemReport.hpp"

#define SOYAC_VERSION  "0.1"

using namespace soyac;
using namespace soyac::driver;
using soyac::analysis::ModulesRequiredException;

static std::list<std::string> sObjectFiles;


/**
 * Parses the passed command-line argument vector and stores all option
 * values in the global variables declared in the soyac::driver::options
 * namespace.  A list of of the input files specified in the command-line
 * arguments is then returned.
 *
 * @param argc  The argument count.
 * @param argv  The argument vector.
 * @return      The input files.
 */
static std::list<std::string>
parse_command_line(int argc, const char** argv)
{
    struct poptOption options[] = {
        {"emit-llvm", '\0',
         POPT_ARG_NONE | POPT_ARGFLAG_ONEDASH,
         &config::emitLLVM, 0,
         "Compile to LLVM assembly only, do not assemble or link"},

        {NULL, 'c',
         POPT_ARG_NONE,
         &config::compileOnly, 0,
         "Compile and assemble only, do not link"},

        {NULL, 'I',
         POPT_ARG_ARGV,
         &config::interfacePaths, 0,
         "Add <directory> to the interface file search path",
         "<directory>"},

        {NULL, 'o',
         POPT_ARG_STRING,
         &config::outputPath, 0,
         "Output the linked binary to <file>",
         "<file>"},

        {NULL, 's',
         POPT_ARG_ARGV,
         &config::sourcePaths, 0,
         "Add <directory> to the source file search path",
         "<directory>"},

        {NULL, 'S',
         POPT_ARG_NONE,
         &config::emitAssembly, 0,
         "Compile to native assembly only, do not assemble or link"},

        POPT_AUTOHELP
        {0, 0, 0, 0, 0}
    };

    poptContext context = poptGetContext(0, argc, argv, options, 0);
    int rc;

    /*
     * Parse the options specified on the command line, and exit with an
     * error message if any errors are encountered.
     */
    while ((rc = poptGetNextOpt(context)) != -1)
    {
        if (rc < 0)
        {
            std::cerr << config::programName << ": " <<
                         poptBadOption(context, 0) << ": " <<
                         poptStrerror(rc) << std::endl;
            std::exit(1);
        }
    }

    /*
     * Read all input files from the command line and return them.
     */

    std::list<std::string> inputFiles;
    const char* f;

    while ((f = poptGetArg(context)) != 0)
        inputFiles.push_back(std::string(f));

    poptFreeContext(context);
    return inputFiles;
}


/**
 * Searches for a source or interface file which provides the specified module
 * and returns its path. If no module with the passed name can be found, an
 * empty string is returned.
 *
 * @param moduleName  The name of the module to find.
 */
std::string
find_module(const std::string& moduleName)
{
    /*
     * Translate the module name to a file path we can search for (without
     * the .soya or .soyi extension). We do so by replacing all occurrences
     * of "::" with a path separator. So, for instance, "foo::bar::baz" becomes
     * "foo/bar/baz". (Actually, it is 'foo//bar//baz' in the current code,
     * but as empty path components don't alter the path's meaning, this is
     * no problem.)
     */
    std::string modulePath = moduleName;
    std::replace(modulePath.begin(), modulePath.end(), ':', '/');


    /*
     * First, search for the module in the current working directory. We
     * look for a source file first and, if we don't find any, for an
     * interface file.
     */

    llvm::sys::Path filePath;
    filePath.set(modulePath + ".soya");

    if (filePath.exists())
        return filePath.toString();

    filePath.set(modulePath + ".soyi");

    if (filePath.exists())
        return filePath.toString();

    /*
     * If we couldn't find the module in the current working directory, search
     * for it in the source file search path (if specified).
     */
    if (config::sourcePaths != NULL)
    {
        for (const char** p = config::sourcePaths; *p; p++)
        {
            filePath.set(std::string(*p) + '/' + modulePath + ".soya");

            if (filePath.exists())
                return filePath.toString();

            filePath.set(std::string(*p) + '/' + modulePath + ".soyi");

            if (filePath.exists())
                return filePath.toString();
        }
    }

    /*
     * If the module couldn't be found in the source file search path either,
     * we look for it in the interface file search path (if specified).
     */
    if (config::interfacePaths != NULL)
    {
        for (const char** p = config::interfacePaths; *p; p++)
        {
            filePath.set(std::string(*p) + '/' + modulePath + ".soyi");

            if (filePath.exists())
                return filePath.toString();
        }
    }

    /*
     * Last but not least, we look in the default interface paths.
     */
      for (const char** p = config::defaultInterfacePaths; *p; p++)
      {
          filePath.set(std::string(*p) + '/' + modulePath + ".soyi");

          if (filePath.exists())
              return filePath.toString();
      }

    /*
     * If the module cannot be found anywhere, return an empty string.
     */
    return std::string("");
}


/**
 * Processes the passed list of files. If the list contains source files
 * which depend on modules not provided by any file in the list, these
 * modules are searched for using find_module() and, if found, are processed
 * too.
 *
 * @param files  The files to process.
 */
static void
process_files(std::list<std::string>& files)
{
    std::list<std::string> required;

    /*
     * Try to process the passed files.
     */
    for (std::list<std::string>::iterator it = files.begin();
         it != files.end();)
    {
        try
        {
            FileProcessor proc(*it);
            std::string outputFile;

            try
            {
                outputFile = proc.process();
            }
            catch (const std::ifstream::failure& exc)
            {
                std::cerr << config::programName << ": "
                             "cannot read `" << *it << "': " <<
                              std::strerror(errno) << std::endl;

                std::exit(1);
            }

            /*
             * If the output file is an object file, add it to the object
             * file list for linking.
             */
            if (config::linkingRequested() &&
                outputFile != "" &&
                outputFile.substr(outputFile.length() - 2) == ".o")
            {
                sObjectFiles.push_back(outputFile);
            }

            /*
             * If a file has been processed, we can remove it from the
             * file list. This leaves us with a list with only those
             * source files which depend on the required modules.
             */
            it = files.erase(it);
        }
        catch (const ModulesRequiredException& e)
        {
            /*
             * If further modules must be loaded to compile a file,
             * we add those modules to a list.
             */
            for (ModulesRequiredException::const_modules_iterator m_it =
                   e.modules_begin();
                 m_it != e.modules_end();
                 m_it++)
            {
                required.push_back(*m_it);
            }

            it++;
        }
    }

    /*
     * In the list of required modules, replace all module names with
     * paths to corresponding source files using find_module().
     */
    for (std::list<std::string>::iterator it = required.begin();
         it != required.end();)
    {
        /*
         * It could be that the required module was already provided by one
         * of the files passed to this function. If this is the case, remove
         * it from the list of required modules.
         */
        if (soyac::ast::Module::get(soyac::ast::Name(*it)) != NULL)
        {
            it = required.erase(it);
        }
        else
        {
            std::string path = find_module(*it);

            if (path == "")
            {
                std::cerr <<
                  config::programName << ": cannot find required module '" <<
                  *it << "'" << std::endl;
                std::exit(1);
            }
            else
            {
                *it = path;
                it++;
            }
        }
    }

    /*
     * Process the required files. After that, we can go on to process
     * the files which required them.
     */

    if (!required.empty())
        process_files(required);

    if (!files.empty())
        process_files(files);
}


int
main(int argc, const char** argv)
{
    config::programName = argv[0];

    std::list<std::string> inputFiles = parse_command_line(argc, argv);

    if (inputFiles.size() == 0)
    {
        std::cerr << config::programName << ": no input files" << std::endl;
        return 1;
    }

    process_files(inputFiles);

    if (ProblemReport::show() == true)
        return 1;

    if (config::linkingRequested())
        linkFiles(sObjectFiles);

    return 0;
}
