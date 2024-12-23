/*
 * soyac - The Soya programming language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef SOYA_UNRESOLVED_ENTITY_IMPORT_TEST_H_
#define SOYA_UNRESOLVED_ENTITY_IMPORT_TEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <ast/UnresolvedEntityImport.h>
#include "UnitTestUtil.h"

using namespace soyac::ast;

class UnresolvedEntityImportTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE (UnresolvedEntityImportTest);
    CPPUNIT_TEST (testVisit);
    CPPUNIT_TEST (testImportedName);
    CPPUNIT_TEST_SUITE_END ();

    TEST_VISITOR (UnresolvedEntityImport);

public:
    void setUp()
    {
        mImp = new UnresolvedEntityImport("foo::bar.Baz");
    }

    void tearDown()
    {
        delete mImp;
    }

    void testVisit()
    {
        UnresolvedEntityImportTestVisitor v;
        CPPUNIT_ASSERT (((bool) mImp->visit(&v)) == true);
    }

    void testImportedName()
    {
        CPPUNIT_ASSERT (mImp->importedName() == "foo::bar.Baz");
    }

private:
    UnresolvedEntityImport* mImp;
};

#endif
