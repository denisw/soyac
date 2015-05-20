/*
 * soyac - The Soya programming language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _UNRESOLVED_BASE_CONSTRUCTOR_INITIALIZER_TEST_HPP
#define _UNRESOLVED_BASE_CONSTRUCTOR_INITIALIZER_TEST_HPP

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <ast/UnresolvedBaseConstructorInitializer.hpp>
#include "DummyExpression.hpp"
#include "UnitTestUtil.hpp"

using namespace soyac::ast;

class UnresolvedBaseConstructorInitializerTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE (UnresolvedBaseConstructorInitializerTest);
    CPPUNIT_TEST (testVisit);
    CPPUNIT_TEST_SUITE_END ();

    TEST_VISITOR (UnresolvedBaseConstructorInitializer);

public:
    void setUp()
    {
        mExpr = new UnresolvedBaseConstructorInitializer;
    }

    void tearDown()
    {
        delete mExpr;
    }

    void testVisit()
    {
        UnresolvedBaseConstructorInitializerTestVisitor v;
        CPPUNIT_ASSERT (((bool) mExpr->visit(&v)) == true);
    }

private:
    UnresolvedBaseConstructorInitializer* mExpr;
};

#endif
