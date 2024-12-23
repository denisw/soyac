/*
 * soyac - The Soya programming language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef SOYA_BOOLEAN_LITERAL_TEST_H_
#define SOYA_BOOLEAN_LITERAL_TEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <ast/BooleanLiteral.h>
#include <ast/BooleanType.h>
#include "UnitTestUtil.h"

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
