/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef SOYA_UNKNOWN_ARRAY_TYPE_H_
#define SOYA_UNKNOWN_ARRAY_TYPE_H_

#include "Link.h"
#include "UnknownType.h"

namespace soyac {
namespace ast {

/**
 * Placeholder for a to-be-determined array type.
 */
class UnknownArrayType : public UnknownType {
public:
    /**
     * Creates an UnknownArrayType.
     *
     * @param elementType  The unknown element type.
     */
    UnknownArrayType(UnknownType* elementType);

    /**
     * Visits the UnknownArrayType.
     *
     * @see Node::visit()
     */
    virtual void* visit(Visitor* v);

    /**
     * Returns the (to-be-determined) element type of the array type.
     *
     * @return  The array type's element type.
     */
    UnknownType* elementType() const;

private:
    Link<UnknownType> mElementType;
};

} // namespace ast
} // namespace soyac

#endif
