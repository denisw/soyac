/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef SOYA_USER_DEFINED_TYPE_H_
#define SOYA_USER_DEFINED_TYPE_H_

#include "DeclarationBlock.h"
#include "Type.h"

namespace soyac {
namespace ast {

/**
 * Inherited by node classes representing user-defined types, that is,
 * structs, classes or interfaces.
 */
class UserDefinedType : public Type {
public:
    /**
     * Creates a UserDefinedType.
     *
     * @param name  The type's name.
     * @param body  The type declaration's body.
     */
    UserDefinedType(const Name& name, DeclarationBlock* body);

    /**
     * Returns the type declaration's body.
     *
     * @return  The declaration body.
     */
    DeclarationBlock* body() const;

private:
    Link<DeclarationBlock> mBody;

    /**
     * Called when the declaration body's statement list changes.
     *
     * @see  NodeList::elementChanged()
     */
    void onBodyChanged(
        DeclarationStatement* oldDecl, DeclarationStatement* newDecl);
};

} // namespace ast
} // namespace soyac

#endif
