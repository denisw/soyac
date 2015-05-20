/*
 * soyac - The Soya programming language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _BOOLEAN_LITERAL_TEST_HPP
#define _BOOLEAN_LITERAL_TEST_HPP

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <ast/BooleanLiteral.hpp>
#include <ast/BooleanType.hpp>
#include "UnitTestUtil.hpp"

using namespace soyac::ast;

class BooleanLiteralTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE (BooleanLiteralTest);
    CPPUNIT_TEST (testVisit);
    CPPUNIT_TEST (testType);
    CPPUNIT_TEST (testValue);
    CPPUNIT_TEST_SUITE_END ();

    TEST_VISITOR (BooleanLiteral);

public:
    void setUp()
    {
        mLiteral = new BooleanLiteral(true);
    }

    void tearDown()
    {
        delete mLiteral;
    }

    void testVisit()
    {
        BooleanLiteralTestVisitor v;
        CPPUNIT_ASSERT (((bool) mLiteral->visit(&v)) == true);
    }

    void testType()
    {
        CPPUNIT_ASSERT (mLiteral->type() == TYPE_BOOL);
    }

    void testValue()
    {
        CPPUNIT_ASSERT (mLiteral->value() == true);
    }

private:
    BooleanLiteral* mLiteral;
};

#endif
