/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef SOYA_MANGLING_H_
#define SOYA_MANGLING_H_

#include "ast/ast.h"
#include <string>

namespace soyac::codegen
{

/**
 * Returns the mangled symbol name of the passed entity.
 *
 * @param entity  The named entity.
 * @return        The mangled name.
 */
std::string
mangledName(ast::NamedEntity* entity);

} // namespace soyac::codegen

#endif // MANGLING_H
