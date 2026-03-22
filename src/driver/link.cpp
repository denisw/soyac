/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include "link.h"
#include "config.h"
#include <cstdlib>
#include <iostream>
#include <llvm/Support/Program.h>
#include <string>

namespace soyac {
namespace driver {

void linkFiles(const std::vector<std::string>& objectFiles)
{
    llvm::ErrorOr<std::string> gcc = llvm::sys::findProgramByName("cc");

    if (std::error_code code = gcc.getError()) {
        std::cerr << config::programName << ": could not find cc" << std::endl;
        std::exit(1);
    }

    std::vector<llvm::StringRef> args;
    args.push_back("ld");

    for (const auto& op : objectFiles) {
        args.push_back(op);
    }

    for (const auto& lp : config::libraryPaths) {
        args.push_back("-L");
        args.push_back(lp);
    }

    args.push_back("-lsr");
    args.push_back("-lgc");
    args.push_back("-o");
    args.push_back(config::outputPath);

    llvm::sys::ExecuteAndWait(*gcc, args);
}

} // namespace driver
} // namespace soyac
