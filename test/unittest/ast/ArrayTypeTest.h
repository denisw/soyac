/*
 * soyac - The Soya programming language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef SOYA_ARRAY_TYPE_TEST_H_
#define SOYA_ARRAY_TYPE_TEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <ast/ArrayType.h>
#include <ast/Function.h>
#include <ast/IntegerType.h>
#include <ast/Property.h>
#include <ast/VoidType.h>
#include "DummyType.h"
#include "UnitTestUtil.h"

using namespace soyac::ast;

class ArrayTypeTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE (ArrayTypeTest);
    CPPUNIT_TEST (testVisit);
    CPPUNIT_TEST (testGet);
    CPPUNIT_TEST (testName);
    CPPUNIT_TEST (testElementType);
    CPPUNIT_TEST (testGetElementMethod);
    CPPUNIT_TEST (testSetElementMethod);
    CPPUNIT_TEST (testLengthProperty);
    CPPUNIT_TEST_SUITE_END ();

    TEST_VISITOR (ArrayType);

public:
    void setUp()
    {
        mElementType = new DummyType("Foo");
        mType = ArrayType::get(mElementType);
    }

    void tearDown()
    {
    }

    void testVisit()
    {
        ArrayTypeTestVisitor v;
        CPPUNIT_ASSERT (((bool) mType->visit(&v)) == true);
    }

    void testGet()
    {
        CPPUNIT_ASSERT (ArrayType::get(mElementType) == mType);

        DummyType elementType2;
        CPPUNIT_ASSERT (ArrayType::get(new DummyType) != mType);
    }

    void testName()
    {
        CPPUNIT_ASSERT (mType->name() == "Foo[]");
    }

    void testElementType()
    {
        CPPUNIT_ASSERT (mType->elementType() == mElementType);
    }

    void testGetElementMethod()
    {
        Function* method = mType->getElementMethod();

        CPPUNIT_ASSERT (method->name() == "getElement");
        CPPUNIT_ASSERT (method->returnType() == mType->elementType());

        Function::parameters_iterator it = method->parameters_begin();

        CPPUNIT_ASSERT ((*it)->type() == TYPE_LONG);
        it++;
        CPPUNIT_ASSERT (it == method->parameters_end());
    }

    void testSetElementMethod()
    {
        Function* method = mType->setElementMethod();

        CPPUNIT_ASSERT (method->name() == "setElement");
        CPPUNIT_ASSERT (method->returnType() == TYPE_VOID);

        Function::parameters_iterator it = method->parameters_begin();

        CPPUNIT_ASSERT ((*it)->type() == TYPE_LONG);
        it++;
        CPPUNIT_ASSERT ((*it)->type() == mType->elementType());
        it++;
        CPPUNIT_ASSERT (it == method->parameters_end());
    }

    void testLengthProperty()
    {
        Property* prop = mType->lengthProperty();

        CPPUNIT_ASSERT (prop->name() == "length");
        CPPUNIT_ASSERT (prop->type() == TYPE_LONG);
        CPPUNIT_ASSERT (prop->getAccessor() != nullptr);
        CPPUNIT_ASSERT (prop->setAccessor() == nullptr);
    }

private:
    ArrayType* mType;
    Type* mElementType;
};

#endif
