/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _STATEMENT_HPP
#define _STATEMENT_HPP

#include "Link.hpp"
#include "Node.hpp"

namespace soyac {
namespace ast
{

/**
 * Inherited by node classes representing statements.
 *
 * (See the Soya Language Reference, Chapter 9.)
 */
class Statement : public Node
{
public:
    /**
     * Initializes a Statement.
     */
    Statement();
};

}}

#endif
