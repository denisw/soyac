/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef SOYA_NODE_LIST_H_
#define SOYA_NODE_LIST_H_

#include "Link.h"
#include "Node.h"
#include <boost/signals2/signal.hpp>
#include <functional>
#include <list>

namespace soyac {
namespace ast {

/**
 * Represents a list of abstract syntax tree nodes of the specified type.
 * Internally, a NodeList stores its elements as Link instances, which
 * gives it the same reference count management and Node::replaceWith()
 * awareness features.
 *
 * @tparam N  The Node subclass whose instances should be stored
 *            in the NodeList. Note that the type must be specified
 *            in it's pure form, not as pointer or reference type.
 */
template <class N> class NodeList {
private:
    typename std::list<Link<N>*> mData;
    typename boost::signals2::signal<void(N*, N*)> mChanged;

    void onTargetChanged(Node* oldTarget, Node* newTarget)
    {
        changed()((N*)oldTarget, (N*)newTarget);

        if (newTarget == nullptr) {
            remove(nullptr);
        }
    }

public:
    /**
     * The NodeList's constant iterator type.
     */
    class const_iterator {
    public:
        /**
         * Increments the iterator.
         */
        const_iterator operator++()
        {
            mIterator++;
            return *this;
        }

        /**
         * Increments the iterator (postfix).
         */
        const_iterator operator++(int postfix)
        {
            const_iterator it(mIterator);
            mIterator++;
            return it;
        }

        /*
         * Dereferences the iterator.
         */
        N* operator*() const { return (N*)(*mIterator)->target(); }

        /**
         * Returns @c true if the iterator equals the passed other iterator.
         */
        bool operator==(const_iterator it) { return mIterator == it.mIterator; }

        /**
         * Returns @c true if the iterator does not equal the passed other
         * iterator.
         */
        bool operator!=(const_iterator it) { return !(*this == it); }

    private:
        friend class NodeList;
        typename std::list<Link<N>*>::const_iterator mIterator;

        const_iterator(typename std::list<Link<N>*>::const_iterator it)
            : mIterator(it)
        {
        }
    };

    /**
     * Creates a NodeList.
     */
    NodeList() { }

    /**
     * Creates a NodeList with the elements from iterator @c first to
     * @c last (excluding the last iterator's value).
     *
     * @param first  The start iterator.
     * @param last   The end iterator.
     */
    template <class InputIterator>
    NodeList(InputIterator first, InputIterator last)
    {
        for (InputIterator it = first; it != last; it++) {
            push_back(*it);
        }
    }

    /**
     * The NodeList destructor.
     */
    ~NodeList()
    {
        for (typename std::list<Link<N>*>::iterator it = mData.begin();
            it != mData.end(); it++) {
            delete *it;
        }
    }

    /**
     * Returns a constant iterator pointing to the list's first element.
     *
     * @return  The constant start iterator.
     */
    const_iterator begin() const { return const_iterator(mData.begin()); }

    /**
     * Returns a constant iterator pointing past the list's last element.
     *
     * @return  The constant end iterator.
     */
    const_iterator end() const { return const_iterator(mData.end()); }

    /**
     * Returns the number of elements in the list.
     *
     * @return  The list's size.
     */
    size_t size() const { return mData.size(); }

    /**
     * Adds an element to the end of the list.
     *
     * @param n  The node to add.
     */
    void push_back(N* n)
    {
        using namespace std::placeholders;

        Link<N>* l = new Link<N>(n);

        l->targetChanged().connect(
            [this](auto old_, auto new_) { onTargetChanged(old_, new_); });

        mData.push_back(l);
        changed()(nullptr, n);
    }

    /**
     * Removes all occurrences of the passed node from the list.
     *
     * @param n  The node to remove.
     */
    void remove(N* n)
    {
        for (typename std::list<Link<N>*>::iterator it = mData.begin();
            it != mData.end();) {
            if ((*it)->target() == n) {
                Link<N>* removed = *it;
                it = mData.erase(it);

                changed()(n, nullptr);
                delete removed;
            } else {
                it++;
            }
        }
    }

    /**
     * Returns the "changed" signal, which is emitted whenever
     * an element is added to, removed from or replaced within
     * the node list.
     *
     * @par Signature Callback:
     * <tt>void onElementChanged(N* oldElement, N* newElement);</tt>
     * @par
     * If an element has been added, @i oldElement is null. If a node has
     * been removed, @i newElement is null. If an element has been replaced
     * with another one, both @i oldElement and @i newElement are non-null.
     *
     * @return  The "changed" signal.
     */
    typename boost::signals2::signal<void(N*, N*)>& changed()
    {
        return mChanged;
    }
};

} // namespace ast
} // namespace soyac

#endif
