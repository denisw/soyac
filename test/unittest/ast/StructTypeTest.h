/*
 * soyac - The Soya programming language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef SOYA_STRUCT_TYPE_TEST_H_
#define SOYA_STRUCT_TYPE_TEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <ast/DeclarationBlock.h>
#include <ast/DeclarationStatement.h>
#include <ast/StructType.h>
#include "UnitTestUtil.h"
#include "DummyDeclaredEntity.h"

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
