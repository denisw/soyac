/*
 * soyac - The Soya programming language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef SOYA_DUMMY_TYPE_H_
#define SOYA_DUMMY_TYPE_H_

#include "UnitTestUtil.h"
#include "../../../src/ast/Type.h"
#include "../../../src/ast/Name.h"

using soyac::ast::Type;

class DummyType : public Type
{
public:
    DummyType()
        : Type("DUMMY")
    {
    }

    DummyType(const Name& name)
        : Type(name)
    {
    }

    virtual void* visit(soyac::ast::Visitor* v)
    {
        return nullptr;
    }
};

#endif
