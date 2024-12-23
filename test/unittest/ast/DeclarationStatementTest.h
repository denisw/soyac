/*
 * soyac - The Soya programming language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef SOYA_DECLARATION_STATEMENT_TEST_H_
#define SOYA_DECLARATION_STATEMENT_TEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <ast/DeclarationStatement.h>
#include "DummyDeclaredEntity.h"
#include "UnitTestUtil.h"

using namespace soyac::ast;

class DeclarationStatementTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE (DeclarationStatementTest);
    CPPUNIT_TEST (testVisit);
    CPPUNIT_TEST (testDeclaredEntity);
    CPPUNIT_TEST (testDeclaredEntityReplaceable);
    CPPUNIT_TEST_SUITE_END ();

    TEST_VISITOR (DeclarationStatement);

public:
    void setUp()
    {
        mDecl = new DummyDeclaredEntity;
        mStmt = new DeclarationStatement(mDecl);
    }

    void tearDown()
    {
        delete mStmt;
    }

    void testVisit()
    {
        DeclarationStatementTestVisitor v;
        CPPUNIT_ASSERT (((bool) mStmt->visit(&v)) == true);
    }

    void testDeclaredEntity()
    {
        CPPUNIT_ASSERT (mStmt->declaredEntity() == mDecl);
    }

    void testDeclaredEntityReplaceable()
    {
        DummyDeclaredEntity decl2;
        mDecl->replaceWith(&decl2);
        CPPUNIT_ASSERT (mStmt->declaredEntity() == &decl2);
    }

private:
    DeclarationStatement* mStmt;
    DummyDeclaredEntity* mDecl;
};

#endif
