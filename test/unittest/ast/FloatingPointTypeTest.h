/*
 * soyac - The Soya programming language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef SOYA_FLOATING_POINT_TYPE_TEST_H_
#define SOYA_FLOATING_POINT_TYPE_TEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <ast/FloatingPointType.h>
#include <ast/IntegerType.h>
#include "UnitTestUtil.h"

using namespace soyac::ast;

class FloatingPointTypeTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE (FloatingPointTypeTest);
    CPPUNIT_TEST (testVisit);
    CPPUNIT_TEST (testGet);
    CPPUNIT_TEST (testNameOfFloat);
    CPPUNIT_TEST (testNameOfDouble);
    CPPUNIT_TEST (testConvertable);
    CPPUNIT_TEST (testNotConvertable);
    CPPUNIT_TEST (testImplicitlyConvertable);
    CPPUNIT_TEST (testNotImplicitlyConvertable);
    CPPUNIT_TEST_SUITE_END ();

    TEST_VISITOR (FloatingPointType);

public:
    void setUp()
    {
        mFloat = FloatingPointType::getFloat();
        mDouble = FloatingPointType::getDouble();
    }

    void tearDown()
    {
    }

    void testVisit()
    {
        FloatingPointTypeTestVisitor v;
        CPPUNIT_ASSERT (((bool) mFloat->visit(&v)) == true);
    }

    void testGet()
    {
        CPPUNIT_ASSERT (FloatingPointType::getFloat() == mFloat);
        CPPUNIT_ASSERT (FloatingPointType::getDouble() == mDouble);
    }

    void testNameOfFloat()
    {
        CPPUNIT_ASSERT (mFloat->name() == Name("float"));
    }

    void testNameOfDouble()
    {
        CPPUNIT_ASSERT (mDouble->name() == Name("double"));
    }

    void testConvertable()
    {
        CPPUNIT_ASSERT (mFloat->isConvertableTo(mDouble));
        CPPUNIT_ASSERT (mDouble->isConvertableTo(mFloat));
        CPPUNIT_ASSERT (mFloat->isConvertableTo(TYPE_INT));
        CPPUNIT_ASSERT (mDouble->isConvertableTo(TYPE_INT));
    }

    void testNotConvertable()
    {
        DummyType other;
        CPPUNIT_ASSERT (!mFloat->isConvertableTo(&other));
        CPPUNIT_ASSERT (!mDouble->isConvertableTo(&other));
    }

    void testImplicitlyConvertable()
    {
        CPPUNIT_ASSERT (mFloat->isImplicitlyConvertableTo(mDouble));
    }

    void testNotImplicitlyConvertable()
    {
        CPPUNIT_ASSERT (!mDouble->isImplicitlyConvertableTo(mFloat));
        CPPUNIT_ASSERT (!mFloat->isImplicitlyConvertableTo(TYPE_INT));
        CPPUNIT_ASSERT (!mDouble->isImplicitlyConvertableTo(TYPE_INT));
    }

private:
    FloatingPointType* mFloat;
    FloatingPointType* mDouble;
};

#endif
