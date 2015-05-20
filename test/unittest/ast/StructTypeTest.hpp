/*
 * soyac - The Soya programming language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _STRUCT_TYPE_TEST_HPP
#define _STRUCT_TYPE_TEST_HPP

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <ast/DeclarationBlock.hpp>
#include <ast/DeclarationStatement.hpp>
#include <ast/StructType.hpp>
#include "UnitTestUtil.hpp"
#include "DummyDeclaredEntity.hpp"

using namespace soyac::ast;

class StructTypeTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE (StructTypeTest);
    CPPUNIT_TEST (testVisit);
    CPPUNIT_TEST_SUITE_END ();

    TEST_VISITOR (StructType);

public:
    void setUp()
    {
        mMember = new DummyDeclaredEntity("bar");
        
        DeclarationStatement* dstmt = new DeclarationStatement(mMember);
        mBody = new DeclarationBlock(&dstmt, &dstmt + 1);

        mType = new StructType("Foo", mBody);
    }

    void tearDown()
    {
        delete mType;
    }

    void testVisit()
    {
        StructTypeTestVisitor v;
        CPPUNIT_ASSERT (((bool) mType->visit(&v)) == true);
    }

private:
    StructType* mType;
    DeclarationBlock* mBody;
    DeclaredEntity* mMember;    
};

#endif
