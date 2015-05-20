/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _IF_STATEMENT_HPP
#define _IF_STATEMENT_HPP

#include "Expression.hpp"
#include "StatementWithBody.hpp"

namespace soyac {
namespace ast
{

/**
 * Represents an "if" statement.
 *
 * (See the Soya Language Reference,)
 */
class IfStatement : public StatementWithBody
{
public:
    /**
     * Creates a IfStatement.
     *
     * @param condition  The condition.
     * @param body       The "if" statement's body.
     * @param elseBody   The associated "else" statement's body, or null.
     */
    IfStatement(Expression* condition, Statement* body, Statement* elseBody);

    /**
     * Visits the IfStatement.
     *
     * @see Node::visit()
     */
    virtual void* visit(Visitor* v);

    /**
     * Returns the expression that determines if the "if" statement's body
     * is executed.
     *
     * @return  The "if" condition.
     */
    Expression* condition() const;

    /**
     * Returns the body of the "else" statement associated with the
     * "if" statement, if any. May be null if the "if" statement has
     * no associated "else" statement.
     *
     * @return  The associated "else" statement's body, or null.
     */
    Statement* elseBody() const;

private:
    Link<Expression> mCondition;
    Link<Statement> mElseBody;
};

}}

#endif
