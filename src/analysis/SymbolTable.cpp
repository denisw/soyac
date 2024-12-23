/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include "SymbolTable.hpp"
#include "FunctionGroup.hpp"
#include <ast/Function.hpp>

namespace soyac {
namespace analysis {

///// Scope
///////////////////////////////////////////////////////////////////////

/**
 * Represents a scope.
 */
class Scope {
public:
    /**
     * Creates a scope.
     *
     * @param anonymous  Whether the scope is anonymous.
     */
    Scope(bool anonymous)
        : mIsAnonymous(anonymous)
    {
    }

    /**
     * Scope destructor.
     */
    ~Scope()
    {
        for (std::map<std::string, Link<NamedEntity>*>::iterator it
            = mMembers.begin();
            it != mMembers.end(); it++) {
            delete it->second;
        }
    }

    /**
     * Returns @c true if the scope is anonymous.
     *
     * @return  @c true if the scope is anonymous;
     *          @c false otherwise.
     */
    bool isAnonymous() const { return mIsAnonymous; }

    /**
     * Looks for a named entity with the passed name in the scope.
     *
     * @see SymbolTable::lookup()
     */
    NamedEntity* lookup(const std::string& name) const
    {
        std::map<std::string, Link<NamedEntity>*>::const_iterator it
            = mMembers.find(name);

        if (it == mMembers.end()) {
            return NULL;
        } else {
            return it->second->target();
        }
    }

    /**
     * Adds a named entity to the scope, and returns @c true on success.
     *
     * @see SymbolTable::add()
     */
    bool add(NamedEntity* entity)
    {
        NamedEntity* lookupResult = lookup(entity->name().str());

        if (lookupResult != NULL) {
            FunctionGroup* group;
            Function* func;

            /*
             * If the scope already has members with the passed entity's
             * name, this doesn't necessarily mean there is a conflict,
             * provided the entity is a function. In this case, there is
             * only a conflict if the existing member is not a function;
             * otherwise, both functions may coexist in the same scope if
             * they have non-identical signatures.
             */
            if ((group = dynamic_cast<FunctionGroup*>(lookupResult)) != NULL
                && (func = dynamic_cast<Function*>(entity)) != NULL) {
                for (FunctionGroup::overloads_iterator it
                    = group->overloads_begin();
                    it != group->overloads_end(); it++) {
                    if (func->type() == (*it)->type()) {
                        return false;
                    }
                }

                group->addOverload(func);
                return true;
            } else {
                return false;
            }
        } else {
            Link<NamedEntity>* link = new Link<NamedEntity>;
            Function* func;

            if ((func = dynamic_cast<Function*>(entity)) != NULL) {
                link->setTarget(new FunctionGroup(&func, &func + 1));
            } else {
                link->setTarget(entity);
            }

            mMembers[entity->name().str()] = link;
            return true;
        }
    }

    /**
     * Removes a named entity from the scope.
     *
     * @param entity  The named entity to remove.
     */
    void remove(NamedEntity* entity)
    {
        std::map<std::string, Link<NamedEntity>*>::iterator it
            = mMembers.find(entity->name().str());

        if (it != mMembers.end()) {
            delete it->second;
            mMembers.erase(it);
        }
    }

private:
    bool mIsAnonymous;
    std::map<std::string, Link<NamedEntity>*> mMembers;
};

///// SymbolTable
/////////////////////////////////////////////////////////////////

std::map<Module*, SymbolTable*> SymbolTable::sInstances;

SymbolTable::SymbolTable()
{
    /*
     * Add a first Scope instance to the scope stack on initialization.
     * This Scope represents the module-wide scope.
     */
    mScopeStack.push_back(new Scope(false));
}

SymbolTable::~SymbolTable()
{
    while (mScopeStack.size() > 1) {
        leaveScope();
    }

    for (std::map<NamedEntity*, Scope*>::iterator it = mEntityScopes.begin();
        it != mEntityScopes.end(); it++) {
        delete (*it).second;
    }

    for (std::map<Module*, SymbolTable*>::iterator it = sInstances.begin();
        it != sInstances.end(); it++) {
        if (it->second == this) {
            it->second = NULL;
        }
    }
}

SymbolTable* SymbolTable::get(Module* module, bool create)
{
    SymbolTable* table = sInstances[module];

    if (table == NULL && create) {
        table = new SymbolTable;
        sInstances[module] = table;
    }

    return table;
}

void SymbolTable::enterScope() { mScopeStack.push_back(new Scope(true)); }

void SymbolTable::enterScope(NamedEntity* entity)
{
    Scope* s = mEntityScopes[entity];

    if (s == NULL) {
        s = mEntityScopes[entity] = new Scope(false);
    }

    mScopeStack.push_back(s);
}

void SymbolTable::leaveScope()
{
    if (mScopeStack.size() > 1) {
        Scope* current = mScopeStack.back();

        if (current->isAnonymous()) {
            delete current;
        }

        mScopeStack.pop_back();
    }
}

bool SymbolTable::add(NamedEntity* entity)
{
    return mScopeStack.back()->add(entity);
}

bool SymbolTable::addGlobal(NamedEntity* entity)
{
    return mScopeStack.front()->add(entity);
}

void SymbolTable::remove(NamedEntity* entity)
{
    mScopeStack.back()->remove(entity);
}

NamedEntity* SymbolTable::lookup(const Name& identifier) const
{
    assert(identifier.isSimple());

    for (std::list<Scope*>::const_reverse_iterator it = mScopeStack.rbegin();
        it != mScopeStack.rend(); it++) {
        NamedEntity* lookupResult = (*it)->lookup(identifier.str());

        if (lookupResult != NULL) {
            return lookupResult;
        }
    }

    return NULL;
}

NamedEntity* SymbolTable::lookup(
    const Name& identifier, NamedEntity* scope) const
{
    assert(identifier.isSimple());

    std::map<NamedEntity*, Scope*>::const_iterator it
        = mEntityScopes.find(scope);

    if (it == mEntityScopes.end()) {
        return NULL;
    } else {
        return it->second->lookup(identifier.str());
    }
}

} // namespace analysis
} // namespace soyac
