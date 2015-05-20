/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _FILE_PROCESSOR_HPP
#define _FILE_PROCESSOR_HPP

#include <fstream>
#include <iostream>
#include <string>
#include <llvm/Module.h>
#include <llvm/System/Path.h>
#include <analysis/ModulesRequiredException.hpp>
#include <ast/Module.hpp>

using llvm::sys::Path;

namespace soyac {
namespace driver
{

/**
 * Encapsulates the processing of a single input file.
 */
class FileProcessor
{
public:
    /**
     * Creates a FileProcessor.
     *
     * @param inputFile  A path to the file to process.
     */
    FileProcessor(const std::string& inputFile);

    /**
     * FileProcessor destructor.
     */
    ~FileProcessor();

    /**
     * Processes the input file associated with the FileProcessor and returns
     * the name of the generated output file. If no processing is required
     * (e.g., if "-c" is specified as command-line argument and the input file
     * is already an object file), the returned output file is identical to
     * the input file. If processing fails (e.g. due to compilation errors),
     * an empty string is returned instead.
     *
     * If an I/O error occurs while an input source or interface file is
     * read, a std::ifstream::failure exception is thrown.
     *
     * If the input file is a source or interface file, it may be that the
     * module specified by this file depends on other modules which have not
     * been processed yet. In this case, process() throws a
     * soyac::ast::ModulesRequiredException.
     *
     * @throw std::ifstream::failure                If an I/O error occurs.
     * @throw soyac::ast::ModulesRequiredException  If other modules need to
     *                                              be processed first.
     * 
     * @return  The generated output file, or an empty string.
     */
    std::string process()
      throw (std::ifstream::failure,
             soyac::analysis::ModulesRequiredException);

private:
    Path mFilePath;
    Path mTempDir;

    /**
     * Analyzes the passed Module. If there are modules required which are not
     * processed yet, a ModulesRequiredException is thrown. Otherwise, @c true
     * is returned if no errors are found in the module. If errors are found,
     * @c false is returned.
     *
     * @param m  The module to analyze.
     * @return   @c true if no errors were found,
     *           @c false otherwise.
     */
    bool analyze(soyac::ast::Module* m)
      throw (soyac::analysis::ModulesRequiredException);

    /**
     * Generates an LLVM assembly file from the passed module and returns
     * its path. (If the FileProcessor's target output format is LLVM assembly,
     * the file will be located in the input file's directory. Otherwise, it
     * is stored in the FileProcessor's temporary directory.)
     *
     * @param m  The module to generate code for.
     * @return   The resulting LLVM assembly file's path.
     */
    Path generateLLVM(soyac::ast::Module* m);

    /**
     * Generates an LLVM bitcode file from the passed LLVM assembly file and
     * returns its path. The file will be located in the FileProcessor's
     * temporary directory.
     *
     * @param inputFile  The LLVM assembly file to translate.
     * @return           The resulting LLVM bitcode file's path.
     */
    Path generateBitcode(const Path& inputFile);

    /**
     * Generates an native assembly file from the passed LLVM bitcode file and
     * returns its path. (If the FileProcessor's target output format is native
     * assembly, the file will be located in the input file's directory.
     * Otherwise, it is stored in the FileProcessor's temporary directory.)
     *
     * @param inputFile  The LLVM bitcode file to translate.
     * @return           The resulting native assembly file's path.
     */
    Path generateAssembly(const Path& inputFile);

    /**
     * Generates a binary object file from the passed native assembly file
     * and returns its path. The file will be located in the input file's
     * directory.
     *
     * @param inputFile  The LLVM assembly file to translate.
     * @return           The resulting native assembly file's path.
     */
    Path generateBinary(const Path& inputFile);
};

}}

#endif
