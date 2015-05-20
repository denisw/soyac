/*
 * soyac - The Soya programming language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _ENUM_TYPE_TEST_HPP
#define _ENUM_TYPE_TEST_HPP

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <ast/BooleanType.hpp>
#include <ast/EnumType.hpp>
#include <ast/Function.hpp>
#include <ast/IntegerType.hpp>
#include "DummyType.hpp"
#include "UnitTestUtil.hpp"

using namespace soyac::ast;

class EnumTypeTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE (EnumTypeTest);
    CPPUNIT_TEST (testVisit);
    CPPUNIT_TEST (testUnderlyingType);
    CPPUNIT_TEST (testValues);
    CPPUNIT_TEST (testEqualsMethod);
    CPPUNIT_TEST_SUITE_END ();

    TEST_VISITOR (EnumType);

public:
    void setUp()
    {
        mValues = new EnumConstant*[3];
        mValues[0] = new EnumConstant("Foo", IntegerValue(0, false));
        mValues[1] = new EnumConstant("Bar", IntegerValue(1, false));
        mValues[2] = new EnumConstant("Baz", IntegerValue(2, false));

        mType = new EnumType("Foo", TYPE_INT, mValues, mValues + 3);
    }

    void tearDown()
    {
        delete mType;
        delete [] mValues;
    }

    void testVisit()
    {
        EnumTypeTestVisitor v;
        CPPUNIT_ASSERT (((bool) mType->visit(&v)) == true);
    }

    void testUnderlyingType()
    {
        CPPUNIT_ASSERT (mType->underlyingType() == TYPE_INT);
    }

    void testValues()
    {
        EnumType::values_iterator it = mType->values_begin();

        CPPUNIT_ASSERT (*it == mValues[0]);
        it++;
        CPPUNIT_ASSERT (*it == mValues[1]);
        it++;
        CPPUNIT_ASSERT (*it == mValues[2]);
        it++;
        CPPUNIT_ASSERT (it == mType->values_end());
    }

    void testEqualsMethod()
    {
        Function* method = mType->equalsMethod();

        CPPUNIT_ASSERT (method->name() == "equals");
        CPPUNIT_ASSERT (method->returnType() == TYPE_BOOL);

        Function::parameters_iterator it = method->parameters_begin();

        CPPUNIT_ASSERT ((*it)->type() == mType);
        it++;
        CPPUNIT_ASSERT (it == method->parameters_end());
    }

private:
    EnumType* mType;
    EnumConstant** mValues;
};

#endif
