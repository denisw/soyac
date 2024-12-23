/*
 * soyac - The Soya programming language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef SOYA_UNRESOLVED_THIS_EXPRESSION_TEST_H_
#define SOYA_UNRESOLVED_THIS_EXPRESSION_TEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <ast/UnresolvedThisExpression.h>
#include <ast/UnknownType.h>
#include "UnitTestUtil.h"

using namespace soyac::ast;

class UnresolvedThisExpressionTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE (UnresolvedThisExpressionTest);
    CPPUNIT_TEST (testVisit);
    CPPUNIT_TEST (testType);
    CPPUNIT_TEST_SUITE_END ();

    TEST_VISITOR (UnresolvedThisExpression);

public:
    void setUp()
    {
        mExpr = new UnresolvedThisExpression;
    }

    void tearDown()
    {
        delete mExpr;
    }

    void testVisit()
    {
        UnresolvedThisExpressionTestVisitor v;
        CPPUNIT_ASSERT (((bool) mExpr->visit(&v)) == true);
    }

    void testType()
    {
        CPPUNIT_ASSERT (mExpr->type() == TYPE_UNKNOWN);
    }

private:
    UnresolvedThisExpression* mExpr;
};

#endif
