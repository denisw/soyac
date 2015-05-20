/*
 * soyac - The Soya programming language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _FLOATING_POINT_VALUE_TEST_HPP
#define _FLOATING_POINT_VALUE_TEST_HPP

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <ast/FloatingPointValue.hpp>

using namespace soyac::ast;

class FloatingPointValueTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE (FloatingPointValueTest);
    CPPUNIT_TEST (testData);
    CPPUNIT_TEST (testIsSinglePrecision);
    CPPUNIT_TEST_SUITE_END ();

public:
    void setUp()
    {
        mValue = new FloatingPointValue(218.123, true);
    }

    void tearDown()
    {
        delete mValue;
    }

    void testData()
    {
        CPPUNIT_ASSERT (mValue->data() == 218.123);
    }

    void testIsSinglePrecision()
    {
        CPPUNIT_ASSERT (mValue->isSinglePrecision() == true);
    }

private:
    FloatingPointValue* mValue;
};

#endif
