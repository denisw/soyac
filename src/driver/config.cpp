/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include <cstdlib>
#include "config.hpp"

namespace soyac {
namespace driver {
namespace config
{

bool compileOnly = false;
bool emitAssembly = false;
bool emitLLVM = false;
const char* defaultInterfacePaths[] = {
    "/usr/local/include/soya",
    "/usr/include/soya",
    NULL
};
const char** interfacePaths = NULL;
const char* outputPath = "./a.out";
const char* programName = "soyac";
const char** sourcePaths = NULL;

}}}
