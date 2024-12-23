/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include "LLValueExpression.h"
#include "CodeGenerator.h"
#include <cassert>

namespace soyac {
namespace codegen {

LLValueExpression::LLValueExpression(Type* type, llvm::Value* value)
    : mType(type)
    , mValue(value)
{
    assert(type != nullptr);
    assert(value != nullptr);
}

void* LLValueExpression::visit(Visitor* v)
{
    return ((CodeGenerator*)v)->visitLLValueExpression(this);
}

Type* LLValueExpression::type() const { return mType.target(); }

llvm::Value* LLValueExpression::value() const { return mValue; }

} // namespace codegen
} // namespace soyac
