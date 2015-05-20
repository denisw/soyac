/*
 * soyac - The Soya programming language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _PROPERTY_GET_ACCESSOR_TEST_HPP
#define _PROPERTY_GET_ACCESSOR_TEST_HPP

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <ast/FunctionParameter.hpp>
#include <ast/Property.hpp>
#include <ast/PropertyGetAccessor.hpp>
#include <ast/UnknownType.hpp>
#include "DummyType.hpp"

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
        CPPUNIT_ASSERT (dynamic_cast<UnknownType*>(mAcc->returnType()) != NULL);
    }

    void testReturnTypeWithParent()
    {
        Property prop("foo", new DummyType, mAcc, NULL);
        mAcc->ref();

        CPPUNIT_ASSERT (mAcc->returnType() == prop.type());
    }

private:
    PropertyGetAccessor* mAcc;
    Block* mBody;
};

#endif
