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
#include <filesystem>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

#include <boost/program_options.hpp>
#include <llvm/Support/TargetSelect.h>

#include "FileProcessor.h"
#include "ProblemReport.h"
#include "config.h"
#include "link.h"
#include <analysis/ModulesRequiredException.h>

#define SOYAC_VERSION "0.1"

namespace po = boost::program_options;

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
static std::vector<std::string> parse_command_line(int argc, const char** argv)
{
    po::options_description desc(
        "Compiler for the Soya toy programming language");

    desc.add_options()("help", "Show this help message and exit")(
        "emit-llvm", "Compile to LLVM assembly only, do not assemble or link")(
        "compile-only,c", "Compile and assemble only, do not link")(
        "include-path,I", po::value<std::vector<std::string>>(),
        "Add <directory> to the interface file search path")("library-path,L",
        po::value<std::vector<std::string>>(),
        "Add <directory> to the library path")("o",
        po::value<std::string>()->default_value("a.out"),
        "Output the linked binary to <file>")("source-path,s",
        po::value<std::vector<std::string>>(),
        "Add <directory> to the source file search path")(
        "S", "Compile to native assembly only, do not assemble or link")(
        "input-file", po::value<std::vector<std::string>>(), "Input files");

    po::positional_options_description p;
    p.add("input-file", -1);

    po::variables_map vars;
    po::command_line_parser parser(argc, argv);
    po::store(parser.options(desc).positional(p).run(), vars);
    po::notify(vars);

    if (vars.count("help")) {
        std::cout << desc << "\n";
        return {};
    }

    if (vars.count("emit-llvm")) {
        config::emitLLVM = true;
    }

    if (vars.count("compile-only")) {
        config::compileOnly = true;
    }

    if (vars.count("include-path")) {
        config::interfacePaths
            = vars["include-path"].as<std::vector<std::string>>();
    }

    if (vars.count("library-path")) {
        config::libraryPaths
            = vars["library-path"].as<std::vector<std::string>>();
    }

    if (vars.count("o")) {
        config::outputPath = vars["o"].as<std::string>();
    }

    if (vars.count("source-path")) {
        config::sourcePaths
            = vars["source-path"].as<std::vector<std::string>>();
    }

    if (vars.count("S")) {
        config::emitAssembly = true;
    }

    if (vars.count("input-file")) {
        return vars["input-file"].as<std::vector<std::string>>();
    } else {
        return {};
    }
}

/**
 * Searches for a source or interface file which provides the specified module
 * and returns its path. If no module with the passed name can be found, an
 * empty string is returned.
 *
 * @param moduleName  The name of the module to find.
 */
std::string find_module(const std::string& moduleName)
{
    /*
     * Translate the module name to a file path we can search for (without
     * the .soya or .soyi extension). We do so by replacing all occurrences
     * of "::" with a path separator. So, for instance, "foo::bar::baz" becomes
     * "foo/bar/baz".
     */
    std::string modulePath
        = std::regex_replace(moduleName, std::regex("::"), "/");

    /*
     * First, search for the module in the current working directory. We
     * look for a source file first and, if we don't find any, for an
     * interface file.
     */

    std::filesystem::path filePath(modulePath);

    filePath.replace_extension(".soya");
    if (std::filesystem::exists(filePath)) {
        return filePath.string();
    }

    filePath.replace_extension(".soyi");
    if (std::filesystem::exists(filePath)) {
        return filePath.string();
    }

    /*
     * If we couldn't find the module in the current working directory, search
     * for it in the source file search path (if specified).
     */
    for (std::string p : config::sourcePaths) {
        filePath = (std::filesystem::path(p)) / modulePath;

        filePath.replace_extension(".soya");
        if (std::filesystem::exists(filePath)) {
            return filePath.string();
        }

        filePath.replace_extension(".soyi");
        if (std::filesystem::exists(filePath)) {
            return filePath.string();
        }
    }

    /*
     * If the module couldn't be found in the source file search path either,
     * we look for it in the interface file search path (if specified).
     */
    for (std::string p : config::interfacePaths) {
        filePath = (std::filesystem::path(p)) / modulePath;

        filePath.replace_extension(".soyi");
        if (std::filesystem::exists(filePath)) {
            return filePath.string();
        }
    }

    /*
     * Last but not least, we look in the default interface paths.
     */
    for (std::string p : config::defaultInterfacePaths) {
        filePath = (std::filesystem::path(p)) / modulePath;

        filePath.replace_extension(".soyi");
        if (std::filesystem::exists(filePath)) {
            return filePath.string();
        }
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
static void process_files(std::vector<std::string>& files)
{
    std::vector<std::string> required;

    /*
     * Try to process the passed files.
     */
    for (std::vector<std::string>::iterator it = files.begin();
        it != files.end();) {
        try {
            FileProcessor proc(*it);
            std::string outputFile;

            try {
                outputFile = proc.process();
            } catch (const std::ifstream::failure& exc) {
                std::cerr << config::programName
                          << ": "
                             "cannot read `"
                          << *it << "': " << std::strerror(errno) << std::endl;

                std::exit(1);
            }

            /*
             * If the output file is an object file, add it to the object
             * file list for linking.
             */
            if (config::linkingRequested() && outputFile != ""
                && outputFile.substr(outputFile.length() - 2) == ".o") {
                sObjectFiles.push_back(outputFile);
            }

            /*
             * If a file has been processed, we can remove it from the
             * file list. This leaves us with a list with only those
             * source files which depend on the required modules.
             */
            it = files.erase(it);
        } catch (const ModulesRequiredException& e) {
            /*
             * If further modules must be loaded to compile a file,
             * we add those modules to a list.
             */
            for (ModulesRequiredException::const_modules_iterator m_it
                = e.modules_begin();
                m_it != e.modules_end(); m_it++) {
                required.push_back(*m_it);
            }

            it++;
        }
    }

    /*
     * In the list of required modules, replace all module names with
     * paths to corresponding source files using find_module().
     */
    for (auto it = required.begin(); it != required.end();) {
        /*
         * It could be that the required module was already provided by one
         * of the files passed to this function. If this is the case, remove
         * it from the list of required modules.
         */
        if (soyac::ast::Module::get(soyac::ast::Name(*it)) != nullptr) {
            it = required.erase(it);
        } else {
            std::string path = find_module(*it);

            if (path == "") {
                std::cerr << config::programName
                          << ": cannot find required module '" << *it << "'"
                          << std::endl;
                std::exit(1);
            } else {
                *it = path;
                it++;
            }
        }
    }

    /*
     * Process the required files. After that, we can go on to process
     * the files which required them.
     */

    if (!required.empty()) {
        process_files(required);
    }

    if (!files.empty()) {
        process_files(files);
    }
}

int main(int argc, const char** argv)
{
    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmPrinter();

    config::programName = argv[0];

    std::vector<std::string> inputFiles = parse_command_line(argc, argv);

    if (inputFiles.size() == 0) {
        std::cerr << config::programName << ": no input files" << std::endl;
        return 1;
    }

    process_files(inputFiles);

    if (ProblemReport::show() == true) {
        return 1;
    }

    if (config::linkingRequested()) {
        linkFiles(sObjectFiles);
    }

    return 0;
}
