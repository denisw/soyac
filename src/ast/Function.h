/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef SOYA_FUNCTION_H_
#define SOYA_FUNCTION_H_

#include "Block.h"
#include "DeclaredEntity.h"
#include "FunctionParameter.h"
#include "Type.h"

namespace soyac {
namespace ast {

class FunctionType;

/**
 * Represents a function.
 *
 * (See the Soya Language Reference,)
 */
class Function : public DeclaredEntity {
public:
    /**
     * Iterates over the function's parameters (constant).
     */
    typedef NodeList<FunctionParameter>::const_iterator parameters_iterator;

    /**
     * Creates a Function.
     *
     * @param name              The function's name.
     * @param returnType        The function's return type.
     * @param parameters_begin  Start iterator for the function's
     * parameters.
     * @param parameters_end    End iterator for the function's parameters.
     * @param body              The function's body, or null.
     */
    template <class InputIterator>
    Function(const Name& name, Type* returnType, InputIterator parameters_begin,
        InputIterator parameters_end, Block* body = nullptr)
        : DeclaredEntity(name)
        , mReturnType(returnType)
        , mBody(body)
    {
        for (InputIterator it = parameters_begin; it != parameters_end; it++) {
            mParameters.push_back(*it);
        }
    }

    /**
     * Creates a Function without parameters.
     *
     * @param name        The function's name.
     * @param returnType  The function's return type.
     * @param body        The function's body, or null.
     */
    Function(const Name& name, Type* returnType, Block* body = nullptr);

    /**
     * Creates a Function with a single parameter.
     *
     * @param name        The function's name.
     * @param returnType  The function's return type.
     * @param param       The parameter.
     * @param body        The function's body, or null.
     */
    Function(const Name& name, Type* returnType, FunctionParameter* param,
        Block* body = nullptr);

    /**
     * Visits the Function.
     *
     * @see Node::visit()
     */
    virtual void* visit(Visitor* v);

    /**
     * Override of DeclaredEntity::str().
     */
    virtual std::string str() const;

    /**
     * Returns the function's return type.
     *
     * @return  The function's return type.
     */
    Type* returnType() const;

    /**
     * Returns a constant iterator pointing to the first parameter of
     * the function.
     *
     * @return  The start iterator for the function's parameters.
     */
    parameters_iterator parameters_begin() const;

    /**
     * Returns an iterator pointing past the last parameter of the function.
     *
     * @return  The end iterator for the function's parameters.
     */
    parameters_iterator parameters_end() const;

    /**
     * Returns the function's body if it has one. If not, null is returned.
     *
     * @return  The function's body, or null.
     */
    Block* body() const;

    /**
     * Returns the function type that corresponds to the function's
     * signature.
     *
     * @return  The function's type.
     */
    FunctionType* type() const;

private:
    Link<Type> mReturnType;
    NodeList<FunctionParameter> mParameters;
    Link<Block> mBody;
};

} // namespace ast
} // namespace soyac

#endif
