/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _DECLARATION_STATEMENT_HPP
#define _DECLARATION_STATEMENT_HPP

#include "Statement.hpp"
#include "DeclaredEntity.hpp"

namespace soyac {
namespace ast
{

/**
 * Represents a declaration statement.
 *
 * (See the Soya Language Reference,)
 */
class DeclarationStatement : public Statement
{
public:
    /**
     * Creates a DeclarationStatement.
     *
     * @param decl  The declared entity.
     */
    DeclarationStatement(DeclaredEntity* decl);

    /**
     * Visits the DeclarationStatement.
     *
     * @see Node::visit()
     */
    virtual void* visit(Visitor* v);

    /**
     * Returns the entity declared by the declaration statement.
     *
     * @return  The declared entity.
     */
    DeclaredEntity* declaredEntity() const;

private:
    Link<DeclaredEntity> mDeclaredEntity;
};

}}

#endif
