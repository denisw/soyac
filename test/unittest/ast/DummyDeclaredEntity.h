/*
 * soyac - The Soya programming language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef SOYA_DUMMY_DECLARED_ENTITY_H_
#define SOYA_DUMMY_DECLARED_ENTITY_H_

#include "UnitTestUtil.h"
#include "../../../src/ast/DeclaredEntity.h"
#include "../../../src/ast/Name.h"

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
        return nullptr;
    }
};

#endif
