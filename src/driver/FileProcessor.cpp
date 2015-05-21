/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */


#include <cstdlib>
#include <fstream>
#include <iostream>
#include <vector>
#include <llvm/System/Program.h>

#include <parser/ParserDriver.hpp>
#include <analysis/BasicAnalyzer.hpp>
#include <analysis/ControlFlowAnalyzer.hpp>
#include <codegen/CodeGenerator.hpp>

#include "FileProcessor.hpp"
#include "config.hpp"
#include "ProblemReport.hpp"

using llvm::sys::Program;

namespace soyac {
namespace driver
{


FileProcessor::FileProcessor(const std::string& inputFile)
    : mFilePath(inputFile)
{
    /*
     * Create a temporary directory where all created temporary
     * files will be put in.
     */
    mTempDir = Path::GetTemporaryDirectory();
}


FileProcessor::~FileProcessor()
{
    mTempDir.eraseFromDisk(true);
}


std::string
FileProcessor::process()
  throw (std::ifstream::failure,
         soyac::analysis::ModulesRequiredException)
{
    Path p = mFilePath;



    /*
     * If the file to process is a source or interface file, we parse and
     * validate it. In the case of a source file, we also translate the file
     * to LLVM assembly.
     */
    if (p.getSuffix() == "soya" || p.getSuffix() == "soyi")
    {
        /*
         * Parse the file.
         */

        PassResult* result = NULL;
        ast::Module* m = parser::ParserDriver(p.toString()).parse(result);

        if (result != NULL)
            ProblemReport::addPassResult(result);

        /*
         * If parse() returned a module, continue with analyzing the
         * parsed source module.
         */
        if (m != NULL)
        {
            /*
             * Analyze the module. If errors were found in the module, we
             * cannot proceed further as the abstract syntax tree is invalid.
             */
            if (!analyze(m))
                return std::string("");

            /*
             * If the input file is a source file, generate LLVM assembly for
             * it. We set 'p' to the generated LLVM assembly file's path so
             * that the file will be further processed if this is needed.
             */
            if (p.getSuffix() == "soya")
                p = generateLLVM(m);
            /*
             * If we are processing an interface file, there won't be any
             * compilation steps taken, so we can return now.
             */
            else
                return p.toString();
        }
    }

    /*
     * If the target output format is LLVM assembly, we're done.
     */
    if (config::emitLLVM)
        return p.toString();

    /*
     * If the (new) file to process is an LLVM assembly file, we translate it
     * to a native assembler file. Again, we set 'p' to the new file for the
     * case that further processing is needed.
     */
    if (p.getSuffix() == "ll")
    {
        p = generateBitcode(p);
        if (p == Path())
            return std::string("");

        p = generateAssembly(p);
        if (p == Path())
            return std::string("");
    }

    /*
     * If the target output format is native assembly, we're done.
     */
    if (config::emitAssembly)
        return p.toString();

    /*
     * If the (new) file to process is a native assembly file, we assemble
     * it to an binary object file.
     */
    if (p.getSuffix() == "s")
    {
        p = generateBinary(p);
        if (p == Path())
            return std::string("");
    }

    /*
     * We're definitely done now.
     */
    return p.toString();
}


bool
FileProcessor::analyze(ast::Module* m)
  throw (soyac::analysis::ModulesRequiredException)
{
    PassResult* result;

    /*
     * Basic Analysis
     */
    result = analysis::BasicAnalyzer().analyze(m);
    if (result != NULL)
    {
        ProblemReport::addPassResult(result);
        if (result->foundErrors())
            return false;
    }

    /*
     * Control Flow Analysis
     */
    result = analysis::ControlFlowAnalyzer().analyze(m);
    if (result != NULL)
    {
        ProblemReport::addPassResult(result);
        if (result->foundErrors())
            return false;
    }

    return true;
}


Path
FileProcessor::generateLLVM(ast::Module* m)
{
    Path outpath;

    if (config::emitLLVM)
    {
        outpath = mFilePath;
        outpath.eraseSuffix();
    }
    else
    {
        outpath = mTempDir;
        outpath.appendComponent(mFilePath.getBasename());
    }

    outpath.appendSuffix("ll");

    llvm::Module* lm = (llvm::Module*) codegen::CodeGenerator().generateCode(m);

    std::ofstream f;

    f.open(outpath.toString().c_str(), std::ios_base::binary);
    lm->print(f, NULL);

    f.close();
    delete lm;

    return outpath;
}


Path
FileProcessor::generateBitcode(const Path& inputFile)
{
    Path outpath = mTempDir;
    outpath.appendComponent(mFilePath.getBasename());

    outpath.appendSuffix("bc");

    Path llvm_as = Program::FindProgramByName("llvm-as");

    if (!llvm_as.isValid())
    {
        std::cerr << "soyac: could not find llvm-as" << std::endl;
        std::exit(1);
    }

    const char* argv[] = {
        "llvm-as",
        "-f",
        "-o", outpath.c_str(),
        inputFile.c_str(),
        NULL
    };

    int ret = Program::ExecuteAndWait(llvm_as, argv);

    if (ret != 0)
        return Path();
    else
        return outpath;
}

Path
FileProcessor::generateAssembly(const Path& inputFile)
{
    Path outpath;

    if (config::emitAssembly)
    {
        outpath = mFilePath;
        outpath.eraseSuffix();
    }
    else
    {
        outpath = mTempDir;
        outpath.appendComponent(mFilePath.getBasename());
    }

    outpath.appendSuffix("s");

    Path llc = Program::FindProgramByName("llc");

    if (!llc.isValid())
    {
        std::cerr << "soyac: could not find llvm-as" << std::endl;
        std::exit(1);
    }

    const char* argv[] = {
        "llc",
        "-f",
        "-o", outpath.c_str(),
        inputFile.c_str(),
        NULL
    };

    int ret = Program::ExecuteAndWait(llc, argv);

    if (ret != 0)
        return Path();
    else
        return outpath;
}


Path
FileProcessor::generateBinary(const Path& inputFile)
{
    Path outpath = mFilePath;
    outpath.eraseSuffix();

    outpath.appendSuffix("o");

    Path gcc = Program::FindProgramByName("as");

    if (!gcc.isValid())
    {
        std::cerr << "soyac: could not find gcc" << std::endl;
        std::exit(1);
    }

    const char* argv[] = {
        "as",
        "-o", outpath.c_str(),
        inputFile.c_str(),
        NULL
    };

    int ret = Program::ExecuteAndWait(gcc, argv);

    if (ret != 0)
        return Path();
    else
        return outpath;
}


}}
