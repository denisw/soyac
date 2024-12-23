/*
 * soyac - The Soya programming language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef SOYA_MODULE_IMPORT_TEST_H_
#define SOYA_MODULE_IMPORT_TEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <ast/Module.h>
#include <ast/ModuleImport.h>
#include "UnitTestUtil.h"

using namespace soyac::ast;

class ModuleImportTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE (ModuleImportTest);
    CPPUNIT_TEST (testVisit);
    CPPUNIT_TEST (testImported);
    CPPUNIT_TEST (testIsDirect);
    CPPUNIT_TEST_SUITE_END ();

    TEST_VISITOR (ModuleImport);

public:
    void setUp()
    {
        mImported = Module::get("foo::bar");
        mImp = new ModuleImport(mImported, true);
    }

    void tearDown()
    {
        delete mImp;
    }

    void testVisit()
    {
        ModuleImportTestVisitor v;
        CPPUNIT_ASSERT (((bool) mImp->visit(&v)) == true);
    }

    void testImported()
    {
        CPPUNIT_ASSERT (mImp->imported() == mImported);
    }

    void testIsDirect()
    {
        CPPUNIT_ASSERT (mImp->isDirect() == true);
    }

private:
    ModuleImport* mImp;
    Module* mImported;
};

#endif
