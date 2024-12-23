/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _STATEMENT_WITH_BODY_HPP
#define _STATEMENT_WITH_BODY_HPP

#include "Statement.hpp"

namespace soyac {
namespace ast {

/**
 * Inherited by node classes representing statements which contain
 * another statement.
 *
 * (See the Soya Language Reference,)
 */
class StatementWithBody : public Statement {
public:
    /**
     * StatementWithBody constructor.
     *
     * @param body  The body statement.
     */
    StatementWithBody(Statement* body);

    /**
     * Returns the body statement contained by the statement.
     *
     * @return  The body statement.
     */
    Statement* body() const;

private:
    Link<Statement> mBody;
};

} // namespace ast
} // namespace soyac

#endif
