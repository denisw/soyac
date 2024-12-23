/*
 * soyac - The Soya programming language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef SOYA_DECLARATION_BLOCK_TEST_H_
#define SOYA_DECLARATION_BLOCK_TEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <ast/DeclarationBlock.h>
#include <ast/DeclarationStatement.h>
#include "DummyDeclaredEntity.h"
#include "UnitTestUtil.h"

using namespace soyac::ast;

class DeclarationBlockTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE (DeclarationBlockTest);
    CPPUNIT_TEST (testVisit);
    CPPUNIT_TEST (testDeclarations);
    CPPUNIT_TEST (testAddDeclaration);
    CPPUNIT_TEST_SUITE_END ();

    TEST_VISITOR (DeclarationBlock);

public:
    void setUp()
    {
        mDecls = new DeclarationStatement*[3];
        mDecls[0] = new DeclarationStatement(new DummyDeclaredEntity);
        mDecls[1] = new DeclarationStatement(new DummyDeclaredEntity);
        mDecls[2] = new DeclarationStatement(new DummyDeclaredEntity);

        mBlock = new DeclarationBlock(mDecls, mDecls + 3);
    }

    void tearDown()
    {
        delete mBlock;
        delete [] mDecls;
    }

    void testVisit()
    {
        DeclarationBlockTestVisitor v;
        CPPUNIT_ASSERT (((bool) mBlock->visit(&v)) == true);
    }

    void testDeclarations()
    {
        DeclarationBlock::declarations_iterator it =
          mBlock->declarations_begin();

        CPPUNIT_ASSERT (*it == mDecls[0]);
        it++;
        CPPUNIT_ASSERT (*it == mDecls[1]);
        it++;
        CPPUNIT_ASSERT (*it == mDecls[2]);
        it++;
        CPPUNIT_ASSERT (it == mBlock->declarations_end());
    }

    void testAddDeclaration()
    {
        DeclarationStatement* dstmt =
          new DeclarationStatement(new DummyDeclaredEntity);

        mBlock->addDeclaration(dstmt);

        DeclarationBlock::declarations_iterator it =
          mBlock->declarations_begin();

        CPPUNIT_ASSERT (*it == mDecls[0]);
        it++;
        CPPUNIT_ASSERT (*it == mDecls[1]);
        it++;
        CPPUNIT_ASSERT (*it == mDecls[2]);
        it++;
        CPPUNIT_ASSERT (*it == dstmt);
        it++;
        CPPUNIT_ASSERT (it == mBlock->declarations_end());
    }

private:
    DeclarationBlock* mBlock;
    DeclarationStatement** mDecls;
};

#endif
