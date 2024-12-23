/*
 * soyac - The Soya programming language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef SOYA_ARRAY_CREATION_EXPRESSION_TEST_H_
#define SOYA_ARRAY_CREATION_EXPRESSION_TEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <ast/ArrayCreationExpression.h>
#include <ast/IntegerType.h>
#include "DummyExpression.h"
#include "UnitTestUtil.h"

using namespace soyac::ast;

class ArrayCreationExpressionTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE (ArrayCreationExpressionTest);
    CPPUNIT_TEST (testVisit);
    CPPUNIT_TEST (testType);
    CPPUNIT_TEST (testLengthExpression);
    CPPUNIT_TEST (testElements);
    CPPUNIT_TEST_SUITE_END ();

    TEST_VISITOR (ArrayCreationExpression);

public:
    void setUp()
    {
        mType = ArrayType::get(TYPE_INT);
        mLengthExpression = new DummyExpression;

        mElements = new Expression*[3];
        mElements[0] = new DummyExpression;
        mElements[1] = new DummyExpression;
        mElements[2] = new DummyExpression;

        mExpr = new ArrayCreationExpression(mType, mLengthExpression,
                                            mElements, mElements + 3);
    }

    void tearDown()
    {
        delete mExpr;
        delete [] mElements;
    }

    void testVisit()
    {
        ArrayCreationExpressionTestVisitor v;
        CPPUNIT_ASSERT (((bool) mExpr->visit(&v)) == true);
    }

    void testType()
    {
        CPPUNIT_ASSERT (mExpr->type() == mType);
    }

    void testLengthExpression()
    {
        CPPUNIT_ASSERT (mExpr->lengthExpression() == mLengthExpression);
    }

    void testElements()
    {
        ArrayCreationExpression::elements_iterator it = mExpr->elements_begin();

        CPPUNIT_ASSERT (*it == mElements[0]);
        it++;
        CPPUNIT_ASSERT (*it == mElements[1]);
        it++;
        CPPUNIT_ASSERT (*it == mElements[2]);
        it++;
        CPPUNIT_ASSERT (it == mExpr->elements_end());
    }

private:
    ArrayCreationExpression* mExpr;
    ArrayType* mType;
    Expression* mLengthExpression;
    Expression** mElements;
};

#endif
