/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef SOYA_CONSTRUCTOR_H_
#define SOYA_CONSTRUCTOR_H_

#include "ConstructorInitializer.h"
#include "Function.h"
#include "VoidType.h"
#include <cassert>

/**
 * The name given to every Constructor.
 *
 * @see DeclaredEntity::name()
 */
#define CONSTRUCTOR_NAME "(constructor)"

namespace soyac {
namespace ast {

class Constructor : public Function {
public:
    /**
     * Creates a Constructor with parameters.
     *
     * @param initializer       The constructor initializer.
     * @param parameters_begin  Start iterator for the constructor's
     * parameters.
     * @param parameters_end    End iterator for the constructor's
     * parameters.
     * @param body              The constructor's body.
     */
    template <class InputIterator>
    Constructor(ConstructorInitializer* initializer,
        InputIterator parameters_begin, InputIterator parameters_end,
        Block* body)
        : Function(CONSTRUCTOR_NAME, TYPE_VOID, parameters_begin,
              parameters_end, body)
        , mInitializer(initializer)
    {
        assert(initializer != nullptr);
        assert(body != nullptr);
    }

    /**
     * Creates a Constructor.
     *
     * @param body  The constructor's body.
     */
    Constructor(ConstructorInitializer* initializer, Block* body);

    /**
     * Visits the Constructor.
     *
     * @see Node::visit()
     */
    virtual void* visit(Visitor* v);

    /**
     * Returns the costructor's initializer.
     *
     * @return  The constructor intializer.
     */
    ConstructorInitializer* initializer() const;

private:
    Link<ConstructorInitializer> mInitializer;
};

} // namespace ast
} // namespace soyac

#endif
