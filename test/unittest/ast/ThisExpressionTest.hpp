/*
 * soyac - The Soya programming language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _THIS_EXPRESSION_TEST_HPP
#define _THIS_EXPRESSION_TEST_HPP

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <ast/ThisExpression.hpp>
#include <ast/UnknownType.hpp>
#include "DummyType.hpp"
#include "UnitTestUtil.hpp"

using namespace soyac::ast;

class ThisExpressionTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE (ThisExpressionTest);
    CPPUNIT_TEST (testVisit);
    CPPUNIT_TEST (testType);
    CPPUNIT_TEST (testTypeReplaceable);
    CPPUNIT_TEST_SUITE_END ();

    TEST_VISITOR (ThisExpression);

public:
    void setUp()
    {
        mType = new DummyType;
        mExpr = new ThisExpression(mType);
    }

    void tearDown()
    {
        delete mExpr;
    }

    void testVisit()
    {
        ThisExpressionTestVisitor v;
        CPPUNIT_ASSERT (((bool) mExpr->visit(&v)) == true);
    }

    void testType()
    {
        CPPUNIT_ASSERT (mExpr->type() == mType);
    }

    void testTypeReplaceable()
    {
        DummyType type2;
        mType->replaceWith(&type2);
        CPPUNIT_ASSERT (mExpr->type() == &type2);
    }

private:
    ThisExpression* mExpr;
    Type* mType;
};

#endif
