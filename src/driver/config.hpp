/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _CONFIG_HPP
#define _CONFIG_HPP

namespace soyac {
namespace driver {
namespace config
{

/**
 * Whether the input files should only be compiled, but not linked.
 * The default value is @c false.
 */
extern bool compileOnly;

/**
 * The default paths in which to search for interface files.
 */
extern const char* defaultInterfacePaths[];

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
extern const char** interfacePaths;

/**
 * Where to put the generated binary file when linking.
 */
extern const char* outputPath;

/**
 * The name of the 'soyac' executable.
 */
extern const char* programName;

/**
 * The paths in which to search for source files.
 */
extern const char** sourcePaths;

/**
 * Returns @c true object files resulting from compilation should be linked
 * together.
 */
inline bool linkingRequested()
{
    return (!emitLLVM && !emitAssembly && !compileOnly); 
}

}}}

#endif
