/*
 * soyac - The Soya programming language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _OBJECT_CREATION_EXPRESSION_TEST_HPP
#define _OBJECT_CREATION_EXPRESSION_TEST_HPP

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <ast/Constructor.hpp>
#include <ast/ObjectCreationExpression.hpp>
#include <ast/UnresolvedDefaultConstructorInitializer.hpp>
#include "DummyExpression.hpp"
#include "DummyType.hpp"
#include "UnitTestUtil.hpp"

using namespace soyac::ast;

class ObjectCreationExpressionTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE (ObjectCreationExpressionTest);
    CPPUNIT_TEST (testVisit);
    CPPUNIT_TEST (testType);
    CPPUNIT_TEST (testConstructor);
    CPPUNIT_TEST (testConstructorReplaceable);
    CPPUNIT_TEST (testArguments);
    CPPUNIT_TEST_SUITE_END ();

    TEST_VISITOR (ObjectCreationExpression);

public:
    void setUp()
    {
        mType = new DummyType;
        mConstructor =
          new Constructor(new UnresolvedDefaultConstructorInitializer, new Block);

        mArgs = new Expression*[3];
        mArgs[0] = new DummyExpression;
        mArgs[1] = new DummyExpression;
        mArgs[2] = new DummyExpression;

        mExpr = new ObjectCreationExpression(mType, mConstructor,
                                             mArgs, mArgs + 3);
    }

    void tearDown()
    {
        delete mExpr;
        delete [] mArgs;
    }

    void testVisit()
    {
        ObjectCreationExpressionTestVisitor v;
        CPPUNIT_ASSERT (((bool) mExpr->visit(&v)) == true);
    }

    void testType()
    {
        CPPUNIT_ASSERT (mExpr->type() == mType);
    }

    void testConstructor()
    {
        CPPUNIT_ASSERT (mExpr->constructor() == mConstructor);
    }

    void testConstructorReplaceable()
    {
        Constructor constructor2(new UnresolvedDefaultConstructorInitializer, new Block);
        mConstructor->replaceWith(&constructor2);
        CPPUNIT_ASSERT (mExpr->constructor() == &constructor2);
    }

    void testArguments()
    {
        ObjectCreationExpression::arguments_iterator it = mExpr->arguments_begin();

        CPPUNIT_ASSERT (*it == mArgs[0]);
        it++;
        CPPUNIT_ASSERT (*it == mArgs[1]);
        it++;
        CPPUNIT_ASSERT (*it == mArgs[2]);
        it++;
        CPPUNIT_ASSERT (it == mExpr->arguments_end());
    }

private:
    ObjectCreationExpression* mExpr;
    Type* mType;
    Constructor* mConstructor;
    Expression** mArgs;
};

#endif
