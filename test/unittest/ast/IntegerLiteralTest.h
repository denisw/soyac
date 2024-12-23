/*
 * soyac - The Soya programming language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef SOYA_INTEGER_LITERAL_TEST_H_
#define SOYA_INTEGER_LITERAL_TEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <ast/IntegerLiteral.h>
#include <ast/IntegerType.h>
#include "UnitTestUtil.h"

using namespace soyac::ast;

class IntegerLiteralTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE (IntegerLiteralTest);
    CPPUNIT_TEST (testVisit);
    CPPUNIT_TEST (testType);
    CPPUNIT_TEST (testValue);
    CPPUNIT_TEST_SUITE_END ();

    TEST_VISITOR (IntegerLiteral);

public:
    void setUp()
    {
        IntegerValue val(104, false);
        mLiteral = new IntegerLiteral(val);
    }

    void tearDown()
    {
        delete mLiteral;
    }

    void testVisit()
    {
        IntegerLiteralTestVisitor v;
        CPPUNIT_ASSERT (((bool) mLiteral->visit(&v)) == true);
    }

    void testType()
    {
        CPPUNIT_ASSERT (mLiteral->type() == TYPE_INT);
    }

    void testValue()
    {
        CPPUNIT_ASSERT (mLiteral->value() == IntegerValue(104, false));
    }

private:
    IntegerLiteral* mLiteral;
};

#endif
