/*
 * soyac - The Soya programming language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef SOYA_CONSTRUCTOR_TEST_H_
#define SOYA_CONSTRUCTOR_TEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <ast/Block.h>
#include <ast/Constructor.h>
#include <ast/UnresolvedDefaultConstructorInitializer.h>
#include "UnitTestUtil.h"

using namespace soyac::ast;

class ConstructorTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE (ConstructorTest);
    CPPUNIT_TEST (testVisit);
    CPPUNIT_TEST (testInitializer);
    CPPUNIT_TEST_SUITE_END ();

    TEST_VISITOR (Constructor);

public:
    void setUp()
    {
        mInit = new UnresolvedDefaultConstructorInitializer;
        mCons = new Constructor(mInit, new Block);
    }

    void tearDown()
    {
        delete mCons;
    }

    void testVisit()
    {
        ConstructorTestVisitor v;
        CPPUNIT_ASSERT (((bool) mCons->visit(&v)) == true);
    }

    void testInitializer()
    {
        CPPUNIT_ASSERT (mCons->initializer() == mInit);
    }

private:
    Constructor* mCons;
    ConstructorInitializer* mInit;
};

#endif
