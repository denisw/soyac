/*
 * soyac - The Soya programming language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _LINK_TEST_HPP
#define _LINK_TEST_HPP

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <ast/Link.hpp>
#include "DummyNode.hpp"

using namespace soyac::ast;

class LinkTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE (LinkTest);
    CPPUNIT_TEST (testTarget);
    CPPUNIT_TEST (testSetTarget);
    CPPUNIT_TEST (testTargetChangedOnSetTarget);
    CPPUNIT_TEST (testTargetChangedOnReplace);
    CPPUNIT_TEST (testTargetUnchangedIfNoNewTarget);
    CPPUNIT_TEST (testCopyConstructor);
    CPPUNIT_TEST (testAssignmentOperator);
    CPPUNIT_TEST_SUITE_END ();

    void targetChangedCallback(Node* oldTarget, Node* newTarget)
    {
        CPPUNIT_ASSERT (oldTarget == mTarget);
        CPPUNIT_ASSERT (newTarget == mNewTarget);
        mCallbackCalled = true;
    }

public:
    void setUp()
    {
        mTarget = new DummyNode;
        mLink = new Link<Node>(mTarget);
    }

    void tearDown()
    {
        delete mLink;
    }

    void testTarget()
    {
        CPPUNIT_ASSERT (mLink->target() == mTarget);
    }

    void testSetTarget()
    {
        DummyNode newTarget;
        mLink->setTarget(&newTarget);
        CPPUNIT_ASSERT (mLink->target() == &newTarget);
    }

    void testTargetChangedOnSetTarget()
    {
        mCallbackCalled = false;

        mLink->targetChanged().connect(
          sigc::mem_fun(this, &LinkTest::targetChangedCallback));

        mNewTarget = new DummyNode;
        mLink->setTarget(mNewTarget);
        CPPUNIT_ASSERT (mCallbackCalled);

        mLink->targetChanged().clear();
    }

    void testTargetChangedOnReplace()
    {
        mCallbackCalled = false;

        mLink->targetChanged().connect(
          sigc::mem_fun(this, &LinkTest::targetChangedCallback));

        mNewTarget = new DummyNode;
        mTarget->replaceWith(mNewTarget);
        CPPUNIT_ASSERT (mCallbackCalled);

        mLink->targetChanged().clear();
    }

    void testTargetUnchangedIfNoNewTarget()
    {
        mCallbackCalled = false;

        mLink->targetChanged().connect(
          sigc::mem_fun(this, &LinkTest::targetChangedCallback));

        mLink->setTarget(mTarget);
        CPPUNIT_ASSERT (!mCallbackCalled);

        mLink->targetChanged().clear();
    }

    void testCopyConstructor()
    {
        Link<Node> link2(*mLink);
        CPPUNIT_ASSERT (link2.target() == mTarget);

        delete mTarget;
        CPPUNIT_ASSERT (mLink->target() == NULL);
    }

    void testAssignmentOperator()
    {
        Link<Node> link2;
        link2 = *mLink;
        CPPUNIT_ASSERT (link2.target() == mTarget);
    }

private:
    Link<Node>* mLink;
    Node* mTarget;
    Node* mNewTarget;
    bool mCallbackCalled;
};

#endif
