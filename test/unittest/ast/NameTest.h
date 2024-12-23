/*
 * soyac - The Soya programming language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef SOYA_NAME_TEST_H_
#define SOYA_NAME_TEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <list>
#include <sstream>
#include <ast/Name.h>

using namespace soyac::ast;

class NameTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE (NameTest);
    CPPUNIT_TEST (testIdentifiers);
    CPPUNIT_TEST (testFirst);
    CPPUNIT_TEST (testLast);
    CPPUNIT_TEST (testCountIdentifiers);
    CPPUNIT_TEST (testStr);
    CPPUNIT_TEST (testEqualityOperator);
    CPPUNIT_TEST (testUnequalityOperator);
    CPPUNIT_TEST (testPlusOperator);
    CPPUNIT_TEST (testStreamOperator);
    CPPUNIT_TEST_SUITE_END ();

public:
    void setUp()
    {
        std::list<std::string> identifiers;
        identifiers.push_back("foo");
        identifiers.push_back("bar");
        identifiers.push_back("baz");

        mName = new Name(identifiers.begin(), identifiers.end());
    }

    void tearDown()
    {
        delete mName;
    }

    void testIdentifiers()
    {
        Name::identifiers_iterator it = mName->identifiers_begin();

        CPPUNIT_ASSERT (*it == "foo");
        it++;
        CPPUNIT_ASSERT (*it == "bar");
        it++;
        CPPUNIT_ASSERT (*it == "baz");
        it++;
        CPPUNIT_ASSERT (it == mName->identifiers_end());
    }

    void testFirst()
    {
        CPPUNIT_ASSERT (mName->first() == "foo");
    }

    void testLast()
    {
        CPPUNIT_ASSERT (mName->last() == "baz");
    }

    void testCountIdentifiers()
    {
        CPPUNIT_ASSERT (mName->countIdentifiers() == 3);
    }

    void testStr()
    {
        CPPUNIT_ASSERT (mName->str() == "foo.bar.baz");
    }

    void testEqualityOperator()
    {
        std::list<std::string> identifiers;
        identifiers.push_back("foo");
        identifiers.push_back("bar");
        identifiers.push_back("baz");

        Name name2(identifiers.begin(), identifiers.end());
        CPPUNIT_ASSERT (*mName == name2);
    }

    void testUnequalityOperator()
    {
        std::list<std::string> identifiers;
        identifiers.push_back("foo");
        identifiers.push_back("but");
        identifiers.push_back("not");
        identifiers.push_back("the");
        identifiers.push_back("same");

        Name name2(identifiers.begin(), identifiers.end());
        CPPUNIT_ASSERT (*mName != name2);
    }

    void testPlusOperator()
    {
        std::list<std::string> ids;
        ids.push_back("blubb");
        ids.push_back("blubbub");

        Name name2(ids.begin(), ids.end());
        Name name3 = *mName + name2;

        Name::identifiers_iterator it = name3.identifiers_begin();

        CPPUNIT_ASSERT (*it == "foo");
        it++;
        CPPUNIT_ASSERT (*it == "bar");
        it++;
        CPPUNIT_ASSERT (*it == "baz");
        it++;
        CPPUNIT_ASSERT (*it == "blubb");
        it++;
        CPPUNIT_ASSERT (*it == "blubbub");
        it++;
        CPPUNIT_ASSERT (it == name3.identifiers_end());
    }

    void testStreamOperator()
    {
        std::stringstream s;
        s << *mName;
        CPPUNIT_ASSERT (s.str() == "foo.bar.baz");
    }

private:
    Name* mName;
};

#endif
