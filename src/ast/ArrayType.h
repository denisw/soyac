/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef SOYA_ARRAY_TYPE_H_
#define SOYA_ARRAY_TYPE_H_

#include "Type.h"
#include <map>

namespace soyac {
namespace ast {

class Function;
class Property;

/**
 * Represents a structure type.
 */
class ArrayType : public Type {
public:
    /**
     * Returns the ArrayType instance representing the array type with
     * the passed element type.
     *
     * @param elementType  The array element type.
     * @return             The requested ArrayType.
     */
    static ArrayType* get(Type* elementType);

    /**
     * Visits the ArrayType.
     *
     * @see Node::visit()
     */
    virtual void* visit(Visitor* v);

    /**
     * Returns the integer type which is used for the enum type's value
     * representation.
     *
     * @return  The enum type's underlying integer type.
     */
    Type* elementType() const;

    /**
     * Returns the getElement() method of the array type.
     *
     * @return  The array type's getElement() method.
     */
    Function* getElementMethod() const;

    /**
     * Returns the setElement() method of the array type.
     *
     * @return  The array type's setElement() method.
     */
    Function* setElementMethod() const;

    /**
     * Returns the "length" property of the array type.
     *
     * @return  The array type's "length" property.
     */
    Property* lengthProperty() const;

private:
    static std::map<Type*, ArrayType*> mInstances;

    Type* mElementType;
    Function* mGetElementMethod;
    Function* mSetElementMethod;
    Property* mLengthProperty;

    /**
     * Creates an ArrayType.
     *
     * @param elementType  The array element type.
     */
    ArrayType(Type* elementType);
};

} // namespace ast
} // namespace soyac

#endif
