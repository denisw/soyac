/*
 * soyac - The Soya programming language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _FUNCTION_PARAMETER_TEST_HPP
#define _FUNCTION_PARAMETER_TEST_HPP

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <ast/FunctionParameter.hpp>
#include "DummyType.hpp"

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
