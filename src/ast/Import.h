/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef SOYA_IMPORT_H_
#define SOYA_IMPORT_H_

#include "Link.h"
#include "Node.h"

namespace soyac {
namespace ast {

/**
 * Inherited by node classes representing imports into modules.
 *
 * (See the Soya Language Reference, Chapter 9.)
 */
class Import : public Node {
public:
    /**
     * Initializes an Import.
     */
    Import();
};

} // namespace ast
} // namespace soyac

#endif
