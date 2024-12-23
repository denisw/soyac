/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include "Function.hpp"
#include "FunctionType.hpp"
#include "Visitor.hpp"
#include <cassert>
#include <list>
#include <sstream>

namespace soyac {
namespace ast {

Function::Function(const Name& name, Type* returnType, Block* body)
    : DeclaredEntity(name)
    , mReturnType(returnType)
    , mBody(body)
{
    assert(returnType != NULL);
}

Function::Function(
    const Name& name, Type* returnType, FunctionParameter* param, Block* body)
    : DeclaredEntity(name)
    , mReturnType(returnType)
    , mBody(body)
{
    assert(returnType != NULL);
    assert(param != NULL);
    mParameters.push_back(param);
}

void* Function::visit(Visitor* v) { return v->visitFunction(this); }

std::string Function::str() const
{
    std::stringstream s;
    s << qualifiedName() << "(";

    if (mParameters.size() > 0) {
        Function::parameters_iterator it = parameters_begin();
        s << (*it)->type();
        it++;

        for (; it != parameters_end(); it++) {
            s << ", " << (*it)->type();
        }
    }

    s << ")";
    return s.str();
}

Type* Function::returnType() const { return mReturnType.target(); }

Function::parameters_iterator Function::parameters_begin() const
{
    return mParameters.begin();
}

Function::parameters_iterator Function::parameters_end() const
{
    return mParameters.end();
}

Block* Function::body() const { return mBody.target(); }

FunctionType* Function::type() const
{
    std::list<Type*> paramTypes;

    for (parameters_iterator it = parameters_begin(); it != parameters_end();
        it++) {
        paramTypes.push_back((*it)->type());
    }

    return FunctionType::get(
        returnType(), paramTypes.begin(), paramTypes.end());
}

} // namespace ast
} // namespace soyac
