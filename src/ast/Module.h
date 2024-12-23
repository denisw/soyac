/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef SOYA_MODULE_H_
#define SOYA_MODULE_H_

#include "Block.h"
#include "Import.h"
#include "NamedEntity.h"
#include <map>
#include <string>

namespace soyac {
namespace ast {

/**
 * Represents a module.
 *
 * (See the Soya Language Reference, Chapter 10.)
 */
class Module : public NamedEntity {
public:
    /**
     * Iterates over the module's imports (constant).
     */
    typedef NodeList<Import>::const_iterator imports_iterator;

    /**
     * Returns the module with the passed name. If no such module exists,
     * and @c true is passed as second argument, a Module with the name
     * will be created and returned. Otherwise, null is returned.
     *
     * @param name    The name of the module to return.
     * @param create  Whether to create the requested Module if
     * non-existent.
     * @return        The requested Module, or null.
     */
    static Module* get(const Name& name, bool create = false);

    /*
     * Returns the program module if existant. If it does not exist yet,
     * @c true is passedas second argument, the program Module will be
     * created and returned. Otherwise, null is returned.
     *
     * @param create  Whether to create the program Module if non-existent.
     * @return        The program Module, or null.
     */
    static Module* getProgram(bool create = false);

    /**
     * Visits the Module.
     *
     * @see Node::visit()
     */
    virtual void* visit(Visitor* v);

    /**
     * Returns a constant iterator pointing to the first module/entity
     * imported into the module.
     *
     * @return  Start iterator for the module's imports.
     */
    imports_iterator imports_begin() const;

    /**
     * Returns a constant iterator pointing past the last module/entity
     * imported into the module.
     *
     * @return  End iterator for the module's imports.
     */
    imports_iterator imports_end() const;

    /**
     * Adds an import to the module.
     *
     * @param imp  The import to add.
     */
    void addImport(Import* imp);

    /**
     * Returns a Block node representing the module's body. Note that this
     * node is built into the Module and thus cannot be replaced with
     * Node::replaceWith().
     *
     * @return  The module's body.
     */
    Block* body();

private:
    static std::map<std::string, Module*> sInstances;
    NodeList<Import> mImports;
    Block mBody;

    /**
     * Creates a Module.
     *
     * @param name  The module's name.
     */
    Module(const Name& name);

    /**
     * Called when the module body's statement list changes.
     *
     * @see  NodeList::elementChanged()
     */
    void onBodyChanged(Statement* oldStmt, Statement* newStmt);
};

} // namespace ast
} // namespace soyac

#endif
