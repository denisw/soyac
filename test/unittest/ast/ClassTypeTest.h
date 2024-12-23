/*
 * soyac - The Soya programming language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef SOYA_CLASS_TYPE_TEST_H_
#define SOYA_CLASS_TYPE_TEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <ast/ClassType.h>
#include <ast/DeclarationBlock.h>
#include <ast/DeclarationStatement.h>
#include "UnitTestUtil.h"
#include "DummyDeclaredEntity.h"

using namespace soyac::ast;

class ClassTypeTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE (ClassTypeTest);
    CPPUNIT_TEST (testVisit);
    CPPUNIT_TEST (testBaseClass);
    CPPUNIT_TEST_SUITE_END ();

    TEST_VISITOR (ClassType);

public:
    void setUp()
    {
        mMember = new DummyDeclaredEntity("bar");
        
        DeclarationStatement* dstmt = new DeclarationStatement(mMember);
        mBody = new DeclarationBlock(&dstmt, &dstmt + 1);

        mType = new ClassType("Foo", mBody);
    }

    void tearDown()
    {
        delete mType;
    }

    void testVisit()
    {
        ClassTypeTestVisitor v;
        CPPUNIT_ASSERT (((bool) mType->visit(&v)) == true);
    }

    void testBaseClass()
    {
        DeclaredEntity* member = new DummyDeclaredEntity("baz");
        DeclarationStatement* dstmt = new DeclarationStatement(member);
        DeclarationBlock* body = new DeclarationBlock(&dstmt, &dstmt + 1);

        ClassType* child = new ClassType("Bar", mType, body);
        CPPUNIT_ASSERT (child->baseClass() == mType);   
    }
private:
    ClassType* mType;
    DeclarationBlock* mBody;
    DeclaredEntity* mMember;    
};

#endif
