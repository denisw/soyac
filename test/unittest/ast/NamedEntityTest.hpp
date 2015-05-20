/*
 * soyac - The Soya programming language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _NAMED_ENTITY_TEST_HPP
#define _NAMED_ENTITY_TEST_HPP

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <sstream>
#include <ast/NamedEntity.hpp>

using namespace soyac::ast;

class NamedEntityTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE (NamedEntityTest);
    CPPUNIT_TEST (testName);
    CPPUNIT_TEST (testParent);
    CPPUNIT_TEST (testQualifiedName);
    CPPUNIT_TEST (testQualifiedNameNoParent);
    CPPUNIT_TEST (testStr);
    CPPUNIT_TEST (testStreamOperator);
    CPPUNIT_TEST_SUITE_END ();

    /**
     * Exposes the protected members of NamedEntity.
     */
    class OpenNamedEntity : public NamedEntity
    {
    public:
        OpenNamedEntity(const Name& name)
            : NamedEntity(name)
        {
        }

        void addChild(NamedEntity* child)
        {
            NamedEntity::addChild(child);
        }

        void removeChild(NamedEntity* child)
        {
            NamedEntity::addChild(child);
        }
    };

public:
    void setUp()
    {
        mEntity = new OpenNamedEntity(Name("two"));
    }

    void tearDown()
    {
        delete mEntity;
    }

    void testName()
    {
        CPPUNIT_ASSERT (mEntity->name() == Name("two"));
    }

    void testParent()
    {
        OpenNamedEntity entity2(Name("fourty"));
        entity2.addChild(mEntity);
        CPPUNIT_ASSERT (mEntity->parent() == &entity2);        
    }

    void testQualifiedName()
    {
        OpenNamedEntity entity2(Name("fourty"));
        entity2.addChild(mEntity);

        Name qname = mEntity->qualifiedName();

        Name::identifiers_iterator it = qname.identifiers_begin();
        CPPUNIT_ASSERT (*it == "fourty");
        it++;
        CPPUNIT_ASSERT (*it == "two");
        it++;
        CPPUNIT_ASSERT (it == qname.identifiers_end());

        OpenNamedEntity entity3(Name("number"));
        entity3.addChild(&entity2);

        qname = mEntity->qualifiedName();

        it = qname.identifiers_begin();
        CPPUNIT_ASSERT (*it == "number");
        it++;
        CPPUNIT_ASSERT (*it == "fourty");
        it++;
        CPPUNIT_ASSERT (*it == "two");
        it++;
        CPPUNIT_ASSERT (it == qname.identifiers_end());
    }

    void testQualifiedNameNoParent()
    {
        CPPUNIT_ASSERT (mEntity->qualifiedName() == Name("two"));
    }

    void testStr()
    {
        OpenNamedEntity entity2(Name("fourty"));
        entity2.addChild(mEntity);

        CPPUNIT_ASSERT (mEntity->str() == "fourty.two");
    }

    void testStreamOperator()
    {
        OpenNamedEntity entity2(Name("fourty"));
        entity2.addChild(mEntity);

        std::stringstream stream;
        stream << mEntity;
        CPPUNIT_ASSERT (stream.str() == "fourty.two");
    }

private:
    OpenNamedEntity* mEntity;
    Name* mName;
};

#endif
