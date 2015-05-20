/*
 * soyac - The Soya programming language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _VARIABLE_TEST_HPP
#define _VARIABLE_TEST_HPP

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <ast/Variable.hpp>
#include "DummyExpression.hpp"
#include "DummyType.hpp"
#include "UnitTestUtil.hpp"

using namespace soyac::ast;

class VariableTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE (VariableTest);
    CPPUNIT_TEST (testVisit);
    CPPUNIT_TEST (testType);
    CPPUNIT_TEST (testTypeReplaceable);
    CPPUNIT_TEST (testInitializer);
    CPPUNIT_TEST (testInitializerReplaceable);
    CPPUNIT_TEST_SUITE_END ();

    TEST_VISITOR (Variable);

public:
    void setUp()
    {
        mType = new DummyType;
        mInit = new DummyExpression;
        mVar = new Variable("v", mType, mInit);
    }

    void tearDown()
    {
        delete mVar;
    }

    void testVisit()
    {
        VariableTestVisitor v;
        CPPUNIT_ASSERT (((bool) mVar->visit(&v)) == true);
    }

    void testType()
    {
        CPPUNIT_ASSERT (mVar->type() == mType);
    }

    void testTypeReplaceable()
    {
        DummyType type2;
        mType->replaceWith(&type2);
        CPPUNIT_ASSERT (mVar->type() == &type2);
    }

    void testInitializer()
    {
        CPPUNIT_ASSERT (mVar->initializer() == mInit);
    }

    void testInitializerReplaceable()
    {
        DummyExpression init2;
        mInit->replaceWith(&init2);
        CPPUNIT_ASSERT (mVar->initializer() == &init2);
    }

private:
    Variable* mVar;
    Type* mType;
    Expression* mInit;
};

#endif
