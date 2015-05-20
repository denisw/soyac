/*
 * soyac - The Soya programming language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _PROPERTY_SET_ACCESSOR_TEST_HPP
#define _PROPERTY_SET_ACCESSOR_TEST_HPP

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <ast/FunctionParameter.hpp>
#include <ast/Property.hpp>
#include <ast/PropertySetAccessor.hpp>
#include <ast/UnknownType.hpp>
#include "DummyType.hpp"

using namespace soyac::ast;

class PropertySetAccessorTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE (PropertySetAccessorTest);
    CPPUNIT_TEST (testVisit);
    CPPUNIT_TEST (testValueParameter);
    CPPUNIT_TEST (testValueParameterWithParent);
    CPPUNIT_TEST_SUITE_END ();

    TEST_VISITOR (PropertySetAccessor);

public:
    void setUp()
    {
        mBody = new Block;
        mAcc = new PropertySetAccessor(mBody);
    }

    void tearDown()
    {
        delete mAcc;
    }

    void testVisit()
    {
        PropertySetAccessorTestVisitor v;
        CPPUNIT_ASSERT (((bool) mAcc->visit(&v)) == true);
    }

    void testValueParameter()
    {
        FunctionParameter* param = mAcc->valueParameter();
        CPPUNIT_ASSERT (param->name() == "value");
        CPPUNIT_ASSERT (dynamic_cast<UnknownType*>(param->type()) != NULL);
    }

    void testValueParameterWithParent()
    {
        Property prop("foo", new DummyType, NULL, mAcc);
        mAcc->ref();

        FunctionParameter* param = mAcc->valueParameter();
        CPPUNIT_ASSERT (param->name() == "value");
        CPPUNIT_ASSERT (param->type() == prop.type());
    }

private:
    PropertySetAccessor* mAcc;
    Block* mBody;
};

#endif
