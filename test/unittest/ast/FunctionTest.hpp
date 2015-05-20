/*
 * soyac - The Soya programming language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _FUNCTION_TEST_HPP
#define _FUNCTION_TEST_HPP

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <ast/BooleanType.hpp>
#include <ast/Block.hpp>
#include <ast/Function.hpp>
#include <ast/FunctionParameter.hpp>
#include <ast/FunctionType.hpp>
#include <ast/IntegerType.hpp>
#include <ast/Statement.hpp>
#include "UnitTestUtil.hpp"

using namespace soyac::ast;

class FunctionTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE (FunctionTest);
    CPPUNIT_TEST (testVisit);
    CPPUNIT_TEST (testStr);
    CPPUNIT_TEST (testReturnType);
    CPPUNIT_TEST (testReturnTypeReplaceable);
    CPPUNIT_TEST (testParameters);
    CPPUNIT_TEST (testBody);
    CPPUNIT_TEST (testBodyReplaceable);
    CPPUNIT_TEST (testType);
    CPPUNIT_TEST_SUITE_END ();

    TEST_VISITOR (Function);

public:
    void setUp()
    {
        mRetType = new DummyType;
        mBody = new Block;

        mParams = new FunctionParameter*[3];
        mParams[0] = new FunctionParameter(Name("one"), TYPE_INT);
        mParams[1] = new FunctionParameter(Name("two"), TYPE_INT);
        mParams[2] = new FunctionParameter(Name("three"), TYPE_BOOL);

        mFunc = new Function(Name("func"), mRetType,
                             mParams, mParams + 3,
                             mBody);
    }

    void tearDown()
    {
        delete mFunc;
        delete [] mParams;
    }

    void testVisit()
    {
        FunctionTestVisitor v;
        CPPUNIT_ASSERT (((bool) mFunc->visit(&v)) == true);
    }

    void testStr()
    {
        CPPUNIT_ASSERT (mFunc->str() == "func(int, int, bool)");
    }

    void testReturnType()
    {
        CPPUNIT_ASSERT (mFunc->returnType() == mRetType);
    }

    void testReturnTypeReplaceable()
    {
        DummyType retType2;
        mRetType->replaceWith(&retType2);
        CPPUNIT_ASSERT (mFunc->returnType() == &retType2);
    }

    void testParameters()
    {
        Function::parameters_iterator it = mFunc->parameters_begin();

        CPPUNIT_ASSERT (*it == mParams[0]);
        it++;
        CPPUNIT_ASSERT (*it == mParams[1]);
        it++;
        CPPUNIT_ASSERT (*it == mParams[2]);
        it++;
        CPPUNIT_ASSERT (it == mFunc->parameters_end());
    }

    void testBody()
    {
        CPPUNIT_ASSERT (mFunc->body() == mBody);
    }

    void testBodyReplaceable()
    {
        Block* body2 = new Block;
        mBody->replaceWith(body2);
        CPPUNIT_ASSERT (mFunc->body() == body2);
    }

    void testType()
    {
        FunctionType* type = mFunc->type();
        CPPUNIT_ASSERT (type->returnType() == mFunc->returnType());

        FunctionType::parameterTypes_iterator it =
          type->parameterTypes_begin();

        CPPUNIT_ASSERT (*it == mParams[0]->type());
        it++;
        CPPUNIT_ASSERT (*it == mParams[1]->type());
        it++;
        CPPUNIT_ASSERT (*it == mParams[2]->type());
        it++;
        CPPUNIT_ASSERT (it == type->parameterTypes_end());
    }

private:
    Function* mFunc;
    Type* mRetType;
    FunctionParameter** mParams;
    Block* mBody;
};

#endif
