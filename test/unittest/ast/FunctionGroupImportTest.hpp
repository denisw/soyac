/*
 * soyac - The Soya programming language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _FUNCTION_GROUP_IMPORT_TEST_HPP
#define _FUNCTION_GROUP_IMPORT_TEST_HPP

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <ast/Function.hpp>
#include <ast/IntegerType.hpp>
#include <ast/BooleanType.hpp>
#include <ast/FunctionGroupImport.hpp>
#include <ast/VoidType.hpp>
#include "UnitTestUtil.hpp"

using namespace soyac::ast;

class FunctionGroupImportTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE (FunctionGroupImportTest);
    CPPUNIT_TEST (testVisit);
    CPPUNIT_TEST (testOverloads);
    CPPUNIT_TEST_SUITE_END ();

    TEST_VISITOR (FunctionGroupImport);

public:
    void setUp()
    {
        FunctionParameter* intParam = new FunctionParameter("i", TYPE_INT);
        FunctionParameter* boolParam = new FunctionParameter("b", TYPE_BOOL);

        mOverloads = new Function*[3];
        mOverloads[0] = new Function("foo", TYPE_VOID);
        mOverloads[1] = new Function("foo", TYPE_VOID,
                                     &intParam, &intParam + 1);
        mOverloads[2] = new Function("foo", TYPE_VOID,
                                     &boolParam, &boolParam + 1);

        mImp = new FunctionGroupImport(mOverloads, mOverloads + 3);
    }

    void tearDown()
    {
        delete mImp;
        delete [] mOverloads;
    }

    void testVisit()
    {
        FunctionGroupImportTestVisitor v;
        CPPUNIT_ASSERT (((bool) mImp->visit(&v)) == true);
    }

    void testOverloads()
    {
        FunctionGroupImport::overloads_iterator it = mImp->overloads_begin();

        CPPUNIT_ASSERT (*it == mOverloads[0]);
        it++;
        CPPUNIT_ASSERT (*it == mOverloads[1]);
        it++;
        CPPUNIT_ASSERT (*it == mOverloads[2]);
        it++;
        CPPUNIT_ASSERT (it == mImp->overloads_end());
    }

private:
    FunctionGroupImport* mImp;
    Function** mOverloads;
};

#endif
