/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include "config.hpp"

namespace soyac {
namespace driver {
namespace config
{

bool compileOnly = false;
bool emitAssembly = false;
bool emitLLVM = false;
std::vector<std::string> interfacePaths{};
std::vector<std::string> libraryPaths{};
std::string outputPath("./a.out");
std::string programName("soyac");
std::vector<std::string> sourcePaths{};

std::vector<std::string> defaultInterfacePaths = {
    "/usr/local/include/soya",
    "/usr/local/include",
    "/usr/include/soya",
    "/usr/include"
};

}}}
