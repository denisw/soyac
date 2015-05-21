/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include <cassert>
#include "ModuleImport.hpp"
#include "Visitor.hpp"

namespace soyac {
namespace ast
{


ModuleImport::ModuleImport(Module* imported, bool isDirect)
    : mImported(imported),
      mIsDirect(isDirect)
{
    assert (imported != NULL);
}


void*
ModuleImport::visit(Visitor* v)
{
    return v->visitModuleImport(this);
}


Module*
ModuleImport::imported() const
{
    return mImported.target();
}


bool
ModuleImport::isDirect() const
{
    return mIsDirect;
}


}}
