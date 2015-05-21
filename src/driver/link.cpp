/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include <iostream>
#include <cstdlib>
#include <llvm/System/Path.h>
#include <llvm/System/Program.h>
#include "link.hpp"
#include "config.hpp"

using llvm::sys::Path;
using llvm::sys::Program;

namespace soyac {
namespace driver
{


void
linkFiles(const std::list<std::string>& objectFiles)
{
    Path gcc = Program::FindProgramByName("cc");

    if (!gcc.isValid())
    {
        std::cerr << config::programName << ": could not find cc" << std::endl;
        std::exit(1);
    }

    std::vector<const char*> args;
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

    args.push_back(NULL);

    const char** argv = new const char*[args.size()];
    std::copy(args.begin(), args.end(), argv);

    Program::ExecuteAndWait(gcc, argv);
    delete [] argv;
}


}}
