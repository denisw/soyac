/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include "Node.h"
#include "Link.h"
#include <algorithm>
#include <cstdlib>
#include <iostream>

namespace soyac {
namespace ast {

using boost::signals2::signal;

Node::Node()
    : mRefCount(0)
{
}

Node::~Node()
{
    if (mRefCount > 0) {
        mRefCount = -1;
        _replaceWith(nullptr);
    }
}

void Node::ref()
{
    if (mRefCount < 0) {
        return;
    } else {
        mRefCount++;
    }
}

void Node::unref()
{
    if (mRefCount < 0) {
        return;
    }

    mRefCount--;

    if (mRefCount <= 0) {
        delete this;
    }
}

const Location& Node::location() const { return mLocation; }

void Node::setLocation(const Location& l) { mLocation = l; }

void Node::_replaceWith(Node* node)
{
    /*
     * Give the replacing node our location information.
     */
    if (node) {
        node->setLocation(location());
    }

    /*
     * We need to ensure that the Node object isn't destroyed by the
     * "replaceRequested" signal handlers even if they unref() the
     * Node. Calling ref() before emitting the signal and unref()
     * aferwards does the trick.
     */
    ref();
    mReplaceRequested(this, node);
    unref();
}

signal<void(Node*, Node*)>& Node::replaceRequested()
{
    return mReplaceRequested;
}

} // namespace ast
} // namespace soyac
