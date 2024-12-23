/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include <array>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <llvm/Target/TargetMachine.h>

#include <analysis/BasicAnalyzer.hpp>
#include <analysis/ControlFlowAnalyzer.hpp>
#include <codegen/CodeGenerator.hpp>
#include <parser/ParserDriver.hpp>

#include "FileProcessor.hpp"
#include "ProblemReport.hpp"
#include "config.hpp"

using std::filesystem::path;

namespace soyac {
namespace driver {

FileProcessor::FileProcessor(const std::string& inputFile)
    : mFilePath(inputFile)
{
    /*
     * Create a temporary directory where all created temporary
     * files will be put in.
     */
    mTempDir = std::filesystem::temp_directory_path();
}

FileProcessor::~FileProcessor() { }

std::string FileProcessor::process()
{
    std::filesystem::path p(mFilePath);

    /*
     * If the file to process is a source or interface file, we parse and
     * validate it. In the case of a source file, we also translate the file
     * to LLVM assembly.
     */
    if (p.extension().string() == ".soya"
        || p.extension().string() == ".soyi") {
        /*
         * Parse the file.
         */

        PassResult* result = NULL;
        ast::Module* m = parser::ParserDriver(p.string()).parse(result);

        if (result != NULL) {
            ProblemReport::addPassResult(result);
        }

        /*
         * If parse() did not return a module, parsing failed.
         */
        if (!m) {
            return std::string("");
        }

        /*
         * Analyze the module. If errors were found in the module, we
         * cannot proceed further as the abstract syntax tree is invalid.
         */
        if (!analyze(m)) {
            return std::string("");
        }

        /*
         * If the input file is a source file, generate LLVM assembly for
         * it. We set 'p' to the generated LLVM assembly file's path so
         * that the file will be further processed if this is needed.
         */
        if (p.extension().string() == ".soya") {
            return compile(m);
        }
        /*
         * If we are processing an interface file, there won't be any
         * compilation steps taken, so we can return now.
         */
        else {
            return p.string();
        }
    }

    return mFilePath.string();
}

bool FileProcessor::analyze(ast::Module* m)
{
    PassResult* result;

    /*
     * Basic Analysis
     */
    result = analysis::BasicAnalyzer().analyze(m);
    if (result != NULL) {
        ProblemReport::addPassResult(result);
        if (result->foundErrors()) {
            return false;
        }
    }

    /*
     * Control Flow Analysis
     */
    result = analysis::ControlFlowAnalyzer().analyze(m);
    if (result != NULL) {
        ProblemReport::addPassResult(result);
        if (result->foundErrors()) {
            return false;
        }
    }

    return true;
}

path FileProcessor::compile(soyac::ast::Module* m)
{
    if (config::emitLLVM) {
        return generateLLVMAssemblyFile(m);
    } else {
        return generateObjectFile(m);
    }
}

path FileProcessor::generateLLVMAssemblyFile(ast::Module* m)
{
    path outputPath(mFilePath);
    outputPath.replace_extension(".ll");

    codegen::CodeGenerator generator(m);
    std::error_code error;
    generator.toLLVMAssembly(outputPath, error);

    if (error) {
        std::cerr << error.message() << "\n";
        std::exit(1);
    }

    return outputPath;
}

path FileProcessor::generateObjectFile(soyac::ast::Module* m)
{
    path outputPath(mFilePath);
    outputPath.replace_extension(".o");

    codegen::CodeGenerator generator(m);
    std::error_code error;
    generator.toObjectCode(outputPath, error);

    if (error) {
        std::cerr << error.message() << "\n";
        std::exit(1);
    }

    return outputPath;
}

} // namespace driver
} // namespace soyac
