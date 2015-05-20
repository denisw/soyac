/*
 * soyac - The Soya programming language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _FLOATING_POINT_LITERAL_TEST_HPP
#define _FLOATING_POINT_LITERAL_TEST_HPP

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <ast/FloatingPointLiteral.hpp>
#include <ast/FloatingPointType.hpp>
#include "UnitTestUtil.hpp"

using namespace soyac::ast;

class FloatingPointLiteralTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE (FloatingPointLiteralTest);
    CPPUNIT_TEST (testVisit);
    CPPUNIT_TEST (testType);
    CPPUNIT_TEST (testValue);
    CPPUNIT_TEST_SUITE_END ();

    TEST_VISITOR (FloatingPointLiteral);

public:
    void setUp()
    {
        mLiteral = new FloatingPointLiteral(FloatingPointValue(10.4, false));
    }

    void tearDown()
    {
        delete mLiteral;
    }

    void testVisit()
    {
        FloatingPointLiteralTestVisitor v;
        CPPUNIT_ASSERT (((bool) mLiteral->visit(&v)) == true);
    }

    void testType()
    {
        CPPUNIT_ASSERT (mLiteral->type() == TYPE_DOUBLE);
    }

    void testValue()
    {
        CPPUNIT_ASSERT (mLiteral->value().data() == 10.4);
        CPPUNIT_ASSERT (mLiteral->value().isSinglePrecision() == false);
    }

private:
    FloatingPointLiteral* mLiteral;
};

#endif
