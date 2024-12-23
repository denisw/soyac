/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _ENUM_TYPE_HPP
#define _ENUM_TYPE_HPP

#include "EnumConstant.hpp"
#include "IntegerType.hpp"
#include "Type.hpp"

namespace soyac {
namespace ast {

class Function;

/**
 * Represents an enum type.
 */
class EnumType : public Type {
public:
    /**
     * Iterates over the enum type's values (constant).
     */
    typedef NodeList<EnumConstant>::const_iterator values_iterator;

    /**
     * Creates an EnumType.
     *
     * @param name             The enum type's name.
     * @param underlyingType   The integer type underlying the enum type.
     * @param constants_begin  Start iterator for the enum type's values.
     * @param constants_end    End iterator for the enum type's values.
     */
    template <class InputIterator>
    EnumType(const Name& name, IntegerType* underlyingType,
        InputIterator values_begin, InputIterator values_end)
        : Type(name)
        , mUnderlyingType(underlyingType)
    {
        mValues.changed().connect([this](auto oldValue, auto newValue) {
            onValuesChanged(oldValue, newValue);
        });

        for (InputIterator it = values_begin; it != values_end; it++) {
            mValues.push_back(*it);
        }

        initEqualsMethod();
    }

    /**
     * Visits the EnumType.
     *
     * @see Node::visit()
     */
    virtual void* visit(Visitor* v);

    /*
     * Override of Type::isConvertableTo().
     */
    virtual bool isConvertableTo(Type* other) const;

    /**
     * Returns the integer type which is used for the enum type's value
     * representation.
     *
     * @return  The enum type's underlying integer type.
     */
    IntegerType* underlyingType() const;

    /**
     * Returns a constant iterator pointing to the first value of
     * the enum type.
     *
     * @return  Start iterator for the enum's values.
     */
    values_iterator values_begin() const;

    /**
     * Returns a constant iterator pointing past the last value of
     * the enum type.
     *
     * @return  End iterator for the enum's values.
     */
    values_iterator values_end() const;

    /**
     * Returns the equals() method of the enum type.
     *
     * @return  The enum type's equals() method.
     */
    Function* equalsMethod() const;

private:
    Link<IntegerType> mUnderlyingType;
    NodeList<EnumConstant> mValues;

    /*
     * The EnumType's equalsMethod() member shouldn't be replaceable, so
     * we don't wrap it into a Link.
     */
    Function* mEqualsMethod;

    /**
     * Creates the enum type's equals() method.
     */
    void initEqualsMethod();

    /**
     * Called whenever the enum type's set of values changes.
     *
     * @see  NodeList::elementChanged()
     */
    void onValuesChanged(EnumConstant* oldVal, EnumConstant* newVal);
};

} // namespace ast
} // namespace soyac

#endif
