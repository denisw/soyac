/*
 * soyac - The Soya programming language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef SOYA_BOOLEAN_TYPE_TEST_H_
#define SOYA_BOOLEAN_TYPE_TEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <ast/BooleanType.h>
#include "UnitTestUtil.h"

using namespace soyac::ast;

class BooleanTypeTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE (BooleanTypeTest);
    CPPUNIT_TEST (testVisit);
    CPPUNIT_TEST (testName);
    CPPUNIT_TEST (testGet);
    CPPUNIT_TEST_SUITE_END ();

    TEST_VISITOR (BooleanType);

public:
    void setUp()
    {
        mType = BooleanType::get();
    }

    void testVisit()
    {
        BooleanTypeTestVisitor v;
        CPPUNIT_ASSERT (((bool) mType->visit(&v)) == true);
    }

    void testName()
    {
        CPPUNIT_ASSERT (mType->name() == "bool");
    }

    void testGet()
    {
        CPPUNIT_ASSERT (BooleanType::get() == mType);
    }

private:
    BooleanType* mType;
};

#endif
