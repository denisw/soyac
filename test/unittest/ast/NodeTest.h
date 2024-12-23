/*
 * soyac - The Soya programming language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef SOYA_NODE_TEST_H_
#define SOYA_NODE_TEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <ast/Link.h>
#include "DummyNode.h"

using namespace soyac::ast;

class NodeTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE (NodeTest);
    CPPUNIT_TEST (testLocation);
    CPPUNIT_TEST (testSetLocation);
    CPPUNIT_TEST (testReplaceWith);
    CPPUNIT_TEST_SUITE_END ();

public:
    void setUp()
    {
        mNode = new DummyNode;
    }

    void tearDown()
    {
        delete mNode;
    }

    void testLocation()
    {
        CPPUNIT_ASSERT (mNode->location().fileName() == "<unknown>");
        CPPUNIT_ASSERT (mNode->location().startLine() == 0);
        CPPUNIT_ASSERT (mNode->location().startColumn() == 0);
        CPPUNIT_ASSERT (mNode->location().endLine() == 0);
        CPPUNIT_ASSERT (mNode->location().endColumn() == 0);
    }

    void testSetLocation()
    {
        Location l("foo.soya", 104, 1, 302, 2);
        mNode->setLocation(l);

        CPPUNIT_ASSERT (mNode->location().fileName() == "foo.soya");
        CPPUNIT_ASSERT (mNode->location().startLine() == 104);
        CPPUNIT_ASSERT (mNode->location().startColumn() == 1);
        CPPUNIT_ASSERT (mNode->location().endLine() == 302);
        CPPUNIT_ASSERT (mNode->location().endColumn() == 2);
    }

    void testReplaceWith()
    {
        Location loc("foo.soya", 104, 1, 302, 2);
        mNode->setLocation(loc);

        Link<Node> l(mNode);

        DummyNode *other = new DummyNode;
        mNode->ref();
        mNode->replaceWith(other);

        CPPUNIT_ASSERT (l.target() == other);
        CPPUNIT_ASSERT (other->location().fileName() == "foo.soya");
        CPPUNIT_ASSERT (other->location().startLine() == 104);
        CPPUNIT_ASSERT (other->location().startColumn() == 1);
        CPPUNIT_ASSERT (other->location().endLine() == 302);
        CPPUNIT_ASSERT (other->location().endColumn() == 2);
    }

private:
    Node* mNode;
};

#endif
