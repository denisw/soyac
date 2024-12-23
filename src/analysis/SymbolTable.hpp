/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _SYMBOL_TABLE_HPP
#define _SYMBOL_TABLE_HPP

#include <ast/Module.hpp>
#include <ast/Name.hpp>
#include <ast/NamedEntity.hpp>
#include <ast/NodeList.hpp>
#include <list>
#include <map>
#include <string>

using namespace soyac::ast;

namespace soyac {
namespace analysis {

/**
 * Forward declare the private Scope class, which is declared and defined
 * in 'SymbolTable.hpp'.
 */
class Scope;

/**
 * An implementation of a symbol table, which is used for name resolution to
 * query for modules and declared entites by name.
 */
class SymbolTable {
public:
    /**
     * SymbolTable destructor.
     */
    ~SymbolTable();

    /**
     * Returns the SymbolTable for the passed module if it exists. If the
     * requested symbol table does not exist, and the @i create argument
     * is @c true, a new SymbolTable for the module is created and returned;
     * if @i create is @c false, the return value is null in this case.
     *
     * @param module  The module whose symbol table should be returned.
     * @param create  Whether to create the requested symbol table if it
     *                doesn't exist yet.
     * @return        The requested symbol table, or null.
     */
    static SymbolTable* get(Module* module, bool create = false);

    /**
     * Enters a new anonymous scope.
     */
    void enterScope();

    /**
     * Enters a new scope associated with the passed named entity.
     *
     * @param entity  The named entity.
     */
    void enterScope(NamedEntity* entity);

    /**
     * Leaves the last-entered scope.
     */
    void leaveScope();

    /**
     * Adds a named entity to the current scope, and returns @c true on
     * success. If the entity's name conflicts with any other entity in
     * the scope, nothing is done, and @c false is returned instead.
     *
     * @param entity  The entity to add.
     * @return        @c true if the entity was added successfully;
     *                @c false otherwise.
     */
    bool add(NamedEntity* entity);

    /**
     * Adds a named entity to the outermost scope, which is the symbol
     * table's "global scope". The global scope cannot be left with
     * leaveScope() and thus is persistent.
     *
     * @see  add()
     */
    bool addGlobal(NamedEntity* entity);

    /**
     * Removes an entity from the current scope.
     *
     * @param entity  The entity to remove.
     */
    void remove(NamedEntity* entity);

    /**
     * Looks up the passed identifier in the current scope, and returns the
     * matching entity if one is found. Otherwise, null is returned.
     *
     * If the lookup result is one or more functions (due to overloading,
     * multiple functions may have the same name), a FunctionGroup is
     * returned.
     *
     * @param identifier  The identifier to look up.
     * @return            The matching named entity, or null.
     */
    NamedEntity* lookup(const Name& identifier) const;

    /**
     * Looks up the passed identifier in the scope associated with the
     * specified entity (see enterScope(DeclaredEntity*)).
     *
     * @see lookup(const std::string&)
     *
     * @param identifier  The identifier to look up.
     * @param scope       The named entity in whose scope the identifier
     *                    should be looked up.
     * @return            The matching entities, or null.
     */
    NamedEntity* lookup(const Name& identifier, NamedEntity* scope) const;

private:
    static std::map<Module*, SymbolTable*> sInstances;

    std::map<NamedEntity*, Scope*> mEntityScopes;
    std::list<Scope*> mScopeStack;

    /**
     * SymbolTable constructor.
     */
    SymbolTable();
};

} // namespace analysis
} // namespace soyac

#endif
