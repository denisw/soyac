/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include "ArrayCreationExpression.h"
#include "ArrayType.h"
#include "UnknownArrayType.h"
#include "Visitor.h"

namespace soyac {
namespace ast {

ArrayCreationExpression::ArrayCreationExpression(
    ArrayType* type, Expression* lengthExpression)
    : mType(type)
    , mLengthExpression(lengthExpression)
{
    assert(type != nullptr);
    assert(dynamic_cast<ArrayType*>(type) != nullptr
        || dynamic_cast<UnknownArrayType*>(type) != nullptr);
    assert(lengthExpression != nullptr);
}

void* ArrayCreationExpression::visit(Visitor* v)
{
    return v->visitArrayCreationExpression(this);
}

Type* ArrayCreationExpression::type() const { return mType.target(); }

Expression* ArrayCreationExpression::lengthExpression() const
{
    return mLengthExpression.target();
}

ArrayCreationExpression::elements_iterator
ArrayCreationExpression::elements_begin() const
{
    return mElements.begin();
}

ArrayCreationExpression::elements_iterator
ArrayCreationExpression::elements_end() const
{
    return mElements.end();
}

} // namespace ast
} // namespace soyac
