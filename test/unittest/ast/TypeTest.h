/*
 * soyac - The Soya programming language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef SOYA_TYPE_DECLARATION_TEST_H_
#define SOYA_TYPE_DECLARATION_TEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <ast/UnknownType.h>
#include "DummyType.h"

using namespace soyac::ast;

class TypeTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE (TypeTest);
    CPPUNIT_TEST (testConvertable);
    CPPUNIT_TEST (testNotConvertable);
    CPPUNIT_TEST (testImplicitlyConvertable);
    CPPUNIT_TEST (testNotImplicitlyConvertable);
    CPPUNIT_TEST (testImplicitlyConvertableToUnknown);
    CPPUNIT_TEST (testSubtypeOf);
    CPPUNIT_TEST_SUITE_END ();

public:
    void setUp()
    {
        mDecl = new DummyType(Name("Sophia"));
    }

    void tearDown()
    {
        delete mDecl;
    }

    void testConvertable()
    {
        CPPUNIT_ASSERT (mDecl->isConvertableTo(mDecl));
    }

    void testNotConvertable()
    {
        DummyType decl2;
        CPPUNIT_ASSERT (!mDecl->isConvertableTo(&decl2));
    }

    void testImplicitlyConvertable()
    {
        CPPUNIT_ASSERT (mDecl->isImplicitlyConvertableTo(mDecl));
    }

    void testNotImplicitlyConvertable()
    {
        DummyType decl2;
        CPPUNIT_ASSERT (!mDecl->isImplicitlyConvertableTo(&decl2));
    }

    void testImplicitlyConvertableToUnknown()
    {
        CPPUNIT_ASSERT (mDecl->isImplicitlyConvertableTo(TYPE_UNKNOWN));
    }

    void testSubtypeOf()
    {
        DummyType decl2;
        CPPUNIT_ASSERT (!mDecl->isSubtypeOf(&decl2));
    }

private:
    Type* mDecl;
};

#endif
