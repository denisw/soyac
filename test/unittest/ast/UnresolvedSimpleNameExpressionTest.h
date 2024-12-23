/*
 * soyac - The Soya programming language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef SOYA_SIMPLE_NAME_EXPRESSION_TEST_H_
#define SOYA_SIMPLE_NAME_EXPRESSION_TEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <ast/UnresolvedSimpleNameExpression.h>
#include <ast/UnknownType.h>
#include "UnitTestUtil.h"

using namespace soyac::ast;

class UnresolvedSimpleNameExpressionTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE (UnresolvedSimpleNameExpressionTest);
    CPPUNIT_TEST (testVisit);
    CPPUNIT_TEST (testType);
    CPPUNIT_TEST (testName);
    CPPUNIT_TEST_SUITE_END ();

    TEST_VISITOR (UnresolvedSimpleNameExpression);

public:
    void setUp()
    {
        mExpr = new UnresolvedSimpleNameExpression(Name("foo"));
    }

    void tearDown()
    {
        delete mExpr;
    }

    void testVisit()
    {
        UnresolvedSimpleNameExpressionTestVisitor v;
        CPPUNIT_ASSERT (((bool) mExpr->visit(&v)) == true);
    }

    void testType()
    {
        CPPUNIT_ASSERT (mExpr->type() == TYPE_UNKNOWN);
    }

    void testName()
    {
        CPPUNIT_ASSERT (mExpr->name() == Name("foo"));
    }

private:
    UnresolvedSimpleNameExpression* mExpr;
};

#endif
