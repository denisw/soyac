/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef SOYA_FUNCTION_GROUP_H_
#define SOYA_FUNCTION_GROUP_H_

#include <ast/Function.h>

using namespace soyac::ast;

namespace soyac {
namespace analysis {

/**
 * Represents a group of overloaded functions. Used as a return value
 * for SymbolTable::lookup().
 */
class FunctionGroup : public NamedEntity {
public:
    /**
     * Iterates over the function group's functions (constant).
     */
    typedef NodeList<Function>::const_iterator overloads_iterator;

    /**
     * Creates a FunctionGroup.
     *
     * @param overloads_begin  Start iterator for the represented overloaded
     *                         functions. All functions must have the same
     *                         name and enclosing entity.
     * @param overloads_begin  End iterator for the represented overloaded
     *                         functions.
     */
    template <class InputIterator>
    FunctionGroup(InputIterator overloads_begin, InputIterator overloads_end)
        : NamedEntity((*overloads_begin)->name())
        , mOverloads(overloads_begin, overloads_end)
    {
#ifndef NDEBUG
        InputIterator it = overloads_begin;
        Name name = (*it)->name();
        NamedEntity* parent = (*it)->parent();
        it++;

        for (; it != overloads_end; it++) {
            assert((*it)->name() == name);
            assert((*it)->parent() == parent);
        }
#endif
    }

    /**
     * Returns an iterator pointing to the first function of
     * the group.
     *
     * @return  The start iterator for the represented overloaded functions.
     */
    overloads_iterator overloads_begin() const;

    /**
     * Returns an iterator pointing past the last function of
     * the group.
     *
     * @return  The start iterator for the represented overloaded functions.
     */
    overloads_iterator overloads_end() const;

    /**
     * Adds a function to the group.
     *
     * @param func  The function to add.
     */
    void addOverload(Function* func);

private:
    NodeList<Function> mOverloads;
};

} // namespace analysis
} // namespace soyac

#endif
