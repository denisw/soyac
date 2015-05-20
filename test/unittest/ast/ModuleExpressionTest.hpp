/*
 * soyac - The Soya programming language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _MODULE_EXPRESSION_TEST_HPP
#define _MODULE_EXPRESSION_TEST_HPP

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <ast/ModuleExpression.hpp>
#include <ast/Module.hpp>
#include <ast/UnknownType.hpp>
#include "DummyType.hpp"
#include "UnitTestUtil.hpp"

using namespace soyac::ast;

class ModuleExpressionTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE (ModuleExpressionTest);
    CPPUNIT_TEST (testVisit);
    CPPUNIT_TEST (testType);
    CPPUNIT_TEST (testTarget);
    CPPUNIT_TEST_SUITE_END ();

    TEST_VISITOR (ModuleExpression);

public:
    void setUp()
    {
        mTarget = Module::get("foo::bar", true);
        mExpr = new ModuleExpression(mTarget);
    }

    void tearDown()
    {
        delete mExpr;
    }

    void testVisit()
    {
        ModuleExpressionTestVisitor v;
        CPPUNIT_ASSERT (((bool) mExpr->visit(&v)) == true);
    }

    void testType()
    {
        CPPUNIT_ASSERT (mExpr->type() == TYPE_UNKNOWN);
    }

    void testTarget()
    {
        CPPUNIT_ASSERT (mExpr->target() == mTarget);
    }

private:
    ModuleExpression* mExpr;
    Module* mTarget;
};

#endif
