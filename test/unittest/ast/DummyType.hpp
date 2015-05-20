/*
 * soyac - The Soya programming language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _DUMMY_TYPE_HPP
#define _DUMMY_TYPE_HPP

#include "UnitTestUtil.hpp"
#include "../../../src/ast/Type.hpp"
#include "../../../src/ast/Name.hpp"

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
        return NULL;
    }
};

#endif
