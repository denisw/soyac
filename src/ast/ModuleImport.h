/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef SOYA_MODULE_IMPORT_H_
#define SOYA_MODULE_IMPORT_H_

#include "Import.h"
#include "Module.h"

namespace soyac {
namespace ast {

/**
 * Represents an import of a module.
 */
class ModuleImport : public Import {
public:
    /**
     * Creates a ModuleImport.
     *
     * @param imported  The imported module.
     * @param direct    Whether the module's members are directly imported
     *                  into the importing module's namespace.
     */
    ModuleImport(Module* module, bool isDirect);

    /**
     * Visits the ModuleImport.
     *
     * @see Node::visit()
     */
    virtual void* visit(Visitor* v);

    /**
     * Returns the module imported by the ModuleImport.
     *
     * @return  The imported module.
     */
    Module* imported() const;

    /**
     * Returns @c true if the imported module's members are directly
     * imported into the importing module's namespace. Otherwise, @c false
     * is returned.
     *
     * @return  @c true if the module is directly imported;
     *          @c false otherwise.
     */
    bool isDirect() const;

private:
    Link<Module> mImported;
    bool mIsDirect;
};

} // namespace ast
} // namespace soyac

#endif
