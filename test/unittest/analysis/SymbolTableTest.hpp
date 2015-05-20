/*
 * soyac - The Soya programming language compiler
 * Copyright (c) 2008 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _SYMBOL_TABLE_TEST_HPP
#define _SYMBOL_TABLE_TEST_HPP

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <analysis/SymbolTable.hpp>
#include "../ast/DummyDeclaredEntity.hpp"
#include "../ast/DummyStatement.hpp"

using soyac::analysis::SymbolTable;

class SymbolTableTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE (SymbolTableTest);
    CPPUNIT_TEST (testLookupSimple);
    CPPUNIT_TEST (testLookupNestedScope);
    CPPUNIT_TEST (testLookupEnclosingScope);
    CPPUNIT_TEST (testLeaveScope);
    CPPUNIT_TEST (testLookupInEntity);
    CPPUNIT_TEST (testLookupInEntityExcludesEnclosing);
    CPPUNIT_TEST_SUITE_END ();

public:
    void setUp()
    {
        mTable = SymbolTable::get(NULL, true);
    }

    void tearDown()
    {
        delete mTable;
    }

    void testLookupSimple()
    {
        DeclaredEntity* e = new DummyDeclaredEntity("foo");
        mTable->add(e);
        CPPUNIT_ASSERT (mTable->lookup("foo") == e);
    }

    void testLookupNestedScope()
    {
        DeclaredEntity* e1 = new DummyDeclaredEntity("foo");
        mTable->add(e1);

        mTable->enterScope();

        DeclaredEntity* e2 = new DummyDeclaredEntity("foo");
        mTable->add(e2);

        CPPUNIT_ASSERT (mTable->lookup("foo") == e2);
    }

    void testLookupEnclosingScope()
    {
        DeclaredEntity* e = new DummyDeclaredEntity("foo");
        mTable->add(e);

        mTable->enterScope();
        CPPUNIT_ASSERT (mTable->lookup("foo") == e);
    }

    void testLeaveScope()
    {
        DeclaredEntity* e1 = new DummyDeclaredEntity("foo");
        mTable->add(e1);

        mTable->enterScope();

        DeclaredEntity* e2 = new DummyDeclaredEntity("foo");
        mTable->add(e2);

        mTable->leaveScope();

        CPPUNIT_ASSERT (mTable->lookup("foo"));
    }

    void testLookupInEntity()
    {
        DeclaredEntity* scope = new DummyDeclaredEntity("foo");

        mTable->enterScope(scope);

        DeclaredEntity* member = new DummyDeclaredEntity("bar");
        mTable->add(member);

        mTable->leaveScope();
        CPPUNIT_ASSERT (mTable->lookup("bar", scope) == member);
    }

    void testLookupInEntityExcludesEnclosing()
    {
        DeclaredEntity* e = new DummyDeclaredEntity("bar");
        mTable->add(e);

        DeclaredEntity* scope = new DummyDeclaredEntity("foo");
        mTable->enterScope(scope);

        DeclaredEntity* member = new DummyDeclaredEntity("blubb");
        mTable->add(member);

        mTable->leaveScope();

        CPPUNIT_ASSERT (mTable->lookup("bar", scope) == NULL);
    }

private:
    SymbolTable* mTable;
};

#endif
