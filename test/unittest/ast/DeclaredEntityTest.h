/*
 * soyac - The Soya programming language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef SOYA_DECLARATION_TEST_H_
#define SOYA_DECLARATION_TEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "DummyDeclaredEntity.h"

using namespace soyac::ast;

class DeclaredEntityTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE (DeclaredEntityTest);
    CPPUNIT_TEST (testAddModifier);
    CPPUNIT_TEST (testHasModifier);
    CPPUNIT_TEST (testHasNoModifier);
    CPPUNIT_TEST_SUITE_END ();

public:
    void setUp()
    {
        mDecl = new DummyDeclaredEntity("soya");
    }

    void tearDown()
    {
        delete mDecl;
    }

    void testAddModifier()
    {
        CPPUNIT_ASSERT (mDecl->addModifier(DeclaredEntity::PUBLIC) == true);
        CPPUNIT_ASSERT (mDecl->addModifier(DeclaredEntity::PUBLIC) == false);
    }

    void testHasModifier()
    {
        mDecl->addModifier(DeclaredEntity::PUBLIC);
        CPPUNIT_ASSERT (mDecl->hasModifier(DeclaredEntity::PUBLIC) == true);
    }

    void testHasNoModifier()
    {
        CPPUNIT_ASSERT (mDecl->hasModifier(DeclaredEntity::PUBLIC) == false);
    }

private:
    DeclaredEntity* mDecl;
};

#endif
