/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include "UnresolvedModuleImport.h"
#include "Visitor.h"

namespace soyac {
namespace ast {

UnresolvedModuleImport::UnresolvedModuleImport(
    const Name& importedName, bool isDirect)
    : mImportedName(importedName)
    , mIsDirect(isDirect)
{
    assert(importedName.isSimple());
}

void* UnresolvedModuleImport::visit(Visitor* v)
{
    return v->visitUnresolvedModuleImport(this);
}

const Name& UnresolvedModuleImport::importedName() const
{
    return mImportedName;
}

bool UnresolvedModuleImport::isDirect() const { return mIsDirect; }

} // namespace ast
} // namespace soyac
