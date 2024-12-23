/*
 * soyac - The Soya programming language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef SOYA_DUMMY_EXPRESSION_H_
#define SOYA_DUMMY_EXPRESSION_H_

#include "UnitTestUtil.h"
#include "../../../src/ast/Expression.h"
#include "../../../src/ast/UnknownType.h"

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
