/*
 * soyac - The Soya programming language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _DUMMY_EXPRESSION_HPP
#define _DUMMY_EXPRESSION_HPP

#include "UnitTestUtil.hpp"
#include "../../../src/ast/Expression.hpp"
#include "../../../src/ast/UnknownType.hpp"

class DummyExpression : public soyac::ast::Expression
{
public:
    DummyExpression(soyac::ast::Type* t = TYPE_UNKNOWN)
        : mType(t)
    {
    }

    virtual void* visit(soyac::ast::Visitor* v)
    {
        return 0;
    }

    virtual soyac::ast::Type* type() const
    {
        return mType;
    }

private:
    soyac::ast::Type* mType;
};


#endif
