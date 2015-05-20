/*
 * soyac - The Soya programming language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _UNKNOWN_ARRAY_TYPE_TEST_HPP
#define _UNKNOWN_ARRAY_TYPE_TEST_HPP

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <list>
#include <string>
#include <ast/UnknownArrayType.hpp>
#include "DummyType.hpp"

using namespace soyac::ast;

class UnknownArrayTypeTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE (UnknownArrayTypeTest);
    CPPUNIT_TEST (testVisit);
    CPPUNIT_TEST (testElementType);
    CPPUNIT_TEST (testElementTypeReplaceable);
    CPPUNIT_TEST_SUITE_END ();

    TEST_VISITOR (UnknownArrayType);

public:
    void setUp()
    {
        mElementType = new UnknownType("Foo");
        mType = new UnknownArrayType(mElementType);
    }

    void tearDown()
    {
        delete mType;
    }

    void testVisit()
    {
        UnknownArrayTypeTestVisitor v;
        CPPUNIT_ASSERT (((bool) mType->visit(&v)) == true);
    }

    void testElementType()
    {
        CPPUNIT_ASSERT (mType->elementType() == mElementType);
    }

    void testElementTypeReplaceable()
    {
        UnknownType elementType2("Bar");
        mElementType->replaceWith(&elementType2);
        CPPUNIT_ASSERT (mType->elementType() == &elementType2);
    }

private:
    UnknownArrayType* mType;
    UnknownType* mElementType;
};

#endif
