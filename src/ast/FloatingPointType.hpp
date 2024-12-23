/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _FLOATING_POINT_TYPE_HPP
#define _FLOATING_POINT_TYPE_HPP

#include "BuiltInType.hpp"

/**
 * The "float" type. This is a shortcut to FloatingPointType::getFloat().
 */
#define TYPE_FLOAT soyac::ast::FloatingPointType::getFloat()

/**
 * The "double" type. This is a shortcut to FloatingPointType::getDouble().
 */
#define TYPE_DOUBLE soyac::ast::FloatingPointType::getDouble()

namespace soyac {
namespace ast {

/**
 * Represents a floating-point type.
 *
 * (See the Soya Language Reference,)
 */
class FloatingPointType : public BuiltInType {
public:
    /**
     * Returns the FloatingPointType instance representing the "float" type.
     *
     * @return  The "float" type.
     */
    static FloatingPointType* getFloat();

    /**
     * Returns the FloatingPointType instance representing the "double"
     * type.
     *
     * @return  The "double" type.
     */
    static FloatingPointType* getDouble();

    /**
     * Override of Type::isConvertableTo().
     */
    virtual bool isConvertableTo(Type* other) const;

    /**
     * Override of Type::isImplicitlyConvertableTo().
     */
    virtual bool isImplicitlyConvertableTo(Type* other) const;

    /**
     * Visits the FloatingPointType.
     *
     * @see Node::visit()
     */
    virtual void* visit(Visitor* v);

private:
    static FloatingPointType* sFloat;
    static FloatingPointType* sDouble;

    /**
     * FloatingPointType constructor.
     *
     * @param isFloat  @c true if the FloatingPointType represents "double",
     *                 @c false otherwise.
     */
    FloatingPointType(bool isDouble);
};

} // namespace ast
} // namespace soyac

#endif
