/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _WHILE_STATEMENT_HPP
#define _WHILE_STATEMENT_HPP

#include "Expression.hpp"
#include "StatementWithBody.hpp"

namespace soyac {
namespace ast
{

/**
 * Represents an "while" statement.
 *
 * (See the Soya Language Reference,)
 */
class WhileStatement : public StatementWithBody
{
public:
    /**
     * Creates a WhileStatement.
     *
     * @param condition  The loop condition.
     * @param body       The "while" statement's body.
     */
    WhileStatement(Expression* condition, Statement* body);

    /**
     * Visits the WhileStatement.
     *
     * @see Node::visit()
     */
    virtual void* visit(Visitor* v);

    /**
     * Returns the expression that determines if the "while" statement's body
     * is executed (again).
     *
     * @return  The loop condition.
     */
    Expression* condition() const;

private:
    Link<Expression> mCondition;
};

}}

#endif
