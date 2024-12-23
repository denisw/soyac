/*
 * soyac - The Soya programming language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef SOYA_UNRESOLVED_DEFAULT_CONSTRUCTOR_INITIALIZER_TEST_H_
#define SOYA_UNRESOLVED_DEFAULT_CONSTRUCTOR_INITIALIZER_TEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <ast/UnresolvedDefaultConstructorInitializer.h>
#include "DummyExpression.h"
#include "UnitTestUtil.h"

using namespace soyac::ast;

class UnresolvedDefaultConstructorInitializerTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE (UnresolvedDefaultConstructorInitializerTest);
    CPPUNIT_TEST (testVisit);
    CPPUNIT_TEST_SUITE_END ();

    TEST_VISITOR (UnresolvedDefaultConstructorInitializer);

public:
    void setUp()
    {
        mExpr = new UnresolvedDefaultConstructorInitializer;
    }

    void tearDown()
    {
        delete mExpr;
    }

    void testVisit()
    {
        UnresolvedDefaultConstructorInitializerTestVisitor v;
        CPPUNIT_ASSERT (((bool) mExpr->visit(&v)) == true);
    }

private:
    UnresolvedDefaultConstructorInitializer* mExpr;
};

#endif
