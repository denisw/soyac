/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include "Type.hpp"
#include "UnknownType.hpp"

namespace soyac {
namespace ast {

Type::Type(const Name& name)
    : DeclaredEntity(name)
{
}

bool Type::isConvertableTo(Type* other) const
{
    return isImplicitlyConvertableTo(other);
}

bool Type::isImplicitlyConvertableTo(Type* other) const
{
    /*
     * We also check for UknownType instances as we don't want them
     * to produce type mismatch errors.
     */
    return (this == other || this->isSubtypeOf(other)
        || dynamic_cast<UnknownType*>(other) != NULL);
}

bool Type::isSubtypeOf(Type* other) const
{
    return (dynamic_cast<UnknownType*>(other) != NULL);
}

} // namespace ast
} // namespace soyac
