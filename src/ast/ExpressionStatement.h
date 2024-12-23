/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef SOYA_EXPRESSION_STATEMENT_H_
#define SOYA_EXPRESSION_STATEMENT_H_

#include "Expression.h"
#include "Statement.h"

namespace soyac {
namespace ast {

/**
 * Represents an expression statement.
 *
 * (See the Soya Language Reference,)
 */
class ExpressionStatement : public Statement {
public:
    /**
     * Creates an ExpressionStatement.
     *
     * @param expr  The expression.
     */
    ExpressionStatement(Expression* expr);

    /**
     * Visits the ExpressionStatement.
     *
     * @see Node::visit()
     */
    virtual void* visit(Visitor* v);

    /**
     * Returns the expression evaluated by the expression statement.
     *
     * @return  The expression.
     */
    Expression* expression() const;

private:
    Link<Expression> mExpression;
};

} // namespace ast
} // namespace soyac

#endif
