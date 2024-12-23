/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef SOYA_ENTITY_IMPORT_H_
#define SOYA_ENTITY_IMPORT_H_

#include "DeclaredEntity.h"
#include "Import.h"

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
