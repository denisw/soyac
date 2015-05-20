/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _BASIC_ANALYZER_HPP
#define _BASIC_ANALYZER_HPP

#include <ast/Expression.hpp>
#include <ast/Function.hpp>
#include <ast/NodeList.hpp>
#include <ast/Visitor.hpp>
#include <common/PassResult.hpp>
#include <common/PassResultBuilder.hpp>
#include "SymbolTable.hpp"

using namespace soyac::ast;

namespace soyac {
namespace analysis
{

class FunctionGroup;

/**
 * A basic analysis and transformation pass including name resolution,
 * type checking and other checks for conformance with the Soya Language
 * Reference. This doesn't include control flow related checks; these are
 * done by the ControlFlowAnalyzer.  
 */
class BasicAnalyzer : public Visitor
{
public:
    /**
     * Creates a BasicAnalyzer.
     */
    BasicAnalyzer();

    /**
     * Analyzes the passed module and returns a PassResult instance
     * summarizing the problems found during analysis, if any. If
     * the analyzer notices one more yet unprocessed required modules,
     * a ModulesRequiredException is thrown.
     *
     * @throw ModulesRequiredException  If the analyzed module depends on
     *                                  one or multiple other modules which
     *                                  were not processed yet.
     *
     * @param m  The module to analyze.
     * @return   The analysis' result. Owned by the caller.
     */
    PassResult* analyze(Module* m);

protected:
    /**
     * Visits an ArrayCreationExpression.
     *
     * @param expr  The ArrayCreationExpression to visit.
     * @return      Nothing.
     */
    virtual void* visitArrayCreationExpression(ArrayCreationExpression* expr);

    /**
     * Visits an AssignmentExpression.
     *
     * @param expr  The AssignmentExpression to visit.
     * @return      Nothing.
     */
    virtual void* visitAssignmentExpression(AssignmentExpression* expr);

    /**
     * Visits a Block.
     *
     * @param b  The Block to visit.
     * @return   Nothing.
     */
    virtual void* visitBlock(Block* b);

    /**
     * Visits a CallExpression.
     *
     * @param expr  The CallExpression to visit.
     * @return      Nothing.
     */
    virtual void* visitCallExpression(CallExpression* expr);

    /**
     * Visits a CastExpression.
     *
     * @param expr  The CastExpression to visit.
     * @return      Nothing.
     */
    virtual void* visitCastExpression(CastExpression* expr);

    /**
     * Visits a ClassType.
     *
     * @param type  The ClassType to visit.
     * @return      Nothing.
     */
    virtual void* visitClassType(ClassType* type);

    /**
     * Visits a Constructor.
     *
     * @param cons  The Constructor to visit.
     * @return      Nothing.
     */
    virtual void* visitConstructor(Constructor* cons);

    /**
     * Visits a DeclarationBlock.
     *
     * @param b  The DeclarationBlock to visit.
     * @return   Nothing.
     */
    virtual void* visitDeclarationBlock(DeclarationBlock* b);

    /**
     * Visits a DeclarationStatement.
     *
     * @param stmt  The DeclarationStatement to visit.
     * @return      Nothing.
     */
    virtual void* visitDeclarationStatement(DeclarationStatement* stmt);

    /**
     * Visits a DoStatement.
     *
     * @param stmt  The DoStatement to visit.
     * @return      Nothing.
     */
    virtual void* visitDoStatement(DoStatement* stmt);

    /**
     * Visits an EntityImport.
     *
     * @param imp  The EntityImport to visit.
     * @return     Nothing.
     */
    virtual void* visitEntityImport(EntityImport* imp);

    /**
     * Visits an EnumType.
     *
     * @param type  The EnumType to visit.
     * @return      Nothing.
     */
    virtual void* visitEnumType(EnumType* type);

    /**
     * Visits an ExpressionStatement.
     *
     * @param stmt  The ExpressionStatement to visit.
     * @return      Nothing.
     */
    virtual void* visitExpressionStatement(ExpressionStatement* stmt);

    /**
     * Visits a ForStatement.
     *
     * @param stmt  The ForStatement to visit.
     * @return      Nothing.
     */
    virtual void* visitForStatement(ForStatement* stmt);

    /**
     * Visits a Function.
     *
     * @param func  The Function to visit.
     * @return      Nothing.
     */
    virtual void* visitFunction(Function* func);

    /**
     * Visits a FunctionGroupImport.
     *
     * @param imp  The FunctionGroupImport to visit.
     * @return     Nothing.
     */
    virtual void* visitFunctionGroupImport(FunctionGroupImport* imp);

    /**
     * Visits a FunctionParameter.
     *
     * @param param  The FunctionParameter to visit.
     * @return       Nothing.
     */
    virtual void* visitFunctionParameter(FunctionParameter* param);

    /**
     * Visits an IfStatement.
     *
     * @param stmt  The IfStatement to visit.
     * @return      Nothing.
     */
    virtual void* visitIfStatement(IfStatement* stmt);

    /**
     * Visits an InstanceVariableExpression.
     *
     * @param expr  The InstanceVariableExpression to visit.
     * @return      Nothing.
     */
    virtual void* visitInstanceVariableExpression(InstanceVariableExpression* expr);

    /**
     * Visits a LogicalExpression.
     *
     * @param expr  The LogicalExpression to visit.
     * @return      Nothing.
     */
    virtual void* visitLogicalExpression(LogicalExpression* expr);

    /**
     * Visits a LogicalNotExpression.
     *
     * @param expr  The LogicalNotExpression to visit.
     * @return      Nothing.
     */
    virtual void* visitLogicalNotExpression(LogicalNotExpression* expr);

    /**
     * Visits a Module.
     *
     * @param m  The Module to visit.
     * @return   Nothing.
     */
    virtual void* visitModule(Module* m);

    /**
     * Visits an ModuleImport.
     *
     * @param imp  The ModuleImport to visit.
     * @return     Nothing.
     */
    virtual void* visitModuleImport(ModuleImport* imp);

    /**
     * Visits a Property.
     *
     * @param prop  The Property to visit.
     * @return      Nothing.
     */
    virtual void* visitProperty(Property* prop);

    /**
     * Visits a PropertyGetAccessor.
     *
     * @param acc  The PropertyGetAccessor to visit.
     * @return     Nothing.
     */
    virtual void* visitPropertyGetAccessor(PropertyGetAccessor* acc);

    /**
     * Visits a PropertySetAccessor.
     *
     * @param acc  The PropertySetAccessor to visit.
     * @return     Nothing.
     */
    virtual void* visitPropertySetAccessor(PropertySetAccessor* acc);

    /**
     * Visits a ReturnStatement.
     *
     * @param stmt  The ReturnStatement to visit.
     * @return      Nothing.
     */
    virtual void* visitReturnStatement(ReturnStatement* stmt);

    /**
     * Visits a StructType.
     *
     * @param type  The StructType to visit.
     * @return      Nothing.
     */
    virtual void* visitStructType(StructType* type);

    /**
     * Visits an UnknownArrayType.
     *
     * @param type  The UnknownArrayType to visit.
     * @return      Nothing.
     */
    virtual void* visitUnknownArrayType(UnknownArrayType* type);

    /**
     * Visits an UnknownFunctionType.
     *
     * @param type  The UnknownFunctionType to visit.
     * @return      Nothing.
     */
    virtual void* visitUnknownFunctionType(UnknownFunctionType* type);

    /**
     * Visits an UnknownType.
     *
     * @param type  The UnknownType to visit.
     * @return      Nothing.
     */
    virtual void* visitUnknownType(UnknownType* type);

    /**
     * Visits an UnresolvedBaseConstructorInitializer.
     *
     * @param init  The UnresolvedBaseConstructorInitializer to visit.
     * @return      Nothing.
     */
    virtual void* visitUnresolvedBaseConstructorInitializer(UnresolvedBaseConstructorInitializer* init);

    /**
     * Visits an UnresolvedBinaryExpression.
     *
     * @param expr  The UnresolvedBinaryExpression to visit.
     * @return      Nothing.
     */
    virtual void* visitUnresolvedBinaryExpression(UnresolvedBinaryExpression* expr);

    /**
     * Visits an UnresolvedConstructorInitializer.
     *
     * @param init  The UnresolvedConstructorInitializer to visit.
     * @return      Nothing.
     */
    virtual void* visitUnresolvedConstructorInitializer(UnresolvedConstructorInitializer* init);

    /**
     * Visits an UnresolvedDefaultConstructorInitializer.
     *
     * @param init  The UnresolvedDefaultConstructorInitializer to visit.
     * @return      Nothing.
     */
    virtual void* visitUnresolvedDefaultConstructorInitializer(UnresolvedDefaultConstructorInitializer* init);

    /**
     * Visits an UnresolvedElementAccessExpression.
     *
     * @param expr  The UnresolvedElementAccessExpression to visit.
     * @return      Nothing.
     */
    virtual void* visitUnresolvedElementAccessExpression(UnresolvedElementAccessExpression* expr);

    /**
     * Visits an UnresolvedEntityImport.
     *
     * @param imp  The UnresolvedEntityImport to visit.
     * @return     Nothing.
     */
    virtual void* visitUnresolvedEntityImport(UnresolvedEntityImport* imp);

    /**
     * Visits an UnresolvedInstancePropertyExpression.
     *
     * @param expr  The UnresolvedInstancePropertyExpression to visit.
     * @return      Nothing.
     */
    virtual void* visitUnresolvedInstancePropertyExpression(UnresolvedInstancePropertyExpression* expr);

    /**
     * Visits an UnresolvedMemberAccessExpression.
     *
     * @param expr  The UnresolvedMemberAccessExpression to visit.
     * @return      Nothing.
     */
    virtual void* visitUnresolvedMemberAccessExpression(UnresolvedMemberAccessExpression* expr);

    /**
     * Visits an UnresolvedModuleImport.
     *
     * @param imp  The UnresolvedModuleImport to visit.
     * @return     Nothing.
     */
    virtual void* visitUnresolvedModuleImport(UnresolvedModuleImport* imp);

    /**
     * Visits an UnresolvedObjectCreationExpression.
     *
     * @param expr  The UnresolvedObjectCreationExpression to visit.
     * @return      Nothing.
     */
    virtual void* visitUnresolvedObjectCreationExpression(UnresolvedObjectCreationExpression* expr);

    /**
     * Visits an UnresolvedSimpleNameExpression.
     *
     * @param expr  The UnresolvedSimpleNameExpression to visit.
     * @return      Nothing.
     */
    virtual void* visitUnresolvedSimpleNameExpression(UnresolvedSimpleNameExpression* expr);

    /**
     * Visits an UnresolvedThisExpression.
     *
     * @param expr  The UnresolvedThisExpression to visit.
     * @return      Nothing.
     */
    virtual void* visitUnresolvedThisExpression(UnresolvedThisExpression* expr);

    /**
     * Visits a TypeExpression.
     *
     * @param expr  The TypeExpression to visit.
     * @return      Nothing.
     */
    virtual void* visitTypeExpression(TypeExpression* expr);

    /**
     * Visits a Variable.
     *
     * @param var  The Variable to visit.
     * @return     Nothing.
     */
    virtual void* visitVariable(Variable* var);

    /**
     * Visits a VariableExpression.
     *
     * @param expr  The VariableExpression to visit.
     * @return      Nothing.
     */
    virtual void* visitVariableExpression(VariableExpression* expr);

    /**
     * Visits a WhileStatement.
     *
     * @param stmt  The WhileStatement to visit.
     * @return      Nothing.
     */
    virtual void* visitWhileStatement(WhileStatement* stmt);

private:
    Module* mModule;

    SymbolTable* mSymbolTable;
    std::list<Module*> mImportedModules;

    PassResultBuilder* mRBuilder;

    Function* mEnclosingFunc;
    Type* mEnclosingType;
    bool mLValue;

    /**
     * Resolves the passed name in the context of the current scope and
     * returns the matching named entity if one exists. Otherwise, null
     * is returned.
     *
     * @param name  The name to resolve.
     * @return      The matching entity, or null.
     * @see         SymbolTable::lookup(const Name&)
     */
    NamedEntity* resolveName(const Name& name);

    /**
     * Resolves the passed name in the context of the specified named
     * entity and returns the matching member if one exists. Otherwise,
     * null is returned.
     *
     * @param name    The name to resolve.
     * @param entity  The named entity.
     * @return        The matching member, or null.
     * @see           SymbolTable::lookup(const Name&, NamedEntity*)
     */
    NamedEntity* resolveName(const Name& name, NamedEntity* entity);

    /**
     * Returns @c true if the passed declared entity is visible in the
     * current scope's context.
     *
     * @param entity  The entity.
     * @return        @c true if the entity is visible;
     *                @c false otherwise.
     */
    bool isVisible(DeclaredEntity* entity);

    /**
     * Checks if the passed entity is visible in the current scope's context,
     * and reports an error at the specified location if it isn't.
     *
     * @param entity    The entity.
     * @param errorLoc  The error location.
     */
    void checkVisible(DeclaredEntity* entity,
                      const Location& errorLoc);

    /**
     * Checks if the passed expression's value can be converted to the
     * specified type, and returns @c true in this case. Otherwise,
     * @c false is returned.
     *
     * By default, only the possibility of an implicit conversions is checked.
     * If @c true is passed as the third parameter, explicit conversions
     * are also taken into account.
     *
     * This method should be used rather than direct calls to isConvertable()
     * and isImplicitlyConvertable() inside the BasicAnalyzer, as it also
     * respects some special cases e.g. for integer literal conversions.
     *
     * @param expr       The expression.
     * @param type       The type.
     * @param _explicit  Whether to consider explicit conversions.
     * @return           @c true if the expression is convertable to the type;
     *                   @c false otherwise.
     */
    bool canConvert(Expression* expr, Type* type, bool _explicit = false);

    /**
     * Wraps the passed expression into a CastExpression to the specified
     * type if the conversion is possible (checked with canConvert()), and
     * replaces the passed expression with it using replaceWith().
     *
     * On success, @c true is returned. If the conversion is *not* possible,
     * nothing is done, and the return value is @c false.
     *
     * By default, only the possibility of an implicit conversions is checked.
     * If @c true is passed as the third parameter, explicit conversions
     * are also taken into account.
     *
     * @param expr       The expression to convert.
     * @param type       The type to convert to.
     * @param _explicit  Whether to consider explicit conversions.
     * @return           @c true on success;
     *                   @c false otherwise.
     */
    bool convert(Expression* expr, Type* type, bool _explicit = false);
    
    /**
     * Looks through the passed group of overloaded functions and returns
     * the overload that matches the specified arguments best. If multiple
     * overloads fit the arguments equally well or none of the overloads
     * match at all, an error is added to the BasicAnalyzer's internal
     * PassResultBuilder with the specified location information, and
     * null is returned.
     *
     * @param overloads        The overloaded functions.
     * @param arguments_begin  Start iterator for the arguments to match.
     * @param arguments_end    End iterator for the arguments to match.
     * @param errorLoc         The location information for error messages.
     * @return                 The best-matching overload from the
     *                         UnresolvedFunctionExpression, or null.
     */
    Function* bestMatch(FunctionGroup* overloads,
                        NodeList<Expression>::const_iterator arguments_begin,
                        NodeList<Expression>::const_iterator arguments_end,
                        const Location& errorLoc);

    /**
     * From two passed function overloads, return the one whose parameter
     * types match the specified arguments better. If both fit equally well,
     * null is returned.
     *
     * Used to implement bestMatch().
     *
     * @param func1            The first function for the comparison.
     * @param func2            The second function for the comparison.
     * @param arguments_begin  Start iterator for the arguments to match.
     * @param arguments_end    End iterator for the arguments to match.
     * @return                 The better-matching function, or null.
     */
    Function* betterMatch(Function* func1,
                          Function* func2,
                          NodeList<Expression>::const_iterator arguments_begin,
                          NodeList<Expression>::const_iterator arguments_end);

    /**
     * Creates an entity-denoting Expression from the passed entity
     * and returns it.
     *
     * @param entity  The entity.
     * @return        An Expression denoting the entity.
     */
    Expression* createEntityExpression(NamedEntity* entity);

    /**
     * Creates and returns a member-denoting Expression from the passed
     * instance expression and member entity and returns it.
     *
     * @param member    The instance member.
     * @param instance  The instance expression.
     * @return          An Expression denoting the instance and member.
     */
    Expression* createMemberExpression(NamedEntity* member,
                                       Expression* instance);
};


}}

#endif
