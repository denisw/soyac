/*
 * soyac - The Soya programming language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _UNRESOLVED_THIS_EXPRESSION_TEST_HPP
#define _UNRESOLVED_THIS_EXPRESSION_TEST_HPP

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <ast/UnresolvedThisExpression.hpp>
#include <ast/UnknownType.hpp>
#include "UnitTestUtil.hpp"

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
