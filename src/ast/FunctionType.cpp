/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include <sstream>
#include "FunctionType.hpp"
#include "Visitor.hpp"

namespace soyac {
namespace ast
{

std::map<std::pair<Type*, NodeList<Type>*>, FunctionType*,
         FunctionType::SignatureComparator>
  FunctionType::sInstances;


FunctionType*
FunctionType::_get(Type* returnType,
                   NodeList<Type>* parameterTypes)
{
    std::pair<Type*, NodeList<Type>*> signature(returnType, parameterTypes);
    FunctionType* ret = sInstances[signature];

    if (ret == NULL)
    {
        ret = new FunctionType(returnType, parameterTypes);
        ret->ref();
        sInstances[signature] = ret;
    }

    return ret;
}


FunctionType::FunctionType(Type* returnType,
                           NodeList<Type>* parameterTypes)
    : BuiltInType("(function)"),
      mReturnType(returnType),
      mParameterTypes(parameterTypes)
{
}

void*
FunctionType::visit(Visitor* v)
{
    return v->visitFunctionType(this);
}


std::string
FunctionType::str() const
{
    std::stringstream s;
    s << "function(";

    if (mParameterTypes->size() > 0)
    {
        parameterTypes_iterator it = parameterTypes_begin();
        s << *it;
        it++;

        for (; it != parameterTypes_end(); it++)
            s << ", " << *it;
    }

    s << ")";

    if (returnType() != NULL)
        s << " => " << returnType();

    return s.str();
}


bool
FunctionType::isSubtypeOf(Type* other) const
{
    if (Type::isSubtypeOf(other))
        return true;
    else if (dynamic_cast<FunctionType*>(other) == NULL)
        return false;

    FunctionType* ftype = (FunctionType*) other;

    if (returnType() != ftype->returnType() &&
        !returnType()->isSubtypeOf(ftype->returnType()))
    {
       return false;
    }

    parameterTypes_iterator it1 = this->parameterTypes_begin();
    parameterTypes_iterator it2 = ftype->parameterTypes_begin();

    for (; it1 != parameterTypes_end() && it2 != ftype->parameterTypes_end();
         it1++, it2++)
    {
        if (*it1 != *it2 && !((*it1)->isSubtypeOf(*it2)))
            return false;
    }

    if (it1 != parameterTypes_end() || it2 != ftype->parameterTypes_end())
        return false;

    return true;
}


Type*
FunctionType::returnType() const
{
    return mReturnType.target();
}


FunctionType::parameterTypes_iterator
FunctionType::parameterTypes_begin() const
{
    return mParameterTypes->begin();
}


FunctionType::parameterTypes_iterator
FunctionType::parameterTypes_end() const
{
    return mParameterTypes->end();
}


}}
