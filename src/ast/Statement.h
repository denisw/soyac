/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef SOYA_STATEMENT_H_
#define SOYA_STATEMENT_H_

#include "Link.h"
#include "Node.h"

namespace soyac {
namespace ast {

/**
 * Inherited by node classes representing statements.
 *
 * (See the Soya Language Reference, Chapter 9.)
 */
class Statement : public Node {
public:
    /**
     * Initializes a Statement.
     */
    Statement();
};

} // namespace ast
} // namespace soyac

#endif
