/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include <iostream>
#include <cstdlib>
#include <string>
#include <llvm/Support/Program.h>
#include "link.hpp"
#include "config.hpp"

namespace soyac {
namespace driver
{


void
linkFiles(const std::list<std::string>& objectFiles)
{
  llvm::ErrorOr<std::string> gcc = llvm::sys::findProgramByName("cc");

    if (std::error_code code = gcc.getError())
    {
        std::cerr << config::programName << ": could not find cc" << std::endl;
        std::exit(1);
    }

    std::vector<llvm::StringRef> args;
    args.push_back("cc");

    for (std::list<std::string>::const_iterator it = objectFiles.begin();
         it != objectFiles.end(); it++)
    {
        args.push_back(it->c_str());
    }

    args.push_back("-lsr");
    args.push_back("-lgc");
    args.push_back("-o");
    args.push_back(config::outputPath);

    llvm::sys::ExecuteAndWait(*gcc, args);
}

}}
