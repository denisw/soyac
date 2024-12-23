/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */
#ifndef _NAVIGATING_VISITOR_HPP
#define _NAVIGATING_VISITOR_HPP

#include <ast/Visitor.hpp>

using namespace soyac::ast;

namespace soyac {
namespace analysis {

/**
 * An implementation of the Visitor interface that automatically navigates
 * through the abstract syntax trees. This only makes overriding of those
 * visit methods necessary where real work needs to be done; the
 * navigational aspects are handled by NavigatingVisitor.
 */
class NavigatingVisitor : public Visitor {
public:
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
     * Visits a ConstructorInitializer.
     *
     * @param init  The ConstructorInitializer to visit.
     * @return      Nothing.
     */
    virtual void* visitConstructorInitializer(ConstructorInitializer* init);

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
     * Visits an IfStatement.
     *
     * @param stmt  The IfStatement to visit.
     * @return      Nothing.
     */
    virtual void* visitIfStatement(IfStatement* stmt);

    /**
     * Visits an InstanceFunctionExpression.
     *
     * @param expr  The InstanceFunctionExpression to visit.
     * @return      Nothing.
     */
    virtual void* visitInstanceFunctionExpression(
        InstanceFunctionExpression* expr);

    /**
     * Visits an InstanceVariableExpression.
     *
     * @param expr  The InstanceVariableExpression to visit.
     * @return      Nothing.
     */
    virtual void* visitInstanceVariableExpression(
        InstanceVariableExpression* expr);

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
     * Visits a Variable.
     *
     * @param var  The Variable to visit.
     * @return     Nothing.
     */
    virtual void* visitVariable(Variable* var);

    /**
     * Visits a WhileStatement.
     *
     * @param stmt  The WhileStatement to visit.
     * @return      Nothing.
     */
    virtual void* visitWhileStatement(WhileStatement* stmt);
};

} // namespace analysis
} // namespace soyac

#endif
