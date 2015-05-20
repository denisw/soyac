/*
 * soyac - The Soya programming language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _UNKNOWN_FUNCTION_TYPE_TEST_HPP
#define _UNKNOWN_FUNCTION_TYPE_TEST_HPP

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <stdint.h>
#include <ast/UnknownFunctionType.hpp>
#include "DummyType.hpp"
#include "UnitTestUtil.hpp"

using namespace soyac::ast;

class UnknownFunctionTypeTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE (UnknownFunctionTypeTest);
    CPPUNIT_TEST (testVisit);
    CPPUNIT_TEST (testReturnType);
    CPPUNIT_TEST (testReturnTypeReplaceable);
    CPPUNIT_TEST (testParameterTypes);
    CPPUNIT_TEST_SUITE_END ();

    TEST_VISITOR (UnknownFunctionType);

public:
    void setUp()
    {
        mReturnType = new DummyType;

        mParamTypes = new Type*[3];
        mParamTypes[0] = new DummyType;
        mParamTypes[1] = new DummyType;
        mParamTypes[2] = new DummyType;
        
        mType =
          new UnknownFunctionType(mReturnType, mParamTypes, mParamTypes + 3);
    }

    void tearDown()
    {
        delete mParamTypes;
    }

    void testVisit()
    {
        UnknownFunctionTypeTestVisitor v;
        CPPUNIT_ASSERT (((bool) mType->visit(&v)) == true);
    }

    void testReturnType()
    {
        CPPUNIT_ASSERT (mType->returnType() == mReturnType);
    }

    void testReturnTypeReplaceable()
    {
        DummyType retType2;
        mReturnType->replaceWith(&retType2);
        CPPUNIT_ASSERT (mType->returnType() == &retType2);
    }

    void testParameterTypes()
    {
        UnknownFunctionType::parameterTypes_iterator it =
          mType->parameterTypes_begin();

        CPPUNIT_ASSERT (*it == mParamTypes[0]);
        it++;
        CPPUNIT_ASSERT (*it == mParamTypes[1]);
        it++;
        CPPUNIT_ASSERT (*it == mParamTypes[2]);
        it++;
        CPPUNIT_ASSERT (it == mType->parameterTypes_end());
    }


private:
    UnknownFunctionType* mType;
    Type* mReturnType;
    Type** mParamTypes;
};

#endif
