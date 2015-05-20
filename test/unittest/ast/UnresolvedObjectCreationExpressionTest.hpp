/*
 * soyac - The Soya programming language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _UNRESOLVED_OBJECT_CREATION_EXPRESSION_TEST_HPP
#define _UNRESOLVED_OBJECT_CREATION_EXPRESSION_TEST_HPP

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <ast/Constructor.hpp>
#include <ast/UnresolvedObjectCreationExpression.hpp>
#include <ast/UnknownType.hpp>
#include "DummyExpression.hpp"
#include "UnitTestUtil.hpp"

using namespace soyac::ast;

class UnresolvedObjectCreationExpressionTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE (UnresolvedObjectCreationExpressionTest);
    CPPUNIT_TEST (testVisit);
    CPPUNIT_TEST (testType);
    CPPUNIT_TEST (testArguments);
    CPPUNIT_TEST_SUITE_END ();

    TEST_VISITOR (UnresolvedObjectCreationExpression);

public:
    void setUp()
    {
        mType = new UnknownType("Foo");

        mArgs = new Expression*[3];
        mArgs[0] = new DummyExpression;
        mArgs[1] = new DummyExpression;
        mArgs[2] = new DummyExpression;

        mExpr = new UnresolvedObjectCreationExpression(mType,
                                                       mArgs,
                                                       mArgs + 3);
    }

    void tearDown()
    {
        delete mExpr;
        delete [] mArgs;
    }

    void testVisit()
    {
        UnresolvedObjectCreationExpressionTestVisitor v;
        CPPUNIT_ASSERT (((bool) mExpr->visit(&v)) == true);
    }

    void testType()
    {
        CPPUNIT_ASSERT (mExpr->type() == mType);
    }

    void testArguments()
    {
        UnresolvedObjectCreationExpression::arguments_iterator it = mExpr->arguments_begin();

        CPPUNIT_ASSERT (*it == mArgs[0]);
        it++;
        CPPUNIT_ASSERT (*it == mArgs[1]);
        it++;
        CPPUNIT_ASSERT (*it == mArgs[2]);
        it++;
        CPPUNIT_ASSERT (it == mExpr->arguments_end());
    }

private:
    UnresolvedObjectCreationExpression* mExpr;
    UnknownType* mType;
    Constructor* mConstructor;
    Expression** mArgs;
};

#endif
