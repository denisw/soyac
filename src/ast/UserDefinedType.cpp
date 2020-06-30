/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include <cassert>
#include "DeclarationStatement.hpp"
#include "UserDefinedType.hpp"
#include "Visitor.hpp"

namespace soyac {
namespace ast
{


UserDefinedType::UserDefinedType(const Name& name, DeclarationBlock* body)
    : Type(name),
      mBody(body)
{
    assert (body != NULL);

    body->declarationListChanged().connect(
      sigc::mem_fun(*this, &UserDefinedType::onBodyChanged));

    /*
     * As onBodyChanged() is not automatically called for declarations which
     * are already part of the body passed to the UserDefinedType, we need to
     * call it manually for each DeclarationStatement in the block.
     */
    for (DeclarationBlock::declarations_iterator it =
           body->declarations_begin();
         it != body->declarations_end(); it++)
    {
        onBodyChanged(NULL, *it);
    }

}


DeclarationBlock*
UserDefinedType::body() const
{
    return mBody.target();
}


void
UserDefinedType::onBodyChanged(DeclarationStatement* oldDecl,
                               DeclarationStatement* newDecl)
{
    /*
     * Make sure that all entities declared directly in
     * the type declaration's body know their new parent
     * (see NamedEntity::addChild()).
     */

    if (oldDecl != NULL)
        removeChild(oldDecl->declaredEntity());

    if (newDecl != NULL)
        addChild(newDecl->declaredEntity());
}


}}
