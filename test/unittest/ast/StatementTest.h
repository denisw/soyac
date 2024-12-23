/*
 * soyac - The Soya programming language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef SOYA_STATEMENT_TEST_H_
#define SOYA_STATEMENT_TEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "DummyStatement.h"

using namespace soyac::ast;

class StatementTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE (StatementTest);
    CPPUNIT_TEST_SUITE_END ();

public:
    void setUp()
    {
        mStmt = new DummyStatement;
    }

    void tearDown()
    {
        delete mStmt;
    }

private:
    Statement *mStmt;
};

#endif
