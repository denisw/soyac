/*
 * soyac - The Soya programming language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef SOYA_CHARACTER_LITERAL_TEST_H_
#define SOYA_CHARACTER_LITERAL_TEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <ast/CharacterLiteral.h>
#include <ast/CharacterType.h>
#include "UnitTestUtil.h"

using namespace soyac::ast;

class CharacterLiteralTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE (CharacterLiteralTest);
    CPPUNIT_TEST (testVisit);
    CPPUNIT_TEST (testType);
    CPPUNIT_TEST (testValue);
    CPPUNIT_TEST_SUITE_END ();

    TEST_VISITOR (CharacterLiteral);

public:
    void setUp()
    {
        mLiteral = new CharacterLiteral((uint32_t) 'v');
    }

    void tearDown()
    {
        delete mLiteral;
    }

    void testVisit()
    {
        CharacterLiteralTestVisitor v;
        CPPUNIT_ASSERT (((bool) mLiteral->visit(&v)) == true);
    }

    void testType()
    {
        CPPUNIT_ASSERT (mLiteral->type() == TYPE_CHAR);
    }

    void testValue()
    {
        CPPUNIT_ASSERT (mLiteral->value() == 'v');
    }

private:
    CharacterLiteral* mLiteral;
};

#endif
