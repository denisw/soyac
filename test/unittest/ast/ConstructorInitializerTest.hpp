/*
 * soyac - The Soya programming language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _CONSTRUCTOR_INITIALIZER_TEST_HPP
#define _CONSTRUCTOR_INITIALIZER_TEST_HPP

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <ast/ConstructorInitializer.hpp>
#include <ast/UnresolvedBaseConstructorInitializer.hpp>
#include "DummyExpression.hpp"
#include "UnitTestUtil.hpp"

using namespace soyac::ast;

class ConstructorInitializerTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE (ConstructorInitializerTest);
    CPPUNIT_TEST (testVisit);
    CPPUNIT_TEST (testArguments);
    CPPUNIT_TEST_SUITE_END ();

    TEST_VISITOR (ConstructorInitializer);

public:
    void setUp()
    {
        mCons =
          new Constructor(new UnresolvedBaseConstructorInitializer, new Block);

        mArgs = new Expression*[3];
        mArgs[0] = new DummyExpression;
        mArgs[1] = new DummyExpression;
        mArgs[2] = new DummyExpression;

        mExpr = new ConstructorInitializer(mCons, mArgs, mArgs + 3);
    }

    void tearDown()
    {
        delete mExpr;
        delete mArgs;
    }

    void testVisit()
    {
        ConstructorInitializerTestVisitor v;
        CPPUNIT_ASSERT (((bool) mExpr->visit(&v)) == true);
    }

    void testArguments()
    {
        ConstructorInitializer::arguments_iterator it =
          mExpr->arguments_begin();

        CPPUNIT_ASSERT (*it == mArgs[0]);
        it++;
        CPPUNIT_ASSERT (*it == mArgs[1]);
        it++;
        CPPUNIT_ASSERT (*it == mArgs[2]);
        it++;
        CPPUNIT_ASSERT (it == mExpr->arguments_end());
    }

private:
    ConstructorInitializer* mExpr;
    Constructor* mCons;
    Expression** mArgs;
};

#endif
