/*
 * soyac - The Soya programming language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _BUILT_IN_TYPE_TEST_HPP
#define _BUILT_IN_TYPE_TEST_HPP

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <ast/BuiltInType.hpp>
#include "DummyDeclaredEntity.hpp"
#include "UnitTestUtil.hpp"

using namespace soyac::ast;

class BuiltInTypeTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE (BuiltInTypeTest);
    CPPUNIT_TEST (testMembers);
    CPPUNIT_TEST_SUITE_END ();

    TEST_VISITOR (BuiltInType);

    class OpenBuiltInType : public BuiltInType
    {
    public:
        OpenBuiltInType(const Name& name)
            : BuiltInType(name)
        {
        }

        virtual void* visit(soyac::ast::Visitor* v)
        {
            return NULL;
        }
        
        void addMember(DeclaredEntity* member)
        {
            BuiltInType::addMember(member);
        }
    };

public:
    void setUp()
    {
        mMembers = new DeclaredEntity*[3];
        mMembers[0] = new DummyDeclaredEntity;
        mMembers[1] = new DummyDeclaredEntity;
        mMembers[2] = new DummyDeclaredEntity;

        mType = new OpenBuiltInType("foo");
        mType->addMember(mMembers[0]);
        mType->addMember(mMembers[1]);
        mType->addMember(mMembers[2]);
    }

    void tearDown()
    {
        delete mType;
        delete mMembers;
    }

    void testMembers()
    {
        BuiltInType::members_iterator it = mType->members_begin();

        CPPUNIT_ASSERT (*it == mMembers[0]);
        it++;
        CPPUNIT_ASSERT (*it == mMembers[1]);
        it++;
        CPPUNIT_ASSERT (*it == mMembers[2]);
        it++;
        CPPUNIT_ASSERT (it == mType->members_end());
    }

private:
    OpenBuiltInType* mType;
    DeclaredEntity** mMembers;
};

#endif
