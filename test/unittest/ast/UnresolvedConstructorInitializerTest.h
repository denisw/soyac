/*
 * soyac - The Soya programming language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef SOYA_UNRESOLVED_CONSTRUCTOR_INITIALIZER_TEST_H_
#define SOYA_UNRESOLVED_CONSTRUCTOR_INITIALIZER_TEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <ast/UnresolvedConstructorInitializer.h>
#include "DummyExpression.h"
#include "UnitTestUtil.h"

using namespace soyac::ast;

class UnresolvedConstructorInitializerTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE (UnresolvedConstructorInitializerTest);
    CPPUNIT_TEST (testVisit);
    CPPUNIT_TEST_SUITE_END ();

    TEST_VISITOR (UnresolvedConstructorInitializer);

public:
    void setUp()
    {
        mExpr = new UnresolvedConstructorInitializer;
    }

    void tearDown()
    {
        delete mExpr;
    }

    void testVisit()
    {
        UnresolvedConstructorInitializerTestVisitor v;
        CPPUNIT_ASSERT (((bool) mExpr->visit(&v)) == true);
    }

private:
    UnresolvedConstructorInitializer* mExpr;
};

#endif
