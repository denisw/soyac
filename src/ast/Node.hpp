/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _AST_NODE_HPP
#define _AST_NODE_HPP

#include <list>
#include <sigc++/signal.h>
#include "Location.hpp"

namespace soyac {
namespace ast
{

class Visitor;

/**
 * The base class of all classes whose instances represent abstract syntax
 * tree nodes. An abstract syntax node represents a specific element or
 * code section of a Soya source or interface file.
 *
 * The Node class provides the following features:
 *
 * - <b>Reference counting:</b> Each node begins with a reference count
 *   of 0. Using the ref() and unref() methods, the count can be incremented
 *   and decremented; a decrease to or beneath 0 destroys the node. In most
 *   cases, reference count management is not done manually, but is automated
 *   by wrapping Node references in Link objects. <em>Note that while it
 *   is possible, you should NOT directly delete Node instances that are part
 *   of an abstract syntax tree, as this might render the tree inconsistent!</em>
 * - <b>The replaceWith() method:</b> Using replaceWith(), all Link references
 *   to a node can be replaced by references to another node, thus fully
 *   substiting one node with another in the abstract syntax tree. This is
 *   useful in several circumstances, for instance for replacing expressions
 *   with semantically equivalent, but simpler versions for optimization
 *   purposes.
 * - <b>Visitor hooks:</b> The Node class provides a visit() method, which
 *   visits the node with a specified Visitor. visit() is overridden by every
 *   concrete child class of Node to call the correct visit method of the
 *   passed Visitor instance, which means that the caller does not need to know
 *   the run-time type of a node to call the correct visitor method.
 */
class Node
{
public:
    /**
     * Creates an Node.
     */
    Node();

    /**
     * The destructor.
     */
   virtual ~Node();

    /**
     * Increases the reference count of the Node instance by one.
     */
    void ref();

    /**
     * Decreases the reference count of the Node instance by one,
     * which destroys the node if the count reaches zero.
     *
     * <strong>Do only manually call unref() after a manual call to ref()!
     * </strong> Otherwise a Node object still referred to by an abstract
     * syntax tree might be destroyed, which will render the tree
     * inconsistent! 
     * 
     */
    void unref();

    /**
     * Returns the location of the code element represented by the
     * Node instance.
     *
     * @return  The node's location data.
     */
    const Location& location() const;

    /**
     * Sets the location of the code element represented by the Node
     * instance.
     *
     * @param l  The Node's location data
     */
    void setLocation(const Location& l);

    /**
     * Replaces all abstract syntax tree references to the Node instance with
     * references to the passed other node. Replacing a node with null is not
     * allowed.
     *
     * @param node  The node with which to replace. Null is ignored.
     */
    template <class N>
    void replaceWith(N* node)
    {
        if (node != NULL)
            _replaceWith(node);
    }

    /**
     * Visits the node with the specified Visitor.
     *
     * Every concrete child class of Node should override this to call the
     * correct visitor method of the passed Visitor instance.
     *
     * @param v  The Visitor with which the node should be visited.
     * @return   The visitor method's return value.
     */
    virtual void* visit(Visitor* v)
    {
        return NULL;
    }

    /**
     * Returns the "replaceRequested" signal, which is emitted when the
     * Node instance's replaceWith() method is called.
     *
     * Link instances pointing to the node subscribe to this signal to
     * do the be notified when a replacement operation is required. The
     * signal can also be subscribed to by other nodes or the signal-emitting
     * Node instance itself, though.
     *
     * Note that after the each time the "replaceRequested" signal has been
     * emitted, all signal handlers are disconnected.
     *
     * @par Callback Signature:
     * <tt>void onReplaceRequested(Node* oldNode, Node* newNode);</tt>
     *
     * @return  The "replaceRequested" signal.
     */
    sigc::signal<void, Node*, Node*>& replaceRequested();

private:
    int mRefCount;
    Location mLocation;
    sigc::signal<void, Node*, Node*> mReplaceRequested;

    /**
     * The concrete implementation of replaceWith().
     */
    void _replaceWith(Node* node);
};

}}

#endif
