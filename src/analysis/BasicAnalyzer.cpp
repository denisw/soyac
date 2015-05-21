/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include <cassert>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <list>
#include <boost/format.hpp>

#include <ast/ast.hpp>
#include "BasicAnalyzer.hpp"
#include "FunctionGroup.hpp"
#include "ModulesRequiredException.hpp"

namespace soyac {
namespace analysis
{


BasicAnalyzer::BasicAnalyzer()
    : mEnclosingFunc(NULL),
      mEnclosingType(NULL),
      mLValue(false)
{
}


PassResult*
BasicAnalyzer::analyze(Module* m)
{
    return (PassResult*) visitModule(m);
}


///// Helper Methods ///////////////////////////////////////////////////////////


NamedEntity*
BasicAnalyzer::resolveName(const Name& name)
{
    NamedEntity* ret = mSymbolTable->lookup(name);

    /*
     * If the symbol table lookup does not yield a result, we look in
     * the symbol tables of the imported modules.
     */
    if (ret == NULL)
    {
        for (std::list<Module*>::iterator it = mImportedModules.begin();
             it != mImportedModules.end(); it++)
        {
            SymbolTable* table = SymbolTable::get(*it);
            assert (table != NULL);

            ret = table->lookup(name);

            if (ret != NULL)
                break;
        }
    }

    return ret;
}


NamedEntity*
BasicAnalyzer::resolveName(const Name& name, NamedEntity* entity)
{
    NamedEntity* ret = mSymbolTable->lookup(name, entity);

    /*
     * If the symbol table lookup did not yield any result, this might be
     * because have not added the requested entity to the table yet; some of
     * them, like members of built-in types, are added lazily when they are
     * first requested. If we are doing a lookup in a type with such members,
     * add those to the symbol table and try again.
     */
    if (ret == NULL)
    {
        if (dynamic_cast<BuiltInType*>(entity) != NULL)
        {
            BuiltInType* type = (BuiltInType*) entity;
            mSymbolTable->enterScope(entity);

            for (BuiltInType::members_iterator it = type->members_begin();
                 it != type->members_end(); it++)
            {
                mSymbolTable->add(*it);
            }

            mSymbolTable->leaveScope();
            ret = mSymbolTable->lookup(name, entity);
        }
        else if (dynamic_cast<ArrayType*>(entity) != NULL)
        {
            if (name == "getElement")
            {
                mSymbolTable->enterScope(entity);
                mSymbolTable->add(
                  ((ArrayType*) entity)->getElementMethod());
                mSymbolTable->leaveScope();
            }
            else if (name == "length")
            {
                mSymbolTable->enterScope(entity);
                mSymbolTable->add(
                  ((ArrayType*) entity)->lengthProperty());
                mSymbolTable->leaveScope();
            }
            else if (name == "setElement")
            {
                mSymbolTable->enterScope(entity);
                mSymbolTable->add(
                  ((ArrayType*) entity)->setElementMethod());
                mSymbolTable->leaveScope();
            }

            ret = mSymbolTable->lookup(name, entity);
        }
        else
        {
            /*
             * - Call parent() till we have the module
             * - table = SymbolTable::get(module)
             * - table->lookup(name, entity)
             */
            NamedEntity* module = entity;

            while (dynamic_cast<Module*>(module) == NULL)
            {
                assert (module != NULL);
                module = module->parent();
            }

            SymbolTable* table = SymbolTable::get((Module*) module);
            assert (table != NULL);

            if (entity == module)
                ret = table->lookup(name);
            else
                ret = table->lookup(name, entity);
        }
    }

    return ret;
}


bool
BasicAnalyzer::isVisible(DeclaredEntity* entity)
{
    /*
     * If the entity neither has a "private" nor a "protected" modifier,
     * it is visible in any case.
     */
    if (!entity->hasModifier(DeclaredEntity::PRIVATE) &&
        !entity->hasModifier(DeclaredEntity::PROTECTED))
    {
        return true;
    }
    /*
     * If the entity is private or protected, it is still visible if
     * it is declared as a member of the module which we are processing,
     * or in a block. (It is assumed that block-local entities passed to
     * this method are recevied from resolveName() and thus are in the
     * current scope.)
     */
    else if (entity->parent() == NULL ||
             entity->parent() == mModule)
    {
        return true;
    }
    /*
     * Otherwise, the entity might be declared as a member of an
     * enclosing type, in which case it is visible, too.
     */
    else if (mEnclosingType != NULL)
    {
        NamedEntity* parent = mEnclosingType;

        while (parent != NULL)
        {
            if (parent == entity->parent())
                return true;
            else
                parent = parent->parent();
        }
    }

    /*
     * If none of the above is true, the entity is invisible.
     */
    return false;
}


void
BasicAnalyzer::checkVisible(DeclaredEntity* entity,
                            const Location& errorLoc)
{
    if (!isVisible(entity))
    {
        if (entity->hasModifier(DeclaredEntity::PRIVATE))
        {
            mRBuilder->addError(
              errorLoc,
              boost::format(
                "'%1%' is not visible from here.\n"
                "(It is private.)")
                 % entity);
        }
        else
        {
            mRBuilder->addError(
              errorLoc,
              boost::format(
                "'%1%' is not visible from here.\n"
                "(It is protected.)")
                 % entity);        
        }
    }
}


bool
BasicAnalyzer::canConvert(Expression* expr, Type* type, bool _explicit)
{
    bool convertable;

    if (_explicit)
        convertable = expr->type()->isConvertableTo(type);
    else
        convertable = expr->type()->isImplicitlyConvertableTo(type);

    /*
     * While values of the "int" type cannot be implicitly converted to
     * smaller integer types in the general case, this is possible in the
     * special case of integer literals - provided that the literal value
     * is in the destination type's range.
     */
    if (!convertable &&
        dynamic_cast<IntegerLiteral*>(expr) != NULL &&
        dynamic_cast<IntegerType*>(type) != NULL)
    {
        IntegerLiteral* literal = (IntegerLiteral*) expr;
        IntegerType* itype = (IntegerType*) type;

        if (literal->value() >= itype->min() &&
            literal->value() <= itype->max())
        {
            convertable = true;
        }
    }

    return convertable;
}


bool
BasicAnalyzer::convert(Expression* expr, Type* type, bool _explicit)
{
    if (canConvert(expr, type, _explicit))
    {
        /*
         * If the expression to convert is an UnresolvedFunctionExpression,
         * we just replace it with a FunctionExpression that denotes the
         * correct overload.
         */
        if (dynamic_cast<UnresolvedFunctionExpression*>(expr) != NULL ||
            dynamic_cast<UnresolvedInstanceFunctionExpression*>(expr) != NULL)
        {
            NodeList<Function>::const_iterator overloads_begin =
              (dynamic_cast<UnresolvedFunctionExpression*>(expr) != NULL) ?
              ((UnresolvedFunctionExpression*) expr)->overloads_begin() :
              ((UnresolvedInstanceFunctionExpression*) expr)->overloads_begin();

            NodeList<Function>::const_iterator overloads_end =
              (dynamic_cast<UnresolvedFunctionExpression*>(expr) != NULL) ?
              ((UnresolvedFunctionExpression*) expr)->overloads_end() :
              ((UnresolvedInstanceFunctionExpression*) expr)->overloads_end();

            Function* match = NULL;

            /*
             * First, look if we have an overload whose type is identical
             * to the destination type. If we do, that overload is the
             * right one.
             */
            for (NodeList<Function>::const_iterator it = overloads_begin;
                 it != overloads_end; it++)
            {
                if ((*it)->type() == type)
                    match = *it;
            }

            /*
             * If there is no exact match, look for overloads whose types
             * are convertable to the destination type. If there is more
             * than just one matching overload, the conversion is ambiguous,
             * and we need to report an error.
             */
            if (match == NULL)
            {
                for (NodeList<Function>::const_iterator it = overloads_begin;
                     it != overloads_end; it++)
                {
                    // TODO: Report error if multiple overloads match
                    bool funcConvertable;

                    if (_explicit)
                        funcConvertable = (*it)->type()->isConvertableTo(type);
                    else
                        funcConvertable =
                          (*it)->type()->isImplicitlyConvertableTo(type);

                    if (funcConvertable)
                    {
                        if (match != NULL)
                        {
                            mRBuilder->addError(
                              expr,
                              boost::format(
                                "Conversion of function group '%1%' to "
                                "type '%2%' is ambiguous.\n"
                                "(Multiple overloads can be converted to the "
                                "destination type.)")
                                 % match->name()
                                 % type);
                            return true;
                        }
                        else
                            match = *it;
                    }
                }
            }

            /*
             * At this point, 'match' should point to the correct overload.
             */
            assert (match != NULL);

            Expression* newExpr;

            if (dynamic_cast<UnresolvedInstanceFunctionExpression*>(expr) !=
                  NULL)
            {
                newExpr = new InstanceFunctionExpression(
                  ((UnresolvedInstanceFunctionExpression*) expr)->instance(),
                  match);
            }
            else
                newExpr = new FunctionExpression(match);

            expr->replaceWith(newExpr);
        }
        /*
         * Otherwise, if the destination type is different from the
         * expression's type, wrap the expression into a CastExpression.
         */
        else if (expr->type() != type)
        {
            /*
             * We cannot create a CastExpression with 'expr' as operand
             * and replace 'expr' with it - this would also replace the
             * CastExpression's operand, resulting in the CastExpression
             * being its own operand. Instead, we use a little trick here:
             * we create the CastExpression with a dummy operand, replace
             * 'expr' with the CastExpression, and then replace the dummy
             * expression with 'expr'. Because replacing 'expr' would result
             * in its deletion, we need to temporarily increment its reference
             * count before we call replaceWith().
             */

            BooleanLiteral* dummy = new BooleanLiteral(true);
            CastExpression* cast = new CastExpression(dummy, type);

            expr->ref();
            expr->replaceWith(cast);
            dummy->replaceWith(expr);
            expr->unref();
        }

        return true;
    }
    else
        return false;
}


Function*
BasicAnalyzer::bestMatch(FunctionGroup* overloads,
                         NodeList<Expression>::const_iterator arguments_begin,
                         NodeList<Expression>::const_iterator arguments_end,
                         const Location& errorLoc)
{
    std::list<Function*> matching;
    bool invisibleMatch = false;

    /*
     * Go through all function overloads, and add those compatible to the
     * argument list to the 'matching' list.
     */
    for (NodeList<Function>::const_iterator o = overloads->overloads_begin();
         o != overloads->overloads_end(); o++)
    {
        NodeList<Expression>::const_iterator a = arguments_begin;
        Function::parameters_iterator p = (*o)->parameters_begin();

        while (a != arguments_end && p != (*o)->parameters_end())
        {
            if (!canConvert(*a, (*p)->type()))
                break;

            a++;
            p++;
        }

        /*
         * If either the argument or the parameter iterator is not the end
         * iterator yet, this means that the number of arguments and
         * parameters differ; in this case, the function is not compatible.
         */
        if (a != arguments_end || p != (*o)->parameters_end())
            continue;
        else
        {
            /*
             * If the overload is not visible in the current scope, skip
             * it, but note that we found a matching invisible overload
             * for error reporting purposes.
             */
            if (!isVisible(*o))
                invisibleMatch = true;
            else
                matching.push_back(*o);
        }
    }

    /*
     * If no overload matches the argument list, report an error.
     */
    if (matching.size() == 0)
    {
        if (invisibleMatch)
        {
            mRBuilder->addError(
              errorLoc,
              boost::format(
                "The call to '%1%()' does not fit any visible overload of "
                "this function. (At least one private or protected overload "
                "is matching, though.)")
                 % (*overloads->overloads_begin())->name());
        
        }
        else
        {
            mRBuilder->addError(
              errorLoc,
              boost::format(
                "The call to '%1%()' does not fit any overload of this "
                "function.")
                 % (*overloads->overloads_begin())->name());
        }

        return NULL;
    }

    /*
     * Compare all overloads with each other using betterMatch() to find
     * the best-matching overload.
     */

    std::list<Function*>::iterator m = matching.begin();
    Function* bestMatching = *m;
    m++;

    for (; m != matching.end(); m++)
    {
        Function* better = betterMatch(*m, bestMatching,
                                       arguments_begin,
                                       arguments_end);

        /*
         * If betterMatch() returns NULL, the argument list is ambiguous;
         * we report this as error.
         */
        if (better == NULL)
        {
            mRBuilder->addError(
              errorLoc,
              boost::format(
                "The call to '%1%()' is ambiguous.\n"
                "(There is no single best-fitting overload.)")
                 % (*overloads->overloads_begin())->name());
            return NULL;
        }
        else if (better != bestMatching)
            bestMatching = better;
    }

    /*
     * If we have found a single best-matching overload, the last thing we
     * need to do is to convert all arguments to their corresponding
     * parameters' types using convert().
     */

    NodeList<Expression>::const_iterator a = arguments_begin;
    Function::parameters_iterator p = bestMatching->parameters_begin();

    for (; p != bestMatching->parameters_end(); p++, a++)
        convert(*a, (*p)->type());

    /*
     * Done.
     */
    return bestMatching;
}


Function*
BasicAnalyzer::betterMatch(Function* func1,
                           Function* func2,
                           NodeList<Expression>::const_iterator arguments_begin,
                           NodeList<Expression>::const_iterator arguments_end)
{
    /*
     * NOTE: We assume that both 'func1' and 'func2' have the right number
     * and types of arguments to be possibly called with the specified
     * arguments; this is checked by bestMatch() beforehand.
     */

    Function::parameters_iterator p1 = func1->parameters_begin();
    Function::parameters_iterator p2 = func2->parameters_begin();

    /*
     * Go through each argument along with the corresponding paramater of
     * both 'func1' and 'func2'.
     */
    for (CallExpression::arguments_iterator a = arguments_begin;
         a != arguments_end; a++, p1++, p2++)
    {
        /*
         * If the argument type is identical to the type of the one of the
         * function's corresponding parameters, that function is the better
         * match - as long as the other function's parameter type isn't
         * identical too. In the latter case, we have a "draw" situation
         * and thus need to check the next argument.
         */
        if ((*a)->type() == (*p1)->type())
        {
            if ((*p1)->type() == (*p2)->type())
                continue;
            else
                return func1;
        }
        else if ((*a)->type() == (*p2)->type())
        {
            if ((*p1)->type() == (*p2)->type())
                continue;
            else
                return func2;
        }

        /*
         * Integer Arguments
         */
        if (dynamic_cast<IntegerType*>((*a)->type()) != NULL)
        {
            IntegerType* a_itype = dynamic_cast<IntegerType*>((*a)->type());

            /*
             * If both function's parameter type is an integer type,
             * we act as follows: as we know that neither of the two
             * corresponding parameters has the exact same type (this
             * would have been handled by the code above), we determine
             * which is a better match by checking which parameter's type
             * is "nearer", that is, has the least-diverging size compared
             * to the argument type. Integer types with the same signedness
             * as the argument type are always considered to be "nearer"
             * than types where the signedness is different.
             */
            if (dynamic_cast<IntegerType*>((*p1)->type()) != NULL &&
                dynamic_cast<IntegerType*>((*p2)->type()) != NULL)
            {
                IntegerType* p1_itype = (IntegerType*) (*p1)->type();
                IntegerType* p2_itype = (IntegerType*) (*p2)->type();

                if (p1_itype->isSigned() != p2_itype->isSigned())
                {
                        if (a_itype->isSigned() == p1_itype->isSigned())
                        return func1;
                    else
                        return func2;
                }
                
                /*
                 * As we know that both parameter types's sizes are larger than
                 * the size of argument type (otherwise the types wouldn't be
                 * implicitly convertable), it suffices to check which type is
                 * smaller to determine which is "nearer" to the argument type.
                 */
                else if (p1_itype->size() < p2_itype->size())
                    return func1;
                else if (p2_itype->size() < p1_itype->size())
                    return func2;
                else
                    continue;
            }

            /*
             * If the parameter type of one function is an integer type,
             * and the other function's parameter is of a floating-point
             * type, the integer parameter is a better match.
             */
            else if (dynamic_cast<IntegerType*>((*p1)->type()) != NULL &&
                     dynamic_cast<FloatingPointType*>((*p2)->type()) != NULL)
            {
                return func1;
            }
            else if (dynamic_cast<IntegerType*>((*p2)->type()) != NULL &&
                     dynamic_cast<FloatingPointType*>((*p1)->type()) != NULL)
            {
                return func2;
            }

            /*
             * If both functions' parameter types are floating-point types,
             * "double" is always a better match than "float".
             */
            else if ((*p1)->type() == TYPE_DOUBLE &&
                     (*p2)->type() == TYPE_FLOAT)
            {
                return func1;
            }
            else if ((*p2)->type() == TYPE_DOUBLE &&
                     (*p1)->type() == TYPE_FLOAT)
            {
                return func2;
            }
        }
    }

    return NULL;
}


Expression*
BasicAnalyzer::createEntityExpression(NamedEntity* entity)
{
    Expression* expr;

    EnumConstant* econs;
    FunctionGroup* group;
    FunctionParameter* param;
    Module* mod;
    Type* type;
    Variable* var;

    /*
     * Enum Constants
     */
    if ((econs = dynamic_cast<EnumConstant*>(entity)) != NULL)
    {
        EnumType* etype = dynamic_cast<EnumType*>(econs->parent());
        assert (etype != NULL);

        expr = new EnumValueExpression(etype, econs);
    }

    /*
     * Functions
     */
    else if ((group = dynamic_cast<FunctionGroup*>(entity)) != NULL)
    {
        expr = new UnresolvedFunctionExpression(
          group->overloads_begin(),
          group->overloads_end());
    }

    /*
     * Function Parameters
     */
    else if ((param = dynamic_cast<FunctionParameter*>(entity)) != NULL)
        expr =  new FunctionParameterExpression(param);

    /*
     * Modules
     */
    else if ((mod = dynamic_cast<Module*>(entity)) != NULL)
        expr = new ModuleExpression(mod);

    /*
     * Types
     */
    else if ((type = dynamic_cast<Type*>(entity)) != NULL)
        expr = new TypeExpression(type);

    /*
     * Variables
     */
    else if ((var = dynamic_cast<Variable*>(entity)) != NULL)
        expr = new VariableExpression(var);

    /*
     * (This should never be reached.)
     */
    else
        assert (false);

    return expr;
}


Expression*
BasicAnalyzer::createMemberExpression(NamedEntity* member,
                                      Expression* instance)
{
    Expression* expr;

    FunctionGroup* group;
    Property* prop;
    Variable* var;

    /*
     * Instance Functions
     */
    if ((group = dynamic_cast<FunctionGroup*>(member)) != NULL)
    {
        expr = new UnresolvedInstanceFunctionExpression(
          instance,
          group->overloads_begin(),
          group->overloads_end());
    }

    /*
     * Instance Properties
     */
    else if ((prop = dynamic_cast<Property*>(member)) != NULL)
        expr = new UnresolvedInstancePropertyExpression(instance, prop);

    /*
     * Instance Variables
     */
    else if ((var = dynamic_cast<Variable*>(member)) != NULL)
        expr = new InstanceVariableExpression(instance, var);

    /*
     * (This should never be reached.)
     */
    else
        assert (false);

    return expr;
}


///// Module ///////////////////////////////////////////////////////////////////


void*
BasicAnalyzer::visitModule(Module* m)
{
    mModule = m;

    mSymbolTable = SymbolTable::get(m, true);
    mRBuilder = new PassResultBuilder;

    /*
     * The "__core__" module's members are implictly imported into all
     * modules (except the "__core__" module itself).
     */
    if (m->name() != "__core__")
        m->addImport(new UnresolvedModuleImport("__core__", true));

    /*
     * Visit the import statements, and collect all required but
     * non-existing (or not-yet-processed) modules.
     */

    std::list<std::string> requiredModules;

    for (Module::imports_iterator it = m->imports_begin();
         it != m->imports_end(); it++)
    {
        std::string* required = (std::string*) (*it)->visit(this);

        if (required != NULL)
        {
            requiredModules.push_back(*required);            
            delete required;
        }
    }

    /**
     * If we need to ask for any required modules, we throw a
     * ModulesRequiredException with this information.
     */
    if (!requiredModules.empty())
    {
        throw ModulesRequiredException(requiredModules);
    }

    m->body()->visit(this);

    PassResult* r = mRBuilder->result();
    delete mRBuilder;

    return r;
}


///// Imports //////////////////////////////////////////////////////////////////


void*
BasicAnalyzer::visitEntityImport(EntityImport* imp)
{
    mSymbolTable->add(imp->imported());

    Module* m = Module::get(imp->imported()->qualifiedName().first());
    assert (m != NULL);

    mSymbolTable->enterScope(m);
    mSymbolTable->add(imp->imported());
    mSymbolTable->leaveScope();

    return NULL;
}


void*
BasicAnalyzer::visitFunctionGroupImport(FunctionGroupImport* imp)
{
    Module* m = Module::get((*imp->overloads_begin())->qualifiedName().first());
    assert (m != NULL);

    for (FunctionGroupImport::overloads_iterator it = imp->overloads_begin();
         it != imp->overloads_end(); it++)
    {
        mSymbolTable->add(*it);

        mSymbolTable->enterScope(m);
        mSymbolTable->add(*it);
        mSymbolTable->leaveScope();        
    }

    return NULL;
}


void*
BasicAnalyzer::visitModuleImport(ModuleImport* imp)
{
    Module* m = imp->imported();
    mSymbolTable->add(m);

    if (imp->isDirect())
        mImportedModules.push_back(m);

    return NULL;
}


void*
BasicAnalyzer::visitUnresolvedEntityImport(UnresolvedEntityImport* imp)
{
    Module* m = Module::get(imp->importedName().first());

    if (m == NULL)
    {
        return new std::string(imp->importedName().first());
    }
    else
    {
        std::string declName = imp->importedName().last();
        NodeList<DeclaredEntity> entities;

        for (Block::statements_iterator it = m->body()->statements_begin();
             it != m->body()->statements_end(); it++)
        {
            if (dynamic_cast<DeclarationStatement*>(*it))
            {
                DeclarationStatement* dstmt = (DeclarationStatement*) *it;

                if (dstmt->declaredEntity()->name() == declName)
                    entities.push_back(dstmt->declaredEntity());
            }
        }

        if (entities.size() == 0)
        {
            mRBuilder->addError(
              imp,
              boost::format(
                "Module '%1%' has no member named '%2%'.")
                 % m->name()
                 % declName);
        }
        else if (entities.size() == 1)
        {
            Import* realImp = new EntityImport(*entities.begin());
            imp->replaceWith(realImp);
            realImp->visit(this);
        }
        else
        {
            for (NodeList<DeclaredEntity>::const_iterator it = entities.begin();
                 it != entities.end(); it++)
            {
                if (dynamic_cast<Function*>(*it) == NULL)
                    return NULL;
            }

            Import* realImp =
              new FunctionGroupImport(
                ((NodeList<Function>*) &entities)->begin(),
                ((NodeList<Function>*) &entities)->end());

            imp->replaceWith(realImp);
            realImp->visit(this);
        }

        return NULL;
    }
}


void*
BasicAnalyzer::visitUnresolvedModuleImport(UnresolvedModuleImport* imp)
{
    Module* m = Module::get(imp->importedName());

    if (m == NULL)
    {
        return new std::string(imp->importedName().str());
    }
    else
    {
        Import* realImp = new ModuleImport(m, imp->isDirect());

        imp->replaceWith(realImp);
        realImp->visit(this);

        return NULL;
    }
}


///// Statements ///////////////////////////////////////////////////////////////


void*
BasicAnalyzer::visitBlock(Block* b)
{
    mSymbolTable->enterScope();

    for (Block::statements_iterator it = b->statements_begin();
         it != b->statements_end(); it++)
    {
        (*it)->visit(this);
    }

    mSymbolTable->leaveScope();
    return NULL;
}


void*
BasicAnalyzer::visitDeclarationBlock(DeclarationBlock* b)
{
    bool hasConstructor = false;

    for (DeclarationBlock::declarations_iterator it = b->declarations_begin();
         it != b->declarations_end(); it++)
    {
        (*it)->visit(this);

        if (dynamic_cast<Constructor*>((*it)->declaredEntity()) != NULL)
            hasConstructor = true;
    }

    /*
     * If the declaration block doesn't include a constructor, we add an
     * empty default constructor.
     */
    if (!hasConstructor)
    {
        Constructor* defaultCons =
          new Constructor(new UnresolvedDefaultConstructorInitializer, new Block);

        DeclarationStatement* dstmt = new DeclarationStatement(defaultCons);

        b->addDeclaration(dstmt);
        dstmt->visit(this);
    }

    return NULL;
}


void*
BasicAnalyzer::visitDeclarationStatement(DeclarationStatement* stmt)
{
    DeclaredEntity* entity = stmt->declaredEntity();
    bool success;

     if (dynamic_cast<Module*>(entity->parent()) != NULL)
        success = mSymbolTable->addGlobal(entity);
     else
        success = mSymbolTable->add(entity);
 
    if (!success)
    {
        mRBuilder->addError(
          stmt,
          boost::format(
            "Another entity named '%1%' has already been declared in "
            "this scope.")
             % stmt->declaredEntity());    
    }

    if ((entity->hasModifier(DeclaredEntity::PUBLIC) &&
         (entity->hasModifier(DeclaredEntity::PRIVATE) ||
          entity->hasModifier(DeclaredEntity::PROTECTED))) ||
        (entity->hasModifier(DeclaredEntity::PRIVATE) &&
         entity->hasModifier(DeclaredEntity::PROTECTED)))
    {
        mRBuilder->addError(
          stmt,
          boost::format(
            "Declaration of '%1%' includes more than one visibility modifier.")
             % stmt->declaredEntity());
    }

    stmt->declaredEntity()->visit(this);
    return NULL;
}


void*
BasicAnalyzer::visitDoStatement(DoStatement* stmt)
{
    stmt->condition()->visit(this);

    mSymbolTable->enterScope();
    stmt->body()->visit(this);
    mSymbolTable->leaveScope();

    if (stmt->condition()->type() != TYPE_BOOL &&
        dynamic_cast<UnknownType*>(stmt->condition()->type()) == NULL)
    {
        mRBuilder->addError(
          stmt->condition(),
          boost::format(
            "The \"do\" loop condition must be an expression of type "
            "'bool', not '%1%'.")
             % stmt->condition()->type());
    }

    return NULL;
}


void*
BasicAnalyzer::visitExpressionStatement(ExpressionStatement* stmt)
{
    stmt->expression()->visit(this);
    return NULL;
}


void*
BasicAnalyzer::visitForStatement(ForStatement* stmt)
{
    mSymbolTable->enterScope();

    for (ForStatement::initializers_iterator it = stmt->initializers_begin();
         it != stmt->initializers_end(); it++)
    {
        (*it)->visit(this);
    }

    stmt->condition()->visit(this);

    if (stmt->condition()->type() != TYPE_BOOL &&
        dynamic_cast<UnknownType*>(stmt->condition()->type()) == NULL)
    {
        mRBuilder->addError(
          stmt->condition(),
          boost::format(
            "The \"for\" loop condition must be an expression of type "
            "'bool', not '%1%'.")
             % stmt->condition()->type());
    }

    for (ForStatement::iterators_iterator it = stmt->iterators_begin();
         it != stmt->iterators_end(); it++)
    {
        (*it)->visit(this);
    }

    stmt->body()->visit(this);
    mSymbolTable->leaveScope();

    return NULL;
}


void*
BasicAnalyzer::visitIfStatement(IfStatement* stmt)
{
    stmt->condition()->visit(this);

    mSymbolTable->enterScope();
    stmt->body()->visit(this);
    mSymbolTable->leaveScope();

    if (stmt->elseBody())
    {
        mSymbolTable->enterScope();
        stmt->elseBody()->visit(this);
        mSymbolTable->leaveScope();
    }
    
    if (stmt->condition()->type() != TYPE_BOOL &&
        dynamic_cast<UnknownType*>(stmt->condition()->type()) == NULL)
    {
        mRBuilder->addError(
          stmt->condition(),
          boost::format(
            "The \"if\" statement condition must be an expression of type "
            "'bool', not '%1%'.")
             % stmt->condition()->type());
    }

    return NULL;
}


void*
BasicAnalyzer::visitReturnStatement(ReturnStatement* stmt)
{
    if (mEnclosingFunc == NULL)
    {
        mRBuilder->addError(
          stmt,
          "\"return\" statements may not appear outside of a function.");
    }

    if (stmt->returnValue() != NULL)
    {
        stmt->returnValue()->visit(this);

        if (mEnclosingFunc->returnType() == TYPE_VOID)
        {
            mRBuilder->addError(
              stmt,
              "Cannot return a value in a function without return type.");
        }
        else if (mEnclosingFunc != NULL &&
                 !convert(stmt->returnValue(), mEnclosingFunc->returnType()))
        {
            mRBuilder->addError(
              stmt->returnValue(),
              boost::format(
                "Cannot return a value of type '%1%' in a function with "
                "incompatible return type '%2%'.")
                 % stmt->returnValue()->type()
                 % mEnclosingFunc->returnType());
        }
    }

    return NULL;
}


void*
BasicAnalyzer::visitWhileStatement(WhileStatement* stmt)
{
    stmt->condition()->visit(this);

    mSymbolTable->enterScope();
    stmt->body()->visit(this);
    mSymbolTable->leaveScope();

    if (stmt->condition()->type() != TYPE_BOOL &&
        dynamic_cast<UnknownType*>(stmt->condition()->type()) == NULL)
    {
        mRBuilder->addError(
          stmt->condition(),
          boost::format(
            "The \"while\" loop condition must be an expression of type "
            "'bool', not '%1%'.")
             % stmt->condition()->type());
    }

    return NULL;
}


///// Declared Entities ////////////////////////////////////////////////////////


void*
BasicAnalyzer::visitClassType(ClassType* type)
{
    if (dynamic_cast<UnknownType*>(type->baseClass()) != NULL)
    {
        /*
         * If this is the core Object class, it doesn't have a base class.
         */
        if (type->qualifiedName().str() == "__core__.Object")
            delete type->baseClass();
        else
            type->baseClass()->visit(this);
    }

    Type* tmp;
    tmp = mEnclosingType;
    mEnclosingType = type;

    mSymbolTable->enterScope(type);

    /*
     * Add the members of all base classes to the symbol table.
     */

    ClassType* base = dynamic_cast<ClassType*>(type->baseClass());

    while (base != NULL)
    {
        for (DeclarationBlock::declarations_iterator it =
               base->body()->declarations_begin();
             it != base->body()->declarations_end(); it++)
        {
            DeclaredEntity* member = (*it)->declaredEntity();

            if (!member->hasModifier(DeclaredEntity::PRIVATE) &&
                dynamic_cast<Constructor*>(member) == NULL &&
                dynamic_cast<Variable*>(member) == NULL)
            {
                mSymbolTable->add(member);
            }
        }

        base = dynamic_cast<ClassType*>(base->baseClass());    
    }

    /*
     * Visit all declarations in the class' body.
     */
    type->body()->visit(this);

    /*
     * For every non-private member variable, we need to create a wrapper
     * property by which the variable can be accessed from outside the
     * class (see section ? of the Soya Language Reference). We then add
     * it to the ClassType and replace the variable's symbol table entry
     * with it.
     */
    for (DeclarationBlock::declarations_iterator it =
           type->body()->declarations_begin();
         it != type->body()->declarations_end(); it++)
    {
        Variable* var = dynamic_cast<Variable*>((*it)->declaredEntity());

        if (var != NULL && !var->hasModifier(DeclaredEntity::PRIVATE))
        {
            mSymbolTable->remove(var);

            Expression* varExpr =
              new InstanceVariableExpression(new ThisExpression(type), var);

            Block* getBody = new Block;
            PropertyGetAccessor* getAcc = new PropertyGetAccessor(getBody);
            getBody->addStatement(new ReturnStatement(varExpr));

            Block* setBody = new Block;
            PropertySetAccessor* setAcc = new PropertySetAccessor(setBody);
            setBody->addStatement(new ExpressionStatement(
              new AssignmentExpression(
                varExpr,
                new FunctionParameterExpression(setAcc->valueParameter()))));

            Property* prop =
              new Property(var->name(), var->type(), getAcc, setAcc);

            DeclarationStatement* dstmt = new DeclarationStatement(prop);
            type->body()->addDeclaration(dstmt);

            mSymbolTable->add(prop);
        }
    }
    
    mSymbolTable->leaveScope();
    mEnclosingType = tmp;

    return NULL;
}


void*
BasicAnalyzer::visitConstructor(Constructor* cons)
{
    Function* tmp = mEnclosingFunc;
    mEnclosingFunc = cons;

    mSymbolTable->enterScope();

    for (Constructor::parameters_iterator it = cons->parameters_begin();
         it != cons->parameters_end(); it++)
    {
        (*it)->visit(this);
        mSymbolTable->add(*it);
    }

    cons->initializer()->visit(this);
    cons->body()->visit(this);

    mSymbolTable->leaveScope();
    mEnclosingFunc = tmp;

    return NULL;
}


void*
BasicAnalyzer::visitUnresolvedBaseConstructorInitializer(UnresolvedBaseConstructorInitializer* init)
{
    for (ConstructorInitializer::arguments_iterator it =
           init->arguments_begin();
         it != init->arguments_end(); it++)
    {
        (*it)->visit(this);
    }

    ClassType* type = dynamic_cast<ClassType*>(mEnclosingType);

    if (type == NULL)
    {
        mRBuilder->addError(
          init,
          "Cannot use \"super\" constructor initializer with a struct "
          "constructor.\n"
          "(Structs have no base type.)");
    }

    assert (type->baseClass() != NULL);

    FunctionGroup* constructors = dynamic_cast<FunctionGroup*>(
      resolveName(CONSTRUCTOR_NAME, type->baseClass()));

    assert (constructors != NULL);

    Function* match = bestMatch(
      constructors,
      init->arguments_begin(),
      init->arguments_end(),
      init->location());

    if (match != NULL)
    {
        Constructor* cons = dynamic_cast<Constructor*>(match);
        assert (cons != NULL);

        ConstructorInitializer* newInit = new ConstructorInitializer(
          cons,
          init->arguments_begin(),
          init->arguments_end());

        init->replaceWith(newInit);
        newInit->visit(this);
    }

    return NULL;
}


void*
BasicAnalyzer::visitUnresolvedConstructorInitializer(UnresolvedConstructorInitializer* init)
{
    for (ConstructorInitializer::arguments_iterator it =
           init->arguments_begin();
         it != init->arguments_end(); it++)
    {
        (*it)->visit(this);
    }

    UserDefinedType* type = dynamic_cast<UserDefinedType*>(mEnclosingType);
    assert (type != NULL);

    FunctionGroup* constructors = dynamic_cast<FunctionGroup*>(
      resolveName(CONSTRUCTOR_NAME, type));

    assert (constructors != NULL);

    Function* match = bestMatch(
      constructors,
      init->arguments_begin(),
      init->arguments_end(),
      init->location());

    if (match != NULL)
    {
        Constructor* cons = dynamic_cast<Constructor*>(match);
        assert (cons != NULL);

        ConstructorInitializer* newInit = new ConstructorInitializer(
          cons,
          init->arguments_begin(),
          init->arguments_end());

        init->replaceWith(newInit);
        newInit->visit(this);
    }

    return NULL;
}


void*
BasicAnalyzer::visitUnresolvedDefaultConstructorInitializer(UnresolvedDefaultConstructorInitializer* init)
{
    ClassType* cls = dynamic_cast<ClassType*>(mEnclosingType);

    /*
     * By default, a class constructor calls the default constructor of
     * its base class. An exception is the core Object class, which has
     * no base class.
     */
    if (cls != NULL && cls->baseClass() != NULL)
    {
        ConstructorInitializer* newInit =
          new UnresolvedBaseConstructorInitializer;

        init->replaceWith(newInit);
        newInit->visit(this);
    }
    /*
     * Struct and Object constructors do not have a default initializers.
     */
    else
        delete init;

    return NULL;
}


void*
BasicAnalyzer::visitEnumType(EnumType* type)
{
    mSymbolTable->enterScope(type);

    for (EnumType::values_iterator it = type->values_begin();
         it != type->values_end(); it++)
    {
        mSymbolTable->add(*it);
    }

    mSymbolTable->add(type->equalsMethod());
    mSymbolTable->leaveScope();

    return NULL;
}


void*
BasicAnalyzer::visitFunction(Function* func)
{
    if (dynamic_cast<UnknownType*>(func->returnType()) != NULL)
        func->returnType()->visit(this);

    for (Function::parameters_iterator it = func->parameters_begin();
         it != func->parameters_end(); it++)
    {
        (*it)->visit(this);
    }

    if (func->body() != NULL)
    {
        if (func->hasModifier(DeclaredEntity::EXTERN))
        {
            mRBuilder->addError(
              func,
              boost::format(
                "Function '%1%' is declared with a body although it is "
                "declared to be '\"extern\".")
                 % func);
        }

        Function* tmp = mEnclosingFunc;
        mEnclosingFunc = func;

        mSymbolTable->enterScope();

        for (Function::parameters_iterator it = func->parameters_begin();
            it != func->parameters_end(); it++)
        {
            mSymbolTable->add(*it);
        }

        func->body()->visit(this);
        mSymbolTable->leaveScope();

        mEnclosingFunc = tmp;
    }
    else if (func->body() == NULL && 
             !func->hasModifier(DeclaredEntity::EXTERN))
    {
        mRBuilder->addError(
          func,
          boost::format(
            "The declaration of function '%1%' misses a body. (Was it "
            "supposed to be \"extern\"?)")
             % func);
    }

    return NULL;
}


void*
BasicAnalyzer::visitFunctionParameter(FunctionParameter* param)
{
    if (dynamic_cast<UnknownType*>(param->type()) != NULL)
        param->type()->visit(this);

    return NULL;
}


void*
BasicAnalyzer::visitProperty(Property* prop)
{
    if (dynamic_cast<UnknownType*>(prop->type()) != NULL)
        prop->type()->visit(this);

    if (prop->getAccessor() != NULL)
        prop->getAccessor()->visit(this);

    if (prop->setAccessor() != NULL)
        prop->setAccessor()->visit(this);

    return NULL;
}


void*
BasicAnalyzer::visitPropertyGetAccessor(PropertyGetAccessor* acc)
{
    assert (acc->parent() != NULL);

    if (acc->body() != NULL)
    {
        if (acc->hasModifier(DeclaredEntity::EXTERN))
        {
            mRBuilder->addError(
              acc,
              boost::format(
                "Get accessor of property '%1%' has a body although the "
                "property is defined to be \"extern\".")
                 % acc->parent());
        }

        Function* tmp = mEnclosingFunc;
        mEnclosingFunc = acc;

        acc->body()->visit(this);

        mEnclosingFunc = tmp;
    }

    return NULL;
}


void*
BasicAnalyzer::visitPropertySetAccessor(PropertySetAccessor* acc)
{
    assert (acc->parent() != NULL);

    if (acc->body() != NULL)
    {
        if (acc->hasModifier(DeclaredEntity::EXTERN))
        {
            mRBuilder->addError(
              acc,
              boost::format(
                "Set accessor of property '%1%' has a body although the "
                "property is defined to be \"extern\".")
                 % acc->parent());
        }

        Function* tmp = mEnclosingFunc;
        mEnclosingFunc = acc;

        mSymbolTable->enterScope();
        mSymbolTable->add(acc->valueParameter());

        acc->body()->visit(this);

        mSymbolTable->leaveScope();
        mEnclosingFunc = tmp;
    }

    return NULL;
}


void*
BasicAnalyzer::visitStructType(StructType* type)
{
    Type* tmp;
    tmp = mEnclosingType;
    mEnclosingType = type;

    mSymbolTable->enterScope(type);
    type->body()->visit(this);
    mSymbolTable->leaveScope();

    mEnclosingType = tmp;
    return NULL;
}


void*
BasicAnalyzer::visitUnknownArrayType(UnknownArrayType* type)
{
    type->elementType()->visit(this);

    if (dynamic_cast<UnknownType*>(type->elementType()) == NULL)
        type->replaceWith(ArrayType::get(type->elementType()));

    return NULL;
}


void*
BasicAnalyzer::visitUnknownFunctionType(UnknownFunctionType* type)
{
    /*
     * Resolve the return type and the parameter types.
     */

    type->returnType()->visit(this);

    for (UnknownFunctionType::parameterTypes_iterator it =
           type->parameterTypes_begin();
         it != type->parameterTypes_end(); it++)
    {
        (*it)->visit(this);    
    }

    /*
     * If, after visiting, any types that is part of the function type
     * are still unknown, it makes no sense to create a corresponding
     * FunctionType instance. Return in this case.
     */

    if (dynamic_cast<UnknownType*>(type->returnType()) != NULL)
        return NULL;

    for (UnknownFunctionType::parameterTypes_iterator it =
           type->parameterTypes_begin();
         it != type->parameterTypes_end(); it++)
    {
        if (dynamic_cast<UnknownType*>(*it) != NULL)
            return NULL;
    }

    /*
     * Replace the UnknownFunctionType with the corresponding FunctionType
     * instance.
     */
    type->replaceWith(
      FunctionType::get(type->returnType(),
                        type->parameterTypes_begin(),
                        type->parameterTypes_end()));

    return NULL;
}


void*
BasicAnalyzer::visitUnknownType(UnknownType* type)
{
    /*
     * The UknownType singleton TYPE_UNKNOWN cannot be resolved.
     */
    if (type == TYPE_UNKNOWN)
        return NULL;

    NamedEntity* lookupResult;

    if (type->qualifiedName().isSimple())
        lookupResult = resolveName(type->qualifiedName());
    else
    {
        Name qname = type->qualifiedName();

        Name::identifiers_iterator it = qname.identifiers_begin();
        lookupResult = resolveName(*it);
        it++;

        for (; it != qname.identifiers_end(); it++)
        {
            if (lookupResult == NULL)
                break;

            lookupResult = resolveName(*it, lookupResult);   
        }
    }

    if (lookupResult == NULL)
    {
        mRBuilder->addError(
          type,
          boost::format(
            "Unknown type '%1%'.")
             % type->qualifiedName());
    }
    else
    {
        Type* newType = dynamic_cast<Type*>(lookupResult);

        if (newType == NULL)
        {
            mRBuilder->addError(
              type,
              boost::format(
                "'%1%' is not a type.")
                 % type->qualifiedName());        
        }
        else
            type->replaceWith(newType);
    }

    return NULL;
}


void*
BasicAnalyzer::visitVariable(Variable* var)
{
    if (dynamic_cast<UnknownType*>(var->type()) != NULL)
        var->type()->visit(this);

    if (var->type() == TYPE_VOID)
    {
        mRBuilder->addError(
          var,
          "'void' is no valid variable type.");

        return NULL;
    }

    if (var->initializer() != NULL)
    {
        var->initializer()->visit(this);

        if (!convert(var->initializer(), var->type()))
        {
            mRBuilder->addError(
              var,
              boost::format(
                "Cannot initialize variable of type '%1%' with a value of "
                "type '%2%'.")
                 % var->type()
                 % var->initializer()->type());
        }
        else if (var->initializer()->type() == TYPE_VOID)
        {
            mRBuilder->addError(
              var,
              boost::format(
                "Cannot initialize variable '%1%' with a value of type "
                "'void'.")
                 % var);
        }
    }
    else if (var->type() == TYPE_UNKNOWN)
    {
        mRBuilder->addError(
          var,
          boost::format(
            "The declaration of variable '%1%' is incomplete; a type "
            "and/or an initializer expression must be specified.")
             % var);
    }

    return NULL;
}


///// Expressions //////////////////////////////////////////////////////////////


void*
BasicAnalyzer::visitArrayCreationExpression(ArrayCreationExpression* expr)
{
    if (dynamic_cast<UnknownType*>(expr->type()) != NULL)
        expr->type()->visit(this);

    if (expr->lengthExpression() != NULL)
    {
        expr->lengthExpression()->visit(this);

        if (!convert(expr->lengthExpression(), TYPE_INT))
        {
            mRBuilder->addError(
              expr->lengthExpression(),
              boost::format(
                "The specified array length must be a value of type 'int', "
                "not '%2'.")
                 % expr->lengthExpression()->type());    
        }
    }

    for (ArrayCreationExpression::elements_iterator it =
           expr->elements_begin();
         it != expr->elements_end(); it++)
    {
        (*it)->visit(this);
    }

    // TODO: Check for non-matching length / number of passed elements
    return NULL;
}


void*
BasicAnalyzer::visitAssignmentExpression(AssignmentExpression* expr)
{
    /*
     * If the assignment's left-hand expression is an element access
     * expression, replace the AssignmentExpression node by a CallExpression
     * node representing the corresponding setElement() method.
     */
    if (dynamic_cast<UnresolvedElementAccessExpression*>(expr->leftHand()) != NULL)
    {
        UnresolvedElementAccessExpression* eacc =
          (UnresolvedElementAccessExpression*) expr->leftHand();

        Expression* callee =
          new UnresolvedMemberAccessExpression(eacc->operand(), "setElement");

        eacc->setLocation(expr->location());
        callee->setLocation(expr->location());

        NodeList<Expression> args;
        args.push_back(eacc->index());
        args.push_back(expr->rightHand());

        Expression* newExpr =
          new CallExpression(callee, args.begin(), args.end());

        expr->replaceWith(newExpr);
        newExpr->visit(this);
    }
    else
    {
        /*
         * If the assignment's left-hand expression is an unresolved
         * simple name or member access expression, it might denote a
         * property; in this case, the assignment expression needs to
         * be replaced with a corresponding call to the property's set
         * accessor.
         */
        if (dynamic_cast<UnresolvedSimpleNameExpression*>(expr->leftHand())
              != NULL ||
            dynamic_cast<UnresolvedMemberAccessExpression*>(expr->leftHand())
              != NULL)
        {
            /*
             * With setting the 'mLValue' member to true, we tell the
             * called visit method to not resolve a property-denoting
             * expression to that property's get accessor call, but to
             * leave it as an Unresolved(Instance)PropertyExpression.
             */
            mLValue = true;
            expr->leftHand()->visit(this);
            mLValue = false;
            
            if (dynamic_cast<UnresolvedInstancePropertyExpression*>(
                  expr->leftHand()) != NULL)
            {
                UnresolvedInstancePropertyExpression* pexpr =
                  (UnresolvedInstancePropertyExpression*) expr->leftHand();

                expr->rightHand()->visit(this);

                if (pexpr->target()->setAccessor() == NULL)
                {
                    mRBuilder->addError(
                      pexpr,
                      boost::format(
                        "Property '%1%' is read-only.")
                         % pexpr->target());                
                }
                else if (!convert(expr->rightHand(), pexpr->target()->type()))
                {
                    mRBuilder->addError(
                      pexpr,
                      boost::format(
                        "Cannot assign value of '%1%' to property of "
                        "type '%2%'.")
                         % expr->rightHand()->type()
                         % pexpr->target());                                
                }
                else
                {
                    checkVisible(pexpr->target(), expr->location());
                    checkVisible(
                      pexpr->target()->setAccessor(), expr->location());

                    Expression* callee =
                      new InstanceFunctionExpression(
                        pexpr->instance(),
                        pexpr->target()->setAccessor());

                    callee->setLocation(expr->location());

                    Expression* arg = expr->rightHand();                    
                    expr->replaceWith(
                      new CallExpression(callee, &arg, &arg + 1));
                }

                return NULL;
            }
        }
        /*
         * Otherwise, no special action is needed.
         * Simply visit the left-hand expression.
         */
        else
            expr->leftHand()->visit(this);

        expr->rightHand()->visit(this);

        if (!convert(expr->rightHand(), expr->leftHand()->type()))
        {
            mRBuilder->addError(
              expr,
              boost::format(
                "Cannot assign '%1%' to '%2%'.")
                 % expr->rightHand()->type()
                 % expr->leftHand()->type());
        }
    }

    return NULL;
}


void*
BasicAnalyzer::visitCallExpression(CallExpression* expr)
{
    expr->callee()->visit(this);

    for (CallExpression::arguments_iterator it = expr->arguments_begin();
         it != expr->arguments_end(); it++)
    {
        (*it)->visit(this);

        /*
         * If we have an unresolved expression in the argument list, it
         * doesn't make any sense to continue; overload resolution would
         * lead to bogus error messages.
         */
        if (dynamic_cast<UnknownType*>((*it)->type()) != NULL)
            return NULL;
    }

    /*
     * If the callee expression denotes a group of function overloads, we
     * need to find out which of the overloads matches the argument list
     * best (if any) and set the callee to the corresponing FunctionExpression.
     */
    if (dynamic_cast<UnresolvedFunctionExpression*>(expr->callee()) != NULL)
    {
        UnresolvedFunctionExpression* unresolved =
          (UnresolvedFunctionExpression*) expr->callee();

        FunctionGroup group(
          unresolved->overloads_begin(),
          unresolved->overloads_end());

        Function* bestMatching = bestMatch(&group,
                                           expr->arguments_begin(),
                                           expr->arguments_end(),
                                           expr->location());

        if (bestMatching != NULL)
            unresolved->replaceWith(new FunctionExpression(bestMatching));
        else
            return NULL;
    }
    /*
     * If the callee expression denotes a group of instance function overloads,
     * we do the same as above, with the exception that the callee will be an
     * InstanceFunctionExpression instead of a FunctionExpression.
     */
    else if (dynamic_cast<UnresolvedInstanceFunctionExpression*>(
               expr->callee()) != NULL)
    {
        UnresolvedInstanceFunctionExpression* unresolved =
          (UnresolvedInstanceFunctionExpression*) expr->callee();

        FunctionGroup group(
          unresolved->overloads_begin(),
          unresolved->overloads_end());

        Function* bestMatching = bestMatch(&group,
                                           expr->arguments_begin(),
                                           expr->arguments_end(),
                                           expr->location());

        if (bestMatching != NULL)
        {
            unresolved->replaceWith(
              new InstanceFunctionExpression(unresolved->instance(),
                                             bestMatching));
        }
        else
            return NULL;
    }
    /*
     * If the callee expression denotes a function reference, we need to
     * check if the passed arguments match the referenced functions's
     * signature.
     */
    else if (dynamic_cast<FunctionType*>(expr->callee()->type()) != NULL)
    {
        FunctionType* ftype = (FunctionType*) expr->callee()->type();

        FunctionType::parameterTypes_iterator p_it =
          ftype->parameterTypes_begin();    
        CallExpression::arguments_iterator a_it = expr->arguments_begin();

        for (; p_it != ftype->parameterTypes_end() &&
               a_it != expr->arguments_end(); p_it++, a_it++)
        {
            if (!convert(*a_it, *p_it))
            {
                mRBuilder->addError(
                  expr,
                  boost::format(
                    "Cannot convert expression of type '%1%' to '%2%'.")
                     % (*a_it)->type()
                     % *p_it);
            }
        }

        if (p_it != ftype->parameterTypes_end())
        {
            mRBuilder->addError(
              expr,
              boost::format(
                "Too few arguments for calling a function of type '%1%'.")
                 % ftype);
        }
        else if (a_it != expr->arguments_end())
        {
            mRBuilder->addError(
              expr,
              boost::format(
                "Too many arguments for calling a function of type '%1%'.")
                 % ftype);    
        }
    }
    /*
     * Otherwise, the callee expression does not denote a callable object.
     */
    else if (dynamic_cast<UnknownType*>(expr->callee()->type()) == NULL ||
             dynamic_cast<TypeExpression*>(expr->callee()) != NULL)
    {
        mRBuilder->addError(
          expr,
          boost::format(
            "Expression of type '%1%' is not callable.")
             % expr->callee()->type());
    }

    return NULL;
}

void*
BasicAnalyzer::visitCastExpression(CastExpression* expr)
{
    expr->operand()->visit(this);

    if (!canConvert(expr->operand(), expr->type(), true))
    {
        mRBuilder->addError(
          expr,
          boost::format(
            "Cannot convert expression of type '%1%' to '%2%'.")
             % expr->operand()->type()
             % expr->type());
    }

    return NULL;
}


void*
BasicAnalyzer::visitInstanceVariableExpression(InstanceVariableExpression* expr)
{
    checkVisible(expr->target(), expr->location());
    return NULL;
}


void*
BasicAnalyzer::visitLogicalExpression(LogicalExpression* expr)
{
    expr->leftHand()->visit(this);
    expr->rightHand()->visit(this);

    if (!convert(expr->leftHand(), TYPE_BOOL))
    {
        std::string op;

        if (expr->kind() == LogicalExpression::AND)
            op = "&&";
        else
            op = "||";

        mRBuilder->addError(
          expr,
          boost::format(
            "Cannot have expression of type '%1%' as the \"%2%\" operator's "
            "left hand.\n"
            "(An expression of type 'bool' is expected.)")
             % expr->leftHand()->type()
             % op);

        /*
         * Return after reporting the incorrect left-hand type, even if the
         * right-hand expression could have the wrong type too; one error
         * per logical expression is enough.
         */
        return NULL;
    }

    if (!convert(expr->rightHand(), TYPE_BOOL))
    {
        std::string op;

        if (expr->kind() == LogicalExpression::AND)
            op = "&&";
        else
            op = "||";

        mRBuilder->addError(
          expr,
          boost::format(
            "Cannot have expression of type '%1%' as the \"%2%\" operator's "
            "right hand.\n"
            "(An expression of type 'bool' is expected.)")
             % expr->rightHand()->type()
             % op);
    }

    return NULL;
}


void*
BasicAnalyzer::visitLogicalNotExpression(LogicalNotExpression* expr)
{
    expr->operand()->visit(this);

    if (!convert(expr->operand(), TYPE_BOOL))
    {
        mRBuilder->addError(
          expr,
          boost::format(
            "'%1%' is not allowed as the \"!\" operator's operand type.\n"
            "(An expression of type 'bool' is expected.)")
             % expr->operand()->type());
    }

    return NULL;
}


void*
BasicAnalyzer::visitTypeExpression(TypeExpression* expr)
{
    if (dynamic_cast<UnknownType*>(expr->target()) != NULL)
        expr->target()->visit(this);

    checkVisible(expr->target(), expr->location());
    return NULL;
}


void*
BasicAnalyzer::visitUnresolvedBinaryExpression(UnresolvedBinaryExpression* expr)
{
    /*
     * In Soya, all operator expressions are just short-hand notations for
     * calls to specific "operator methods" of the left-hand operand. For
     * instance, the expression 'a + b' is the same as 'a.plus(b)'. We
     * therefore replace every UnresolvedBinaryExpression with an equivalent
     * CallExpression here.
     */

    expr->leftHand()->visit(this);
    expr->rightHand()->visit(this);

    /*
     * If the left-hand operand's type is unknown, we cannot determine the
     * operator method to be called; just return in this case.
     */
    if (dynamic_cast<UnknownType*>(expr->leftHand()->type()) != NULL)
        return NULL;

    /*
     * If the UnresolvedBinaryExpression represents an "!=" operator
     * expression, we need to transform it into a negated "==" expression.
     * (There is no specific operator method for "!=").
     */
    if (expr->kind() == UnresolvedBinaryExpression::NE)
    {
        Expression* eq =
          new UnresolvedBinaryExpression(UnresolvedBinaryExpression::EQ,
                                         expr->leftHand(),
                                         expr->rightHand());
        eq->setLocation(expr->location());

        Expression* newExpr = new LogicalNotExpression(eq);

        expr->replaceWith(newExpr);
        newExpr->visit(this);

        return NULL;
    }
    /*
     * If the UnresolvedBinaryExpression represents an "<=" or ">=" operator
     * expression, the replacement needs to be a CombinedRelationalExpression
     * instead of a CallExpression.
     */
    else if (expr->kind() == UnresolvedBinaryExpression::LE ||
             expr->kind() == UnresolvedBinaryExpression::GE)
    {
        Expression* equalsExpr = new UnresolvedMemberAccessExpression(
          expr->leftHand(), "equals");
        Expression* comparisonExpr = new UnresolvedMemberAccessExpression(
          expr->leftHand(),
          expr->kind() == UnresolvedBinaryExpression::LE ?
          "lessThan" :
          "greaterThan");

        Expression* arg = expr->rightHand();
        
        CallExpression* equalsCall =
          new CallExpression(equalsExpr, &arg, &arg + 1);
        CallExpression* comparisonCall =
          new CallExpression(comparisonExpr, &arg, &arg + 1);

        equalsCall->setLocation(expr->location());
        equalsExpr->setLocation(expr->location());
        comparisonCall->setLocation(expr->location());
        comparisonExpr->setLocation(expr->location());
        
        equalsCall->visit(this);
        comparisonCall->visit(this);

        DeclaredEntity* equalsCallee;
        DeclaredEntity* comparisonCallee;

        if (dynamic_cast<InstanceFunctionExpression*>(equalsCall->callee()))
        {
            equalsCallee =
              ((InstanceFunctionExpression*) equalsCall->callee())->target();
        }
        else if (equalsCall->callee()->type() == TYPE_UNKNOWN)
            return NULL;
        else
            assert (false);

        if (dynamic_cast<InstanceFunctionExpression*>(comparisonCall->callee()))
        {
            comparisonCallee =
              ((InstanceFunctionExpression*) comparisonCall->callee())->target();
        }
        else if (comparisonCall->callee()->type() == TYPE_UNKNOWN)
            return NULL;
        else
            assert (false);

        Expression* newExpr =
          new CombinedRelationalExpression(expr->leftHand(),
                                           expr->rightHand(),
                                           equalsCallee,
                                           comparisonCallee);

        expr->replaceWith(newExpr);
        newExpr->visit(this);

        return NULL;
    }
    /*
     * Otherwise, just find the correct operator method, create a
     * corresponding expression calling the method, and replace the
     * UnresolvedBinaryExpression with it.
     */
    else
    {
        Name memberName("");
        bool isAssign = false;

        switch (expr->kind())
        {
            case UnresolvedBinaryExpression::PLUS_ASSIGN:
                isAssign = true;
            case UnresolvedBinaryExpression::PLUS:
                memberName = Name("plus");
                break;
            case UnresolvedBinaryExpression::MINUS_ASSIGN:
                isAssign = true;
            case UnresolvedBinaryExpression::MINUS:
                memberName = Name("minus");
                break;
            case UnresolvedBinaryExpression::MUL_ASSIGN:
                isAssign = true;
            case UnresolvedBinaryExpression::MUL:
                memberName = Name("mul");
                break;
            case UnresolvedBinaryExpression::DIV_ASSIGN:
                isAssign = true;
            case UnresolvedBinaryExpression::DIV:
                memberName = Name("div");
                break;
            case UnresolvedBinaryExpression::MOD_ASSIGN:
                isAssign = true;
            case UnresolvedBinaryExpression::MOD:
                memberName = Name("mod");
                break;
            case UnresolvedBinaryExpression::LSHIFT_ASSIGN:
                isAssign = true;
            case UnresolvedBinaryExpression::LSHIFT:
                memberName = Name("lshift");
                break;
            case UnresolvedBinaryExpression::RSHIFT_ASSIGN:
                isAssign = true;
            case UnresolvedBinaryExpression::RSHIFT:
                memberName = Name("rshift");
                break;
            case UnresolvedBinaryExpression::EQ:
                memberName = Name("equals");
                break;
            case UnresolvedBinaryExpression::LT:
                memberName = Name("lessThan");
                break;
            case UnresolvedBinaryExpression::GT:
                memberName = Name("greaterThan");
                break;
            default:
                assert (false);
        }

        /*
         * If the operator is a compound assignment operator, we need
         * to create a CompoundAssignmentExpression.
         */
        if (isAssign)
        {
            NamedEntity* lookupResult = resolveName(
              memberName,
              expr->leftHand()->type());

            DeclaredEntity* callee;
            FunctionGroup* group;

            if ((group = dynamic_cast<FunctionGroup*>(lookupResult)) != NULL)
            {
                NodeList<Expression> args;
                args.push_back(expr->rightHand());

                callee = bestMatch(
                  group,
                  args.begin(),
                  args.end(),
                  expr->location());

                if (callee == NULL)
                    return NULL;
            }
            else
            {
                mRBuilder->addError(
                  expr,
                  boost::format(
                    "Expected member '%1%' of type '%2%' to be a method "
                    "for compound assignment.")
                     % memberName
                     % expr->leftHand()->type());

                return NULL;
            }
            
            assert (callee != NULL);

            Expression* newExpr = new CompoundAssignmentExpression(
              expr->leftHand(),
              expr->rightHand(),
              callee);

            expr->replaceWith(newExpr);
        }
        /*
         * Otherwise, create a CallExpression.
         */
        else
        {
            Expression* callee =
              new UnresolvedMemberAccessExpression(
                expr->leftHand(),
                memberName);

            callee->setLocation(expr->leftHand()->location());

            Expression* arg = expr->rightHand();
            Expression* newExpr = new CallExpression(callee, &arg, &arg + 1);
            expr->replaceWith(newExpr);
            newExpr->visit(this);
        }

        return NULL;
    }
}

void*
BasicAnalyzer::visitUnresolvedElementAccessExpression(
    UnresolvedElementAccessExpression* expr)
{
    /*
     * We assume here that the element access expression is the equivalent
     * of a getElement() call. The setElement() case is handled in
     * visitAsssignmentExpresion().
     */

    Expression* callee =
      new UnresolvedMemberAccessExpression(expr->operand(), "getElement");

    Expression* arg = expr->index();

    callee->setLocation(expr->operand()->location());

    Expression* newExpr = new CallExpression(callee, &arg, &arg + 1);
    expr->replaceWith(newExpr);
    newExpr->visit(this);

    return NULL;
}


void*
BasicAnalyzer::visitUnresolvedInstancePropertyExpression(
    UnresolvedInstancePropertyExpression* expr)
{
    /*
     * If the expression is the left-hand side of an assignment expression,
     * visitAssignmentExpression() will take care of transforming it into
     * an InstancePropertySetExpression. We just deal with the "get" access
     * case here.
     */
    if (mLValue)
        return NULL;

    if (expr->target()->getAccessor() == NULL)
    {
        mRBuilder->addError(
          expr,
          boost::format(
            "Property '%1%' is write-only.")
             % expr->target());                
    }
    else
    {
        checkVisible(expr->target(), expr->location());
        checkVisible(expr->target()->getAccessor(), expr->location());

        Expression* callee =
          new InstanceFunctionExpression(expr->instance(),
                                         expr->target()->getAccessor());

        callee->setLocation(expr->location());
        expr->replaceWith(new CallExpression(callee));
    }

    return NULL;
}


void*
BasicAnalyzer::visitUnresolvedMemberAccessExpression(
    UnresolvedMemberAccessExpression* expr)
{
    expr->operand()->visit(this);

    TypeExpression* texpr;
    ModuleExpression* mexpr;

    /*
     * If the member access' operand denotes a type, look up the denoted
     * entity in that type.
     */
    if ((texpr = dynamic_cast<TypeExpression*>(expr->operand())) != NULL)
    {
        NamedEntity* lookupResult = resolveName(
          expr->memberName(),
          texpr->target());

        if (lookupResult == NULL)
        {
            mRBuilder->addError(
              expr,
              boost::format(
                "Type '%1%' has no member called '%2%'.")
                 % texpr->target()
                 % expr->memberName());        
        }
        else
        {
            Expression* newExpr = createEntityExpression(lookupResult);
            expr->replaceWith(newExpr);
            newExpr->visit(this);
        }
    }
    /*
     * If the member access' operand denotes a module, look up the denoted
     * entity in that module.
     */
    else if ((mexpr = dynamic_cast<ModuleExpression*>(expr->operand())) != NULL)
    {
        NamedEntity* lookupResult = resolveName(
          expr->memberName(),
          mexpr->target());

        if (lookupResult == NULL)
        {
            mRBuilder->addError(
              expr,
              boost::format(
                "Module '%1%' has no member called '%2%'.")
                 % mexpr->target()
                 % expr->memberName());
        }
        else
        {
            Expression* newExpr = createEntityExpression(lookupResult);
            expr->replaceWith(newExpr);
            newExpr->visit(this);
        }
    }
    /*
     * Otherwise, the operand expression denotes an instance value, and
     * we need to create a corresponding intance member expression. (We
     * can only do this if we know the exact type of the operand, so we
     * check if the operand's type is known first.)
     */
    else if (dynamic_cast<UnknownType*>(expr->operand()->type()) == NULL)
    {
        NamedEntity* lookupResult = resolveName(
          expr->memberName(),
          expr->operand()->type());

        if (lookupResult == NULL)
        {
            mRBuilder->addError(
              expr,
              boost::format(
                "Type '%1%' has no member called '%2%'.")
                 % expr->operand()->type()
                 % expr->memberName());        
        }
        else
        {
            Expression* newExpr =
              createMemberExpression(lookupResult, expr->operand());

            expr->replaceWith(newExpr);
            newExpr->visit(this);
        }
    }

    return NULL;
}


void*
BasicAnalyzer::visitUnresolvedObjectCreationExpression(
  UnresolvedObjectCreationExpression* expr)
{
    for (CallExpression::arguments_iterator it = expr->arguments_begin();
         it != expr->arguments_end(); it++)
    {
        (*it)->visit(this);

        if (dynamic_cast<UnknownType*>((*it)->type()) != NULL)
            return NULL;
    }

    if (dynamic_cast<UnknownType*>(expr->type()) != NULL)
    {
        expr->type()->visit(this);

        if (dynamic_cast<UnknownType*>(expr->type()) != NULL)
            return NULL;
    }

    /*
     * The denoted type should never be an array type, as we handle array
     * creation with the separate ArrayCreationExpression class.
     * (See visitArrayCreationExpression().)
     */
    assert (dynamic_cast<ArrayType*>(expr->type()) == NULL);

    FunctionGroup* constructors = dynamic_cast<FunctionGroup*>(
      resolveName(CONSTRUCTOR_NAME, expr->type()));

    assert (constructors != NULL);

    Function* bestMatching = bestMatch(
      constructors,
      expr->arguments_begin(),
      expr->arguments_end(),
      expr->location());

    if (bestMatching != NULL)
    {
        assert (dynamic_cast<Constructor*>(bestMatching) != NULL);

        expr->replaceWith(new ObjectCreationExpression(
          expr->type(),
          (Constructor*) bestMatching,
          expr->arguments_begin(),
          expr->arguments_end()));
    }

    return NULL;
}


void*
BasicAnalyzer::visitUnresolvedSimpleNameExpression(
  UnresolvedSimpleNameExpression* expr)
{
    NamedEntity* lookupResult = resolveName(expr->name());

    if (lookupResult != NULL)
    {
        DeclaredEntity* entity =
          dynamic_cast<DeclaredEntity*>(lookupResult);

        Expression* newExpr;

        /*
         * If an instance member is denoted by a simple name, this
         * means it was denoted from an instance function. This is
         * semantically equivalent to a member access expression with
         * "this" as operand (that is, "memberName" was written for
         * "this.memberName").
         */
        if (entity != NULL && mEnclosingType != NULL &&
            dynamic_cast<UserDefinedType*>(entity->parent()) != NULL &&
            !entity->hasModifier(DeclaredEntity::STATIC))
        {
            Expression* thisExpr = new ThisExpression(mEnclosingType);
            thisExpr->setLocation(expr->location());

            newExpr = createMemberExpression(lookupResult, thisExpr);
        }
        else
            newExpr = createEntityExpression(lookupResult);

        expr->replaceWith(newExpr);
        newExpr->visit(this);
    }
    else
    {
        mRBuilder->addError(
          expr,
          boost::format(
            "'%1%' is not known in this scope.")
             % expr->name());
    }

    return NULL;
}


void*
BasicAnalyzer::visitUnresolvedThisExpression(UnresolvedThisExpression* expr)
{
    if (mEnclosingType == NULL)
    {
        mRBuilder->addError(
          expr,
          "A \"this\" expression may not appear outside of a type "
          "declaration.");
    }
    else
        expr->replaceWith(new ThisExpression(mEnclosingType));

    return NULL;
}


void*
BasicAnalyzer::visitVariableExpression(VariableExpression* expr)
{
    checkVisible(expr->target(), expr->location());
    return NULL;
}


}}
