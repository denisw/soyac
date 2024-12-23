/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _CONFIG_HPP
#define _CONFIG_HPP

#include <string>
#include <vector>

namespace soyac {
namespace driver {
namespace config {

/**
 * Whether the input files should only be compiled, but not linked.
 * The default value is @c false.
 */
extern bool compileOnly;

/**
 * The default paths in which to search for interface files.
 */
extern std::vector<std::string> defaultInterfacePaths;

/**
 * Whether native assembly code should be the output format.
 * The default value is @c false.
 */
extern bool emitAssembly;

/**
 * Whether LLVM assembly code should be the output format.
 * The default value is @c false.
 */
extern bool emitLLVM;

/**
 * The paths in which to search for interface files.
 */
extern std::vector<std::string> interfacePaths;

/**
 * The paths in which to search for library files.
 */
extern std::vector<std::string> libraryPaths;

/**
 * Where to put the generated binary file when linking.
 */
extern std::string outputPath;

/**
 * The name of the 'soyac' executable.
 */
extern std::string programName;

/**
 * The paths in which to search for source files.
 */
extern std::vector<std::string> sourcePaths;

/**
 * Returns @c true object files resulting from compilation should be
 * linked together.
 */
inline bool linkingRequested()
{
    return (!emitLLVM && !emitAssembly && !compileOnly);
}

} // namespace config
} // namespace driver
} // namespace soyac

#endif
