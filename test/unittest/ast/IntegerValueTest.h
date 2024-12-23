/*
 * soyac - The Soya programming language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef SOYA_INTEGER_VALUE_TEST_H_
#define SOYA_INTEGER_VALUE_TEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <ast/IntegerValue.h>

using namespace soyac::ast;

class IntegerValueTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE (IntegerValueTest);
    CPPUNIT_TEST (testData);
    CPPUNIT_TEST (testIsSigned);
    CPPUNIT_TEST (testIsSignedNormalized);
    CPPUNIT_TEST (testEqual);
    CPPUNIT_TEST (testUnequal);
    CPPUNIT_TEST (testLessThan);
    CPPUNIT_TEST (testGreaterThan);
    CPPUNIT_TEST (testEqualOrLessThan);
    CPPUNIT_TEST (testEqualOrGreaterThan);
    CPPUNIT_TEST_SUITE_END ();

public:
    void setUp()
    {
        mValue = new IntegerValue(-104, true);
    }

    void tearDown()
    {
        delete mValue;
    }

    void testData()
    {
        CPPUNIT_ASSERT (mValue->data() == -104);
    }

    void testIsSigned()
    {
        CPPUNIT_ASSERT (mValue->isSigned() == true);
    }

    void testIsSignedNormalized()
    {
        IntegerValue val(104, true);
        CPPUNIT_ASSERT (val.isSigned() == false);
    }

    void testEqual()
    {
        CPPUNIT_ASSERT (*mValue == IntegerValue(-104, true));
    }

    void testUnequal()
    {
        CPPUNIT_ASSERT (*mValue != IntegerValue(302, true));
        CPPUNIT_ASSERT (*mValue != IntegerValue(-104, false));
    }

    void testLessThan()
    {
        CPPUNIT_ASSERT (*mValue < IntegerValue(-103, true));
        CPPUNIT_ASSERT (*mValue < IntegerValue(302, false));
        CPPUNIT_ASSERT (*mValue < IntegerValue(-316, false));
    }

    void testGreaterThan()
    {
        CPPUNIT_ASSERT (*mValue > IntegerValue(-112, true));
        CPPUNIT_ASSERT (IntegerValue(302, false) > *mValue);
        CPPUNIT_ASSERT (IntegerValue(-316, false) > *mValue);
    }

    void testEqualOrLessThan()
    {
        CPPUNIT_ASSERT (*mValue <= IntegerValue(-104, true));
        CPPUNIT_ASSERT (*mValue <= IntegerValue(-103, true));
        CPPUNIT_ASSERT (*mValue <= IntegerValue(302, false));
        CPPUNIT_ASSERT (*mValue <= IntegerValue(-316, false));
    }

    void testEqualOrGreaterThan()
    {
        CPPUNIT_ASSERT (*mValue >= IntegerValue(-104, true));
        CPPUNIT_ASSERT (*mValue >= IntegerValue(-112, true));
        CPPUNIT_ASSERT (IntegerValue(302, false) >= *mValue);
        CPPUNIT_ASSERT (IntegerValue(-316, false) >= *mValue);
    }

private:
    IntegerValue* mValue;
};

#endif
