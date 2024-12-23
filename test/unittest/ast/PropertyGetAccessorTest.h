/*
 * soyac - The Soya programming language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef SOYA_PROPERTY_GET_ACCESSOR_TEST_H_
#define SOYA_PROPERTY_GET_ACCESSOR_TEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <ast/FunctionParameter.h>
#include <ast/Property.h>
#include <ast/PropertyGetAccessor.h>
#include <ast/UnknownType.h>
#include "DummyType.h"

using namespace soyac::ast;

class PropertyGetAccessorTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE (PropertyGetAccessorTest);
    CPPUNIT_TEST (testVisit);
    CPPUNIT_TEST (testReturnType);
    CPPUNIT_TEST (testReturnTypeWithParent);
    CPPUNIT_TEST_SUITE_END ();

    TEST_VISITOR (PropertyGetAccessor);

public:
    void setUp()
    {
        mBody = new Block;
        mAcc = new PropertyGetAccessor(mBody);
    }

    void tearDown()
    {
        delete mAcc;
    }

    void testVisit()
    {
        PropertyGetAccessorTestVisitor v;
        CPPUNIT_ASSERT (((bool) mAcc->visit(&v)) == true);
    }

    void testReturnType()
    {
        CPPUNIT_ASSERT (dynamic_cast<UnknownType*>(mAcc->returnType()) != nullptr);
    }

    void testReturnTypeWithParent()
    {
        Property prop("foo", new DummyType, mAcc, nullptr);
        mAcc->ref();

        CPPUNIT_ASSERT (mAcc->returnType() == prop.type());
    }

private:
    PropertyGetAccessor* mAcc;
    Block* mBody;
};

#endif
