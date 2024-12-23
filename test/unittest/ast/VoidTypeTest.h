/*
 * soyac - The Soya programming language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef SOYA_VOID_TYPE_TEST_H_
#define SOYA_VOID_TYPE_TEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <ast/VoidType.h>
#include "UnitTestUtil.h"

using namespace soyac::ast;

class VoidTypeTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE (VoidTypeTest);
    CPPUNIT_TEST (testVisit);
    CPPUNIT_TEST (testName);
    CPPUNIT_TEST (testGet);
    CPPUNIT_TEST_SUITE_END ();

    TEST_VISITOR (VoidType);

public:
    void setUp()
    {
        mType = VoidType::get();
    }

    void testVisit()
    {
        VoidTypeTestVisitor v;
        CPPUNIT_ASSERT (((bool) mType->visit(&v)) == true);
    }

    void testName()
    {
        CPPUNIT_ASSERT (mType->name() == "void");
    }

    void testGet()
    {
        CPPUNIT_ASSERT (VoidType::get() == mType);
    }

private:
    VoidType* mType;
};

#endif
