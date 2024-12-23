/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef SOYA_FUNCTION_GROUP_IMPORT_H_
#define SOYA_FUNCTION_GROUP_IMPORT_H_

#include "DeclaredEntity.h"
#include "Function.h"
#include "Import.h"
#include "NodeList.h"

namespace soyac {
namespace ast {

/**
 * Represents an import of a declared entity.
 */
class FunctionGroupImport : public Import {
public:
    /**
     * Iterates over the imported function overloads (constant).
     */
    typedef NodeList<Function>::const_iterator overloads_iterator;

    /**
     * Creates a FunctionGroupImport.
     *
     * @param overloads_begin  Start iterator for the imported function
     *                         overloads.
     * @param overloads_end    End iterator for the imported function
     *                         overloads.
     */
    template <class InputIterator>
    FunctionGroupImport(
        InputIterator overloads_begin, InputIterator overloads_end)
    {
        for (InputIterator it = overloads_begin; it != overloads_end; it++) {
            mOverloads.push_back(*it);
        }
    }

    /**
     * Visits the FunctionGroupImport.
     *
     * @see Node::visit()
     */
    virtual void* visit(Visitor* v);

    /**
     * Returns a constant iterator pointing to the first function overload
     * imported by the FunctionGroupImport.
     *
     * @return  The start iterator for the function overloads.
     */
    overloads_iterator overloads_begin() const;

    /**
     * Returns a constant iterator pointing past the last function overload
     * imported by the FunctionGroupImport.
     *
     * @return  The end iterator for the imported function overloads.
     */
    overloads_iterator overloads_end() const;

private:
    NodeList<Function> mOverloads;
};

} // namespace ast
} // namespace soyac

#endif
