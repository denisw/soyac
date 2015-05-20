/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _LINK_HPP
#define _LINK_HPP

#include <cassert>
#include <sigc++/connection.h>
#include <sigc++/signal.h>
#include "Node.hpp"

namespace soyac {
namespace ast
{

/**
 * A smart pointer class that represents a reference to an abstract syntax
 * node. Each Link automatically manages the reference count of the Node
 * instance it refers to (the "target"). Additionally, Link instances listen to
 * their target node's "replaceRequested" signal (see Node::replaceRequested())
 * and adjust their targets accordingly when notified.
 *
 * The accepted target node type of a Link may be constrained through its
 * template parameter.
 *
 * @tparam T  The target node type.
 */
template <class T>
class Link
{
public:
    /**
     * Creates a Link.
     *
     * @param target  The link's target.
     */
    Link(T* target = NULL)
        : mTarget(NULL)
    {
        setTarget(target);
    }

    /**
     * Link destructor.
     */
    ~Link()
    {
        if (mTarget != NULL)
        {
            mConnection.disconnect();
            ((Node*) mTarget)->unref();
        }
    }

    /**
     * Copy constructor.
     */
    Link(const Link<T>& other)
        : mTarget(NULL)
    {
        setTarget(other.mTarget);
    }

    /**
     * Assignment operator.
     */
    Link<T>& operator=(const Link<T>& other)
    {
        setTarget(other.mTarget);
        return *this;
    }

    /**
     * Returns the Node instance referenced by the Link.
     *
     * @return  The link's target.
     */
    T* target() const
    {
        return mTarget;
    }

    /**
     * Sets the link's target. This triggers the Link's "targetChanged"
     * signal (see targetChanged()). If the passed node is already the
     * link's target, nothing is done.
     *
     * @param target  The link's target.
     */
    void setTarget(T* target)
    {
        if (target != mTarget)
        {
            mConnection.disconnect();

            if (target != NULL)
            {
                ((Node*) target)->ref();
                mConnection = ((Node*) target)->replaceRequested().connect(
                  sigc::mem_fun(this, &Link::onReplaceRequested));
            }

            T* oldTarget = mTarget;
            mTarget = target;

            targetChanged()(oldTarget, mTarget);

            if (oldTarget != NULL)
                ((Node*) oldTarget)->unref();
        }
    }

    /**
     * Returns the Link's "targetChanged" signal, which is emitted whenever
     * the link's target has changed. This happens when setTarget() has been
     * called on the Link, or if the Link's target has been replaced with
     * Node::replaceWith().
     *
     * @par Callback Signature:
     * <tt>void onTargetChanged(Node* oldTarget, Node* newTarget);</tt>
     *
     * @return  The "targetChanged" signal.
     */
    typename sigc::signal<void, T*, T*>& targetChanged()
    {
        return mTargetChanged;
    }

private:
    T* mTarget;
    typename sigc::signal<void, T*, T*> mTargetChanged;
    sigc::connection mConnection;

    /**
     * Handler for the target node's "replaceRequested" signal
     * (see Node::replaceRequested()).
     */
    void onReplaceRequested(Node* oldTarget, Node* newTarget)
    {
/*
        T* castTarget = dynamic_cast<T*>(newTarget);

        if (newTarget != NULL)
            assert (castTarget != NULL);
*/
        setTarget((T*) newTarget);
    }
};

}}

#endif
