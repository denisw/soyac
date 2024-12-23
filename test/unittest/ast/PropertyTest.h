/*
 * soyac - The Soya programming language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef SOYA_PROPERTY_TEST_H_
#define SOYA_PROPERTY_TEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <ast/Property.h>
#include <ast/PropertyGetAccessor.h>
#include <ast/PropertySetAccessor.h>
#include "DummyType.h"

using namespace soyac::ast;

class PropertyTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE (PropertyTest);
    CPPUNIT_TEST (testVisit);
    CPPUNIT_TEST (testType);
    CPPUNIT_TEST (testTypeReplaceable);
    CPPUNIT_TEST (testGetAccessor);
    CPPUNIT_TEST (testGetAccessorReplaceable);
    CPPUNIT_TEST (testGetAccessorParent);
    CPPUNIT_TEST (testSetAccessor);
    CPPUNIT_TEST (testSetAccessorReplaceable);
    CPPUNIT_TEST (testSetAccessorParent);
    CPPUNIT_TEST_SUITE_END ();

    TEST_VISITOR (Property);

public:
    void setUp()
    {
        mType = new DummyType;
        mGetAccessor = new PropertyGetAccessor(nullptr);
        mSetAccessor = new PropertySetAccessor(nullptr);

        mProp = new Property("foo", mType,
                             mGetAccessor,
                             mSetAccessor);
    }

    void tearDown()
    {
        delete mProp;
    }

    void testVisit()
    {
        PropertyTestVisitor v;
        CPPUNIT_ASSERT (((bool) mProp->visit(&v)) == true);
    }

    void testType()
    {
        CPPUNIT_ASSERT (mProp->type() == mType);
    }

    void testTypeReplaceable()
    {
        DummyType type2;
        mType->replaceWith(&type2);
        CPPUNIT_ASSERT (mProp->type() == &type2);
    }

    void testGetAccessor()
    {
        CPPUNIT_ASSERT (mProp->getAccessor() == mGetAccessor);
    }

    void testGetAccessorReplaceable()
    {
        PropertyGetAccessor getAccessor2(nullptr);
        mGetAccessor->replaceWith(&getAccessor2);
        CPPUNIT_ASSERT (mProp->getAccessor() == &getAccessor2);
    }

    void testGetAccessorParent()
    {
        CPPUNIT_ASSERT (mGetAccessor->parent() == mProp);
    }

    void testSetAccessor()
    {
        CPPUNIT_ASSERT (mProp->setAccessor() == mSetAccessor);
    }

    void testSetAccessorReplaceable()
    {
        PropertySetAccessor setAccessor2(nullptr);
        mSetAccessor->replaceWith(&setAccessor2);
        CPPUNIT_ASSERT (mProp->setAccessor() == &setAccessor2);
    }

    void testSetAccessorParent()
    {
        CPPUNIT_ASSERT (mSetAccessor->parent() == mProp);
    }

private:
    Property* mProp;
    Type* mType;
    PropertyGetAccessor* mGetAccessor;
    PropertySetAccessor* mSetAccessor;
};

#endif
