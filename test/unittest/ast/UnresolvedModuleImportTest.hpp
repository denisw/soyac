/*
 * soyac - The Soya programming language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _UNRESOLVED_MODULE_IMPORT_TEST_HPP
#define _UNRESOLVED_MODULE_IMPORT_TEST_HPP

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <ast/UnresolvedModuleImport.hpp>
#include "UnitTestUtil.hpp"

using namespace soyac::ast;

class UnresolvedModuleImportTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE (UnresolvedModuleImportTest);
    CPPUNIT_TEST (testVisit);
    CPPUNIT_TEST (testImportedName);
    CPPUNIT_TEST (testIsDirect);
    CPPUNIT_TEST_SUITE_END ();

    TEST_VISITOR (UnresolvedModuleImport);

public:
    void setUp()
    {
        mImp = new UnresolvedModuleImport("foo::bar.Baz", true);
    }

    void tearDown()
    {
        delete mImp;
    }

    void testVisit()
    {
        UnresolvedModuleImportTestVisitor v;
        CPPUNIT_ASSERT (((bool) mImp->visit(&v)) == true);
    }

    void testImportedName()
    {
        CPPUNIT_ASSERT (mImp->importedName() == "foo::bar.Baz");
    }

    void testIsDirect()
    {
        CPPUNIT_ASSERT (mImp->isDirect() == true);
    }

private:
    UnresolvedModuleImport* mImp;
};

#endif
