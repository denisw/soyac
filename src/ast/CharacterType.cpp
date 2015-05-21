/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include "CharacterType.hpp"
#include "BooleanType.hpp"
#include "Function.hpp"
#include "FunctionParameter.hpp"
#include "IntegerType.hpp"
#include "Visitor.hpp"

namespace soyac {
namespace ast
{

CharacterType* CharacterType::sInstance = NULL;


CharacterType::CharacterType()
    : BuiltInType("char")
{
    FunctionParameter* p = new FunctionParameter(Name("x"), this);

    Function* equals = new Function(Name("equals"), TYPE_BOOL, &p, &p + 1);
    addMember(equals);
}


CharacterType*
CharacterType::get()
{
    if (sInstance == NULL)
    {
        sInstance = new CharacterType;
        sInstance->ref();
    }

    return sInstance;
}


void*
CharacterType::visit(Visitor* v)
{
    return v->visitCharacterType(this);
}


bool
CharacterType::isConvertableTo(Type* other) const
{
    return (this == other || dynamic_cast<IntegerType*>(other) != NULL);
}


}}
