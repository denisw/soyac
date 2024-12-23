/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _RETURN_STATEMENT_HPP
#define _RETURN_STATEMENT_HPP

#include "Expression.hpp"
#include "Statement.hpp"

namespace soyac {
namespace ast {

/**
 * Represents a return statement.
 *
 * (See the Soya Language Reference,)
 */
class ReturnStatement : public Statement {
public:
    /**
     * Creates a ReturnStatement.
     *
     * @param returnValue  The return value. May be null if the return
     *                     statement doesn't return a value.
     */
    ReturnStatement(Expression* returnValue = NULL);

    /**
     * Visits the ReturnStatement.
     *
     * @see Node::visit()
     */
    virtual void* visit(Visitor* v);

    /**
     * Returns the expression that yields the return statement's return
     * value.
     *
     * @return  The return value.
     */
    Expression* returnValue() const;

private:
    Link<Expression> mReturnValue;
};

} // namespace ast
} // namespace soyac

#endif
