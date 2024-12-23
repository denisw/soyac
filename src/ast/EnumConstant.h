/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef SOYA_ENUM_CONSTANT_H_
#define SOYA_ENUM_CONSTANT_H_

#include "DeclaredEntity.h"
#include "IntegerValue.h"

namespace soyac {
namespace ast {

/**
 * Represents an enum constant.
 *
 * (See the Soya Language Reference,)
 */
class EnumConstant : public DeclaredEntity {
public:
    /**
     * Creates a EnumConstant.
     *
     * @param name   The enum constant's name.
     * @param value  The enum constant's value.
     */
    EnumConstant(const Name& name, const IntegerValue& value);

    /**
     * Visits the EnumConstant.
     *
     * @see Node::visit()
     */
    virtual void* visit(Visitor* v);

    /**
     * Returns the enum constant's integer value.
     *
     * @return  The enum constant's value.
     */
    const IntegerValue& value() const;

private:
    IntegerValue mValue;
};

} // namespace ast
} // namespace soyac

#endif
