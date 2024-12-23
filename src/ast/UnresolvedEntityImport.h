/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef SOYA_UNRESOLVED_ENTITY_IMPORT_H_
#define SOYA_UNRESOLVED_ENTITY_IMPORT_H_

#include "Import.h"
#include "Name.h"

namespace soyac {
namespace ast {

/**
 * Represents an import of a to-be-determined entity.
 */
class UnresolvedEntityImport : public Import {
public:
    /**
     * Creates a UnresolvedEntityImport.
     *
     * @param importedName  The imported entity's name.
     */
    UnresolvedEntityImport(const Name& importedName);

    /**
     * Visits the UnresolvedEntityImport.
     *
     * @see Node::visit()
     */
    virtual void* visit(Visitor* v);

    /**
     * Returns the name of the imported entity.
     *
     * @return  The imported entity's name.
     */
    const Name& importedName() const;

private:
    Name mImportedName;
};

} // namespace ast
} // namespace soyac

#endif
