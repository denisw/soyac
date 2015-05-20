/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _UNRESOLVED_MODULE_IMPORT_HPP
#define _UNRESOLVED_MODULE_IMPORT_HPP

#include "Import.hpp"
#include "Name.hpp"

namespace soyac {
namespace ast
{

/**
 * Represents an import of a to-be-determined module.
 */
class UnresolvedModuleImport : public Import
{
public:
    /**
     * Creates a UnresolvedModuleImport.
     *
     * @param importedName  The imported module's name.
     * @param direct        Whether the module's members are directly imported
     *                      into the importing module's namespace.
     */
    UnresolvedModuleImport(const Name& importedName, bool isDirect);

    /**
     * Visits the UnresolvedModuleImport.
     *
     * @see Node::visit()
     */
    virtual void* visit(Visitor* v);

    /**
     * Returns the name of the imported module.
     *
     * @return  The imported module's name.
     */
    const Name& importedName() const;

    /**
     * Returns @c true if the imported module's members are directly imported
     * into the importing module's namespace. Otherwise, @c false is returned.
     *
     * @return  @c true if the module is directly imported;
     *          @c false otherwise.
     */
    bool isDirect() const;

private:
    Name mImportedName;
    bool mIsDirect;
};

}}

#endif
