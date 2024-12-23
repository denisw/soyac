/*
 * soyac - The Soya programming language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef SOYA_FUNCTION_PARAMETER_TEST_H_
#define SOYA_FUNCTION_PARAMETER_TEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <ast/FunctionParameter.h>
#include "DummyType.h"

using namespace soyac::ast;

class FunctionParameterTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE (FunctionParameterTest);
    CPPUNIT_TEST (testVisit);
    CPPUNIT_TEST (testType);
    CPPUNIT_TEST (testTypeReplaceable);
    CPPUNIT_TEST_SUITE_END ();

    TEST_VISITOR (FunctionParameter);

public:
    void setUp()
    {
        mType = new DummyType;
        mParam = new FunctionParameter(Name("foo"), mType);
    }

    void tearDown()
    {
        delete mParam;
    }

    void testVisit()
    {
        FunctionParameterTestVisitor v;
        CPPUNIT_ASSERT (((bool) mParam->visit(&v)) == true);
    }

    void testType()
    {
        CPPUNIT_ASSERT (mParam->type() == mType);
    }

    void testTypeReplaceable()
    {
        DummyType type2;
        mType->replaceWith(&type2);
        CPPUNIT_ASSERT (mParam->type() == &type2);
    }

private:
    FunctionParameter* mParam;
    Type* mType;
};

#endif
