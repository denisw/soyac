/*
 * soyac - The Soya programming language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _DUMMY_DECLARED_ENTITY_HPP
#define _DUMMY_DECLARED_ENTITY_HPP

#include "UnitTestUtil.hpp"
#include "../../../src/ast/DeclaredEntity.hpp"
#include "../../../src/ast/Name.hpp"

using soyac::ast::DeclaredEntity;

class DummyDeclaredEntity : public DeclaredEntity
{
public:
    DummyDeclaredEntity()
        : DeclaredEntity("DUMMY")
    {
    }

    DummyDeclaredEntity(const Name& name)
        : DeclaredEntity(name)
    {
    }

    virtual void* visit(soyac::ast::Visitor* v)
    {
        return NULL;
    }
};

#endif
