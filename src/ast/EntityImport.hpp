/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _ENTITY_IMPORT_HPP
#define _ENTITY_IMPORT_HPP

#include "DeclaredEntity.hpp"
#include "Import.hpp"

namespace soyac {
namespace ast {

/**
 * Represents an import of a declared entity.
 */
class EntityImport : public Import {
public:
    /**
     * Creates a EntityImport.
     *
     * @param imported  The imported declared entity.
     */
    EntityImport(DeclaredEntity* entity);

    /**
     * Visits the EntityImport.
     *
     * @see Node::visit()
     */
    virtual void* visit(Visitor* v);

    /**
     * Returns the declared entity imported by the EntityImport.
     *
     * @return  The imported declared entity.
     */
    DeclaredEntity* imported() const;

private:
    Link<DeclaredEntity> mImported;
};

} // namespace ast
} // namespace soyac

#endif
