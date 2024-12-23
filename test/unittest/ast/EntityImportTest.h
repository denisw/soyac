/*
 * soyac - The Soya programming language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef SOYA_ENTITY_IMPORT_TEST_H_
#define SOYA_ENTITY_IMPORT_TEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <ast/EntityImport.h>
#include "DummyDeclaredEntity.h"
#include "UnitTestUtil.h"

using namespace soyac::ast;

class EntityImportTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE (EntityImportTest);
    CPPUNIT_TEST (testVisit);
    CPPUNIT_TEST (testImported);
    CPPUNIT_TEST (testImportedReplaceable);
    CPPUNIT_TEST_SUITE_END ();

    TEST_VISITOR (EntityImport);

public:
    void setUp()
    {
        mImported = new DummyDeclaredEntity;
        mImp = new EntityImport(mImported);
    }

    void tearDown()
    {
        delete mImp;
    }

    void testVisit()
    {
        EntityImportTestVisitor v;
        CPPUNIT_ASSERT (((bool) mImp->visit(&v)) == true);
    }

    void testImported()
    {
        CPPUNIT_ASSERT (mImp->imported() == mImported);
    }

    void testImportedReplaceable()
    {
        DummyDeclaredEntity imported2;
        mImported->replaceWith(&imported2);
        CPPUNIT_ASSERT (mImp->imported() == &imported2);
    }

private:
    EntityImport* mImp;
    DeclaredEntity* mImported;
};

#endif
