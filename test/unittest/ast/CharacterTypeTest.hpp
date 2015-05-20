/*
 * soyac - The Soya programming language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _CHARACTER_TYPE_TEST_HPP
#define _CHARACTER_TYPE_TEST_HPP

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <ast/CharacterType.hpp>
#include "UnitTestUtil.hpp"

using namespace soyac::ast;

class CharacterTypeTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE (CharacterTypeTest);
    CPPUNIT_TEST (testVisit);
    CPPUNIT_TEST (testName);
    CPPUNIT_TEST (testGet);
    CPPUNIT_TEST_SUITE_END ();

    TEST_VISITOR (CharacterType);

public:
    void setUp()
    {
        mType = CharacterType::get();
    }

    void testVisit()
    {
        CharacterTypeTestVisitor v;
        CPPUNIT_ASSERT (((bool) mType->visit(&v)) == true);
    }

    void testName()
    {
        CPPUNIT_ASSERT (mType->name() == Name("char"));
    }

    void testGet()
    {
        CPPUNIT_ASSERT (CharacterType::get() == mType);
    }

private:
    CharacterType* mType;
};

#endif
