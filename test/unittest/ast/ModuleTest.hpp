/*
 * soyac - The Soya programming language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _MODULE_TEST_HPP
#define _MODULE_TEST_HPP

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <ast/DeclarationStatement.hpp>
#include <ast/Module.hpp>
#include "UnitTestUtil.hpp"
#include "DummyDeclaredEntity.hpp"

using namespace soyac::ast;

class ModuleTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE (ModuleTest);
    CPPUNIT_TEST (testVisit);
    CPPUNIT_TEST (testGet);
    CPPUNIT_TEST (testGetNonExistent);
    CPPUNIT_TEST (testQualifiedNameOfModuleMembers);
    CPPUNIT_TEST_SUITE_END ();

    TEST_VISITOR (Module);

public:
    void setUp()
    {
        mModule = Module::get(Name("foo::bar"), true);
    }

    void tearDown()
    {
    }

    void testVisit()
    {
        ModuleTestVisitor v;
        CPPUNIT_ASSERT (((bool) mModule->visit(&v)) == true);
    }

    void testGet()
    {
        CPPUNIT_ASSERT (Module::get(Name("foo::bar")) == mModule);
        CPPUNIT_ASSERT (Module::get(Name("bar::baz"), true) != mModule);
    }

    void testGetNonExistent()
    {
        CPPUNIT_ASSERT (Module::get(Name("does::not::exist")) == 0);
        CPPUNIT_ASSERT (Module::get(Name("does::not::exist"), true) != 0);
    }

    void testQualifiedNameOfModuleMembers()
    {
        DeclaredEntity* member = new DummyDeclaredEntity(Name("baz"));
        DeclarationStatement* dstmt = new DeclarationStatement(member);

        mModule->body()->addStatement(dstmt);

        Name qualifiedName = member->qualifiedName();
        Name::identifiers_iterator it = qualifiedName.identifiers_begin();

        CPPUNIT_ASSERT (*it == "foo::bar");
        it++;
        CPPUNIT_ASSERT (*it == "baz");
        it++;
        CPPUNIT_ASSERT (it == qualifiedName.identifiers_end());
    }

private:
    Module* mModule;
};

#endif
