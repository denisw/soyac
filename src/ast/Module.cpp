/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include "Module.hpp"
#include "DeclarationStatement.hpp"
#include "Visitor.hpp"

namespace soyac {
namespace ast
{

std::map<std::string, Module*> Module::sInstances;


Module::Module(const Name& name)
    : NamedEntity(name)
{
    /*
     * Call ref() on the built-in Block node so that it won't be deleted
     * if ref()'d and then unref()'d somewhere.
     */
    mBody.ref();

    mBody.statementListChanged().connect(
      sigc::mem_fun(this, &Module::onBodyChanged));
}


Module*
Module::get(const Name& name, bool create)
{
    Module* m = sInstances[name.str()];

    if (m == NULL && create == true)
    {
        m = sInstances[name.str()] = new Module(name);
        m->ref();
    }

    return m;
}


Module*
Module::getProgram(bool create)
{
    return Module::get("__program__", create);
}


void*
Module::visit(Visitor* v)
{
    return v->visitModule(this);
}


Module::imports_iterator
Module::imports_begin() const
{
    return mImports.begin();
}


Module::imports_iterator
Module::imports_end() const
{
    return mImports.end();
}


void
Module::addImport(Import* imp)
{
    mImports.push_back(imp);
}


Block*
Module::body()
{
    return &mBody;
}


void
Module::onBodyChanged(Statement* oldStmt, Statement* newStmt)
{
    /*
     * Make sure that all entities declared directly in
     * the module's body are "childs" of the module. See
     * NamedEntity::addChild() for more information.
     */

    DeclarationStatement* oldDecl = dynamic_cast<DeclarationStatement*>(oldStmt);

    if (oldDecl != NULL)
        removeChild(oldDecl->declaredEntity());

    DeclarationStatement* newDecl = dynamic_cast<DeclarationStatement*>(newStmt);

    if (newDecl != NULL)
        addChild(newDecl->declaredEntity());
}


}}
