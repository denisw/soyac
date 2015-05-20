/*
 * soyac - The Soya programming language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _ENUM_CONSTANT_TEST_HPP
#define _ENUM_CONSTANT_TEST_HPP

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <ast/EnumConstant.hpp>
#include <ast/IntegerValue.hpp>
#include "UnitTestUtil.hpp"

using namespace soyac::ast;

class EnumConstantTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE (EnumConstantTest);
    CPPUNIT_TEST (testVisit);
    CPPUNIT_TEST (testValue);
    CPPUNIT_TEST_SUITE_END ();

    TEST_VISITOR (EnumConstant);

public:
    void setUp()
    {
        IntegerValue val(218, false);
        mEcons = new EnumConstant("foo", val);
    }

    void tearDown()
    {
        delete mEcons;
    }

    void testVisit()
    {
        EnumConstantTestVisitor v;
        CPPUNIT_ASSERT (((bool) mEcons->visit(&v)) == true);
    }

    void testValue()
    {
        CPPUNIT_ASSERT (mEcons->value() == IntegerValue(218, false));
    }

private:
    EnumConstant* mEcons;
};

#endif
