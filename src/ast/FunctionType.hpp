/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _FUNCTION_TYPE_HPP
#define _FUNCTION_TYPE_HPP

#include <map>
#include "BuiltInType.hpp"
#include "NodeList.hpp"

namespace soyac {
namespace ast
{

/**
 * Represents a function type.
 *
 * (See the Soya Language Reference,)
 */
class FunctionType : public BuiltInType
{
public:
    /**
     * Iterates over the function type's parameter types (constant).
     */
    typedef NodeList<Type>::const_iterator parameterTypes_iterator;

    /**
     * Returns a FunctionType. Passing the same arguments will
     * always yield the exact same FunctionType instance.
     *
     * @param returnType            The function type's return type.
     * @param parameterTypes_begin  Start iterator for the function type's
     *                              parameter types.
     * @param parameterTypes        End iterator for the function type's
     *                              parameter types.
     * @return                      The requested FunctionType.
     */
    template <class InputIterator>
    static FunctionType* get(Type* returnType,
                             InputIterator parameterTypes_begin,
                             InputIterator parameterTypes_end)
    {
        NodeList<Type>* parameterTypes =
          new NodeList<Type>(parameterTypes_begin, parameterTypes_end);

        return _get(returnType, parameterTypes);
    }

    /**
     * Visits the FunctionType.
     *
     * @see Node::visit()
     */
    virtual void* visit(Visitor* v);

    /**
     * Override of DeclaredEntity::str().
     */
    virtual std::string str() const;

    /**
     * Override of Type::isSubtypeOf().
     */
    virtual bool isSubtypeOf(Type* other) const;

    /**
     * Returns the function type's return type.
     *
     * @return  The return type.
     */
    Type* returnType() const;

    /**
     * Returns a constant iterator pointing to the first parameter type
     * of the function type.
     *
     * @return  The start iterator for the function type's parameter types.
     */
    parameterTypes_iterator parameterTypes_begin() const;

    /**
     * Returns a constant iterator pointing past the last parameter type
     * of the function type.
     *
     * @return  The end iterator for the function type's parameter types.
     */
    parameterTypes_iterator parameterTypes_end() const;

private:
    /**
     * Compares two function signatures. Needed as comparator for the
     * instance map.
     */
    class SignatureComparator
    {
    public:
        bool operator()(const std::pair<Type*, NodeList<Type>*>& sig1,
                        const std::pair<Type*, NodeList<Type>*>& sig2) const
        {
            if (sig1.first != sig2.first)
                return sig1.first < sig2.first;

            else if (sig1.second->size() != sig2.second->size())
                return sig1.second->size() < sig2.second->size();

            else
            {
                NodeList<Type>::const_iterator it1 = sig1.second->begin();
                NodeList<Type>::const_iterator it2 = sig2.second->begin();

                for (; it1 != sig1.second->end(); it1++, it2++)
                {
                    if (*it1 != *it2)
                        return *it1 < *it2;
                }
            }

            return false;
        }
    };

    static std::map<std::pair<Type*, NodeList<Type>*>,
                    FunctionType*, SignatureComparator> sInstances;

    Link<Type> mReturnType;
    NodeList<Type>* mParameterTypes;

    /**
     * Implementation of get().
     *
     * @param returnType      The function type's return type.
     * @param parameterTypes  The function type's parameter types.
     */
    static FunctionType* _get(Type* returnType,
                              NodeList<Type>* parameterTypes);

    /**
     * Creates a FunctionType.
     *
     * @param returnType      The function type's return type.
     * @param parameterTypes  The function type's parameter types.
     */
    FunctionType(Type* returnType, NodeList<Type>* parameterTypes);
};

}}

#endif
