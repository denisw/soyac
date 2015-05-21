/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include "ArrayCreationExpression.hpp"
#include "ArrayType.hpp"
#include "UnknownArrayType.hpp"
#include "Visitor.hpp"

namespace soyac {
namespace ast
{


ArrayCreationExpression::ArrayCreationExpression(ArrayType* type,
                                                 Expression* lengthExpression)
    : mType(type),
      mLengthExpression(lengthExpression)
{
    assert (type != NULL);
    assert (dynamic_cast<ArrayType*>(type) != NULL ||
            dynamic_cast<UnknownArrayType*>(type) != NULL);
    assert (lengthExpression != NULL);
}


void*
ArrayCreationExpression::visit(Visitor* v)
{
    return v->visitArrayCreationExpression(this);
}


Type*
ArrayCreationExpression::type() const
{
    return mType.target();
}


Expression*
ArrayCreationExpression::lengthExpression() const
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


}}
