/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef SOYA_UNKNOWN_FUNCTION_TYPE_H_
#define SOYA_UNKNOWN_FUNCTION_TYPE_H_

#include "Link.h"
#include "NodeList.h"
#include "UnknownType.h"

namespace soyac {
namespace ast {

/**
 * Placeholder for a to-be-determined function type.
 */
class UnknownFunctionType : public UnknownType {
public:
    /**
     * Iterates over the function type's to-be-determined parameter types
     * (constant).
     */
    typedef NodeList<Type>::const_iterator parameterTypes_iterator;

    /**
     * Creates an UnknownFunctionType.
     *
     * @param elementType  The unknown element type.
     */
    template <class InputIterator>
    UnknownFunctionType(Type* returnType, InputIterator parameterTypes_begin,
        InputIterator parameterTypes_end)
        : UnknownType("(function)")
        , mReturnType(returnType)
        , mParameterTypes(parameterTypes_begin, parameterTypes_end)
    {
    }

    /**
     * Visits the UnknownFunctionType.
     *
     * @see Node::visit()
     */
    virtual void* visit(Visitor* v);

    /**
     * Returns the (possibly to-be-determined) return type of the
     * function type.
     *
     * @return  The function type's return type.
     */
    Type* returnType() const;

    /**
     * Returns a constant iterator pointing to the first (possibly
     * to-be-determined) parameter type of the function type.
     *
     * @return  The start iterator for the function type's parameter
     *          types.
     */
    parameterTypes_iterator parameterTypes_begin() const;

    /**
     * Returns a constant iterator pointing past the last (possibly
     * to-be-determined) parameter type of the function type.
     *
     * @return  The end iterator for the function type's parameter
     *          types.
     */
    parameterTypes_iterator parameterTypes_end() const;

private:
    Link<Type> mReturnType;
    NodeList<Type> mParameterTypes;
};

} // namespace ast
} // namespace soyac

#endif
