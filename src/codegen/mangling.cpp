/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include "mangling.h"

#include <string>

#include "ast/ast.h"

namespace soyac::codegen {

using namespace ast;

static std::string mangledSimpleName(std::string name)
{
    if (name == CONSTRUCTOR_NAME) {
        return "constructor";
    } else {
        return name;
    }
}

std::string mangledName(NamedEntity* entity)
{
    /*
     * If the entity has no qualified name, it is a local entity; in this
     * case, we simply use the unqualified name as mangled name.
     */
    if (entity->qualifiedName().isSimple()) {
        return mangledSimpleName(entity->name().str());
    }

    /*
     * Otherwise, we have to build a more complex mangled name. It will
     * have the following components:
     *
     * (1) A leading underscore ("_").
     *
     * (2) For each identifier of the module name (the first name component),
     *     a number that specifies the length of the identifier, plus the
     *     identifier itself. For instance, the module name "foo::bar::blubb"
     *     would become "3foo3bar5blubb". This part of the mangled name does
     *     not appear in the mangled names of the program module and the
     *     entities declared in it.
     *
     * The following components only applies to mangled names of declared
     * entities (not those of modules):
     *
     * (3) Another underscore.
     *
     * (4) For each following identifier of the name, the same as described
     *     in (2).
     *
     * The following component only applies to mangled names of functions:
     *
     * (5) The mangled name of every parameter type, in the order the
     *     parameters are declared.
     *
     * There are a few special cases:
     *
     * - The mangled name of the "bool" type is "_b".
     * - The mangled name of the "char" type is "_c".
     * - The mangled name of the "double" type is "_d".
     * - The mangled name of the "float" type is "_f".
     * - The mangled name of the "int" type is "_i".
     * - The mangled name of the "long" type is "_l".
     * - The mangled name of the "uint" type is "_u".
     * - The mangled name of the "ulong" type is "_ul".
     */
    Name name = entity->qualifiedName();
    std::stringstream result;

    result << "_";

    auto it = name.identifiers_begin();

    /*
     * Mangle the module identifier.
     */
    if (!Module::getProgram() || *it != Module::getProgram()->name().str()) {
        char* modname = new char[(*it).length() + 1];
        strcpy(modname, (*it).c_str());

        const char* mID = std::strtok(modname, ":");

        do {
            std::string sname = mangledSimpleName(mID);
            result << sname.length();
            result << sname;
        } while ((mID = std::strtok(0, ":")) != 0);

        delete[] modname;
    }

    ++it;

    /*
     * Mangle the remaining identifiers if the entity is not a module.
     */
    if (!dynamic_cast<Module*>(entity)) {
        result << '_';

        for (; it != name.identifiers_end(); ++it) {
            std::string sname = mangledSimpleName(*it);
            result << sname.length();
            result << sname;
        }
    }

    /*
     * If the entity is a function, mangle its parameters.
     */
    if (auto func = dynamic_cast<Function*>(entity)) {
        for (auto it = func->parameters_begin(); it != func->parameters_end();
            ++it) {
            Type* paramType = (*it)->type();

            if (paramType == TYPE_BOOL) {
                result << "_b";
            } else if (paramType == TYPE_CHAR) {
                result << "_c";
            } else if (paramType == TYPE_FLOAT) {
                result << "_f";
            } else if (paramType == TYPE_DOUBLE) {
                result << "_d";
            } else if (paramType == TYPE_LONG) {
                result << "_l";
            } else if (paramType == TYPE_ULONG) {
                result << "_ul";
            } else if (auto i = dynamic_cast<IntegerType*>(paramType)) {
                result << (i->isSigned() ? "_i" : "_u");

                if (paramType != TYPE_INT && paramType != TYPE_UINT) {
                    result << i->size();
                }
            }

            else {
                result << mangledName(paramType);
            }
        }
    }

    return result.str();
}

} // namespace soyac::codegen
