/*
 * soyac - The Soya programming language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _UNKNOWN_TYPE_TEST_HPP
#define _UNKNOWN_TYPE_TEST_HPP

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <list>
#include <string>
#include <ast/UnknownType.hpp>
#include "DummyType.hpp"

using namespace soyac::ast;

class UnknownTypeTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE (UnknownTypeTest);
    CPPUNIT_TEST (testGetSingleton);
    CPPUNIT_TEST (testVisit);
    CPPUNIT_TEST (testImplicitlyConvertable);    
    CPPUNIT_TEST (testQualifiedName);
    CPPUNIT_TEST_SUITE_END ();

    TEST_VISITOR (UnknownType);

public:
    void setUp()
    {
        mType = UnknownType::getSingleton();
    }

    void tearDown()
    {
    }

    void testGetSingleton()
    {
        CPPUNIT_ASSERT (UnknownType::getSingleton() == mType);
    }

    void testVisit()
    {
        UnknownTypeTestVisitor v;
        CPPUNIT_ASSERT (((bool) mType->visit(&v)) == true);
    }

    void testImplicitlyConvertable()
    {
        DummyType type2;
        CPPUNIT_ASSERT (mType->isImplicitlyConvertableTo(&type2));
    }

    void testQualifiedName()
    {
        std::list<std::string> identifiers;
        identifiers.push_back("foo");
        identifiers.push_back("bar");
        identifiers.push_back("baz");

        Name name(identifiers.begin(), identifiers.end());
        UnknownType type(name);

        Name qname = type.qualifiedName();
        Name::identifiers_iterator it = qname.identifiers_begin();

        CPPUNIT_ASSERT (*it == "foo");
        it++;
        CPPUNIT_ASSERT (*it == "bar");
        it++;
        CPPUNIT_ASSERT (*it == "baz");
        it++;
        CPPUNIT_ASSERT (it == qname.identifiers_end());
    }

private:
    UnknownType* mType;
};

#endif
