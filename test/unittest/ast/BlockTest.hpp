/*
 * soyac - The Soya programming language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _BLOCK_TEST_HPP
#define _BLOCK_TEST_HPP

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <ast/Block.hpp>
#include "DummyStatement.hpp"
#include "UnitTestUtil.hpp"

using namespace soyac::ast;

class BlockTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE (BlockTest);
    CPPUNIT_TEST (testVisit);
    CPPUNIT_TEST (testStatements);
    CPPUNIT_TEST (testAddStatement);
    CPPUNIT_TEST_SUITE_END ();

    TEST_VISITOR (Block);

public:
    void setUp()
    {
        mStmt1 = new DummyStatement;
        mStmt2 = new DummyStatement;
        mStmt3 = new DummyStatement;

        mBlock = new Block;
        mBlock->addStatement(mStmt1);
        mBlock->addStatement(mStmt2);
        mBlock->addStatement(mStmt3);
    }

    void tearDown()
    {
        delete mBlock;
    }

    void testVisit()
    {
        BlockTestVisitor v;
        CPPUNIT_ASSERT (((bool) mBlock->visit(&v)) == true);
    }

    void testStatements()
    {
        Block::statements_iterator it = mBlock->statements_begin();

        CPPUNIT_ASSERT (*it == mStmt1);
        it++;
        CPPUNIT_ASSERT (*it == mStmt2);
        it++;
        CPPUNIT_ASSERT (*it == mStmt3);
        it++;
        CPPUNIT_ASSERT (it == mBlock->statements_end());
    }

    void testAddStatement()
    {
        DummyStatement stmt4;
        mBlock->addStatement(&stmt4);

        Block::statements_iterator it = mBlock->statements_begin();

        CPPUNIT_ASSERT (*it == mStmt1);
        it++;
        CPPUNIT_ASSERT (*it == mStmt2);
        it++;
        CPPUNIT_ASSERT (*it == mStmt3);
        it++;
        CPPUNIT_ASSERT (*it == &stmt4);
        it++;
        CPPUNIT_ASSERT (it == mBlock->statements_end());
    }

private:
    Block* mBlock;
    Statement* mStmt1;
    Statement* mStmt2;
    Statement* mStmt3;
};

#endif
