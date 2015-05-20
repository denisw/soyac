/*
 * soyac - The Soya programming language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _NODE_LIST_TEST_HPP
#define _NODE_LIST_TEST_HPP

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <list>
#include <ast/NodeList.hpp>
#include "DummyNode.hpp"

using namespace soyac::ast;

class NodeListTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE (NodeListTest);
    CPPUNIT_TEST (testIterators);
    CPPUNIT_TEST (testSize);
    CPPUNIT_TEST (testPushBack);
    CPPUNIT_TEST (testRemove);
    CPPUNIT_TEST (testElementReplaced);
    CPPUNIT_TEST (testChangedOnReplace);
    CPPUNIT_TEST (testChangedOnPushBack);
    CPPUNIT_TEST (testChangedOnRemove);
    CPPUNIT_TEST_SUITE_END ();

    void changedCallback(Node* oldNode, Node* newNode)
    {
        CPPUNIT_ASSERT (oldNode == mOldNode);
        CPPUNIT_ASSERT (newNode == mNewNode);
        mCallbackCalled = true;
    }

public:
    void setUp()
    {
        mNode1 = new DummyNode;
        mNode2 = new DummyNode;
        mNode3 = new DummyNode;

        std::list<Node*> nodes;
        nodes.push_back(mNode1);
        nodes.push_back(mNode2);
        nodes.push_back(mNode3);

        mList = new NodeList<Node>(nodes.begin(), nodes.end());
    }

    void tearDown()
    {
        delete mList;
    }

    void testIterators()
    {
        NodeList<Node>::const_iterator it = mList->begin();

        CPPUNIT_ASSERT (*it == mNode1);
        it++;
        CPPUNIT_ASSERT (*it == mNode2);
        it++;
        CPPUNIT_ASSERT (*it == mNode3);
        it++;
        CPPUNIT_ASSERT (it == mList->end());
    }

    void testSize()
    {
        CPPUNIT_ASSERT (mList->size() == 3);
    }

    void testPushBack()
    {
        DummyNode node4;
        mList->push_back(&node4);

        NodeList<Node>::const_iterator it = mList->begin();

        CPPUNIT_ASSERT (*it == mNode1);
        it++;
        CPPUNIT_ASSERT (*it == mNode2);
        it++;
        CPPUNIT_ASSERT (*it == mNode3);
        it++;
        CPPUNIT_ASSERT (*it == &node4);
        it++;
        CPPUNIT_ASSERT (it == mList->end());
    }

    void testRemove()
    {
        mList->remove(mNode2);

        NodeList<Node>::const_iterator it = mList->begin();

        CPPUNIT_ASSERT (*it == mNode1);
        it++;
        CPPUNIT_ASSERT (*it == mNode3);
        it++;
        CPPUNIT_ASSERT (it == mList->end());
    }

    void testElementReplaced()
    {
        DummyNode node4;
        mNode2->replaceWith(&node4);

        NodeList<Node>::const_iterator it = mList->begin();

        CPPUNIT_ASSERT (*it == mNode1);
        it++;
        CPPUNIT_ASSERT (*it == &node4);
        it++;
        CPPUNIT_ASSERT (*it == mNode3);
        it++;
        CPPUNIT_ASSERT (it == mList->end());
    }

    void testChangedOnReplace()
    {
        mCallbackCalled = false;

        mList->changed().connect(
          sigc::mem_fun(this, &NodeListTest::changedCallback));

        mOldNode = mNode1;
        mNewNode = new DummyNode;
        mOldNode->replaceWith(mNewNode);
        CPPUNIT_ASSERT (mCallbackCalled == true);

        mList->changed().clear();
    }

    void testChangedOnPushBack()
    {
        mCallbackCalled = false;

        mList->changed().connect(
          sigc::mem_fun(this, &NodeListTest::changedCallback));

        mOldNode = 0;
        mNewNode = new DummyNode;
        mList->push_back(mNewNode);
        CPPUNIT_ASSERT (mCallbackCalled == true);

        mList->changed().clear();
    }

    void testChangedOnRemove()
    {
        mCallbackCalled = false;

        mList->changed().connect(
          sigc::mem_fun(this, &NodeListTest::changedCallback));

        mOldNode = mNode1;
        mNewNode = 0;
        mList->remove(mNode1);
        CPPUNIT_ASSERT (mCallbackCalled == true);

        mList->changed().clear();
    }

private:
    NodeList<Node>* mList;
    Node* mNode1;
    Node* mNode2;
    Node* mNode3;
    Node* mOldNode;
    Node* mNewNode;
    bool mCallbackCalled;
};

#endif
