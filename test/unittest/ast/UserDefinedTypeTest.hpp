/*
 * soyac - The Soya programming language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _USER_DEFINED_TYPE_TEST_HPP
#define _USER_DEFINED_TYPE_TEST_HPP

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <ast/DeclarationBlock.hpp>
#include <ast/DeclarationStatement.hpp>
#include <ast/UserDefinedType.hpp>
#include "UnitTestUtil.hpp"
#include "DummyDeclaredEntity.hpp"

using namespace soyac::ast;

class UserDefinedTypeTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE (UserDefinedTypeTest);
    CPPUNIT_TEST (testBody);
    CPPUNIT_TEST (testQualifiedNameOfMembers);
    CPPUNIT_TEST_SUITE_END ();

    TEST_VISITOR (UserDefinedType);

public:
    void setUp()
    {
        mMember = new DummyDeclaredEntity("bar");
        
        DeclarationStatement* dstmt = new DeclarationStatement(mMember);
        mBody = new DeclarationBlock(&dstmt, &dstmt + 1);

        mType = new UserDefinedType("Foo", mBody);
    }

    void tearDown()
    {
        delete mType;
    }

    void testBody()
    {
        CPPUNIT_ASSERT (mType->body() == mBody);
    }

    void testQualifiedNameOfMembers()
    {
        Name qualifiedName = mMember->qualifiedName();
        Name::identifiers_iterator it = qualifiedName.identifiers_begin();

        CPPUNIT_ASSERT (*it == "Foo");
        it++;
        CPPUNIT_ASSERT (*it == "bar");
        it++;
        CPPUNIT_ASSERT (it == qualifiedName.identifiers_end());
    }

private:
    UserDefinedType* mType;
    DeclarationBlock* mBody;
    DeclaredEntity* mMember;    
};

#endif
