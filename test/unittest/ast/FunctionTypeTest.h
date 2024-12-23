/*
 * soyac - The Soya programming language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef SOYA_FUNCTION_TYPE_TEST_H_
#define SOYA_FUNCTION_TYPE_TEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <stdint.h>
#include <ast/FunctionType.h>
#include "DummyType.h"
#include "UnitTestUtil.h"

using namespace soyac::ast;

class FunctionTypeTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE (FunctionTypeTest);
    CPPUNIT_TEST (testGet);
    CPPUNIT_TEST (testVisit);
    CPPUNIT_TEST (testStr);
    CPPUNIT_TEST (testReturnType);
    CPPUNIT_TEST (testReturnTypeReplaceable);
    CPPUNIT_TEST (testParameterTypes);
    CPPUNIT_TEST_SUITE_END ();

    TEST_VISITOR (FunctionType);

public:
    void setUp()
    {
        mReturnType = new DummyType("blubb");

        mParamTypes = new Type*[3];
        mParamTypes[0] = new DummyType("foo");
        mParamTypes[1] = new DummyType("bar");
        mParamTypes[2] = new DummyType("baz");
        
        mType = FunctionType::get(mReturnType, mParamTypes, mParamTypes + 3);
    }

    void tearDown()
    {
        delete mParamTypes;
    }

    void testGet()
    {
        DummyType returnType2;

        Type* paramTypes1[] = {mParamTypes[0], mParamTypes[1], mParamTypes[2]};
        Type* paramTypes2[] = {mParamTypes[1], mParamTypes[2], mParamTypes[0]};
        Type* paramTypes3[] = {new DummyType,};

        FunctionType* type2 =
          FunctionType::get(mReturnType, paramTypes1, paramTypes1 + 3);
        FunctionType* type3 =
          FunctionType::get(&returnType2, paramTypes1, paramTypes1 + 3);
        FunctionType* type4 =
          FunctionType::get(mReturnType, paramTypes2, paramTypes2 + 3);
        FunctionType* type5 =
          FunctionType::get(mReturnType, paramTypes3, paramTypes3 + 1);

        CPPUNIT_ASSERT (type2 == mType);
        CPPUNIT_ASSERT (type3 != mType);
        CPPUNIT_ASSERT (type4 != mType);
        CPPUNIT_ASSERT (type5 != mType);
    }

    void testVisit()
    {
        FunctionTypeTestVisitor v;
        CPPUNIT_ASSERT (((bool) mType->visit(&v)) == true);
    }

    void testStr()
    {
        CPPUNIT_ASSERT (mType->str() == "function(foo, bar, baz) => blubb");
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
        FunctionType::parameterTypes_iterator it =
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
    FunctionType* mType;
    Type* mReturnType;
    Type** mParamTypes;
};

#endif
