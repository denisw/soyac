/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _DO_STATEMENT_HPP
#define _DO_STATEMENT_HPP

#include "Expression.hpp"
#include "StatementWithBody.hpp"

namespace soyac {
namespace ast
{

/**
 * Represents an "do" statement.
 *
 * (See the Soya Language Reference,)
 */
class DoStatement : public StatementWithBody
{
public:
    /**
     * Creates a DoStatement.
     *
     * @param condition  The loop condition.
     * @param body       The "do" statement's body.
     */
    DoStatement(Expression* condition, Statement* body);

    /**
     * Visits the DoStatement.
     *
     * @see Node::visit()
     */
    virtual void* visit(Visitor* v);

    /**
     * Returns the expression that determines if the "do" statement's body
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
