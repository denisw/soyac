/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _LINK__HPP
#define _LINK__HPP

#include <list>
#include <string>

namespace soyac {
namespace driver
{

/**
 * Links the passed list of object files together to an executable
 * or shared library (depending on the value set for options::linkShared).
 * The resulting binary file is placed at the path stored in
 * options::outputPath.
 *
 * @param objectFiles  The object files to link. 
 */ 
void linkFiles(const std::list<std::string>& objectFiles);

}}

#endif
