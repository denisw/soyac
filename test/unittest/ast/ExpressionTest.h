/*
 * soyac - The Soya programming language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef SOYA_EXPRESSION_TEST_H_
#define SOYA_EXPRESSION_TEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <ast/Expression.h>
#include "DummyExpression.h"

using namespace soyac::ast;

class ExpressionTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE (ExpressionTest);
    CPPUNIT_TEST_SUITE_END ();

public:
    void setUp()
    {
        mExpr = new DummyExpression;
    }

    void tearDown()
    {
        delete mExpr;
    }

private:
    Expression* mExpr;
};

#endif
