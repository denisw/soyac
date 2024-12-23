/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include "NavigatingVisitor.h"
#include <ast/ast.h>

namespace soyac {
namespace analysis {

///// Module
//////////////////////////////////////////////////////////////////////

void* NavigatingVisitor::visitModule(Module* m)
{
    for (Module::imports_iterator it = m->imports_begin();
        it != m->imports_end(); it++) {
        (*it)->visit(this);
    }

    m->body()->visit(this);
    return nullptr;
}

///// Statements
//////////////////////////////////////////////////////////////////

void* NavigatingVisitor::visitBlock(Block* b)
{
    for (Block::statements_iterator it = b->statements_begin();
        it != b->statements_end(); it++) {
        (*it)->visit(this);
    }

    return nullptr;
}

void* NavigatingVisitor::visitDeclarationBlock(DeclarationBlock* b)
{
    for (DeclarationBlock::declarations_iterator it = b->declarations_begin();
        it != b->declarations_end(); it++) {
        (*it)->visit(this);
    }

    return nullptr;
}

void* NavigatingVisitor::visitDeclarationStatement(DeclarationStatement* stmt)
{
    stmt->declaredEntity()->visit(this);
    return nullptr;
}

void* NavigatingVisitor::visitDoStatement(DoStatement* stmt)
{
    stmt->condition()->visit(this);
    stmt->body()->visit(this);
    return nullptr;
}

void* NavigatingVisitor::visitExpressionStatement(ExpressionStatement* stmt)
{
    stmt->expression()->visit(this);
    return nullptr;
}

void* NavigatingVisitor::visitForStatement(ForStatement* stmt)
{
    for (ForStatement::initializers_iterator it = stmt->initializers_begin();
        it != stmt->initializers_end(); it++) {
        (*it)->visit(this);
    }

    stmt->condition()->visit(this);

    for (ForStatement::iterators_iterator it = stmt->iterators_begin();
        it != stmt->iterators_end(); it++) {
        (*it)->visit(this);
    }

    stmt->body()->visit(this);
    return nullptr;
}

void* NavigatingVisitor::visitIfStatement(IfStatement* stmt)
{
    stmt->condition()->visit(this);
    stmt->body()->visit(this);

    if (stmt->elseBody()) {
        stmt->elseBody()->visit(this);
    }

    return nullptr;
}

void* NavigatingVisitor::visitReturnStatement(ReturnStatement* stmt)
{
    if (stmt->returnValue() != nullptr) {
        stmt->returnValue()->visit(this);
    }

    return nullptr;
}

void* NavigatingVisitor::visitWhileStatement(WhileStatement* stmt)
{
    stmt->condition()->visit(this);
    stmt->body()->visit(this);
    return nullptr;
}

///// Declared Entities
///////////////////////////////////////////////////////////

void* NavigatingVisitor::visitClassType(ClassType* type)
{
    type->body()->visit(this);
    return nullptr;
}

void* NavigatingVisitor::visitConstructor(Constructor* cons)
{
    for (Constructor::parameters_iterator it = cons->parameters_begin();
        it != cons->parameters_end(); it++) {
        (*it)->visit(this);
    }

    cons->initializer()->visit(this);

    if (cons->body() != nullptr) {
        cons->body()->visit(this);
    }

    return nullptr;
}

void* NavigatingVisitor::visitConstructorInitializer(
    ConstructorInitializer* init)
{
    for (ConstructorInitializer::arguments_iterator it
        = init->arguments_begin();
        it != init->arguments_end(); it++) {
        (*it)->visit(this);
    }

    return nullptr;
}

void* NavigatingVisitor::visitEnumType(EnumType* type)
{
    for (EnumType::values_iterator it = type->values_begin();
        it != type->values_end(); it++) {
        (*it)->visit(this);
    }

    return nullptr;
}

void* NavigatingVisitor::visitFunction(Function* func)
{
    for (Function::parameters_iterator it = func->parameters_begin();
        it != func->parameters_end(); it++) {
        (*it)->visit(this);
    }

    if (func->body() != nullptr) {
        func->body()->visit(this);
    }

    return nullptr;
}

void* NavigatingVisitor::visitProperty(Property* prop)
{
    if (dynamic_cast<UnknownType*>(prop->type()) != nullptr) {
        prop->type()->visit(this);
    }

    if (prop->getAccessor() != nullptr) {
        prop->getAccessor()->visit(this);
    }

    if (prop->setAccessor() != nullptr) {
        prop->setAccessor()->visit(this);
    }

    return nullptr;
}

void* NavigatingVisitor::visitPropertyGetAccessor(PropertyGetAccessor* acc)
{
    if (acc->body() != nullptr) {
        acc->body()->visit(this);
    }

    return nullptr;
}

void* NavigatingVisitor::visitPropertySetAccessor(PropertySetAccessor* acc)
{
    if (acc->body() != nullptr) {
        acc->body()->visit(this);
    }

    return nullptr;
}

void* NavigatingVisitor::visitStructType(StructType* type)
{
    type->body()->visit(this);
    return nullptr;
}

void* NavigatingVisitor::visitVariable(Variable* var)
{
    if (var->initializer() != nullptr) {
        var->initializer()->visit(this);
    }

    return nullptr;
}

///// Expressions
/////////////////////////////////////////////////////////////////

void* NavigatingVisitor::visitArrayCreationExpression(
    ArrayCreationExpression* expr)
{
    if (expr->lengthExpression() != nullptr) {
        expr->lengthExpression()->visit(this);
    }

    for (ArrayCreationExpression::elements_iterator it = expr->elements_begin();
        it != expr->elements_end(); it++) {
        (*it)->visit(this);
    }

    return nullptr;
}

void* NavigatingVisitor::visitAssignmentExpression(AssignmentExpression* expr)
{
    expr->leftHand()->visit(this);
    expr->rightHand()->visit(this);
    return nullptr;
}

void* NavigatingVisitor::visitCallExpression(CallExpression* expr)
{
    expr->callee()->visit(this);

    for (CallExpression::arguments_iterator it = expr->arguments_begin();
        it != expr->arguments_end(); it++) {
        (*it)->visit(this);
    }

    return nullptr;
}

void* NavigatingVisitor::visitCastExpression(CastExpression* expr)
{
    expr->operand()->visit(this);
    return nullptr;
}

void* NavigatingVisitor::visitInstanceFunctionExpression(
    InstanceFunctionExpression* expr)
{
    expr->instance()->visit(this);
    return nullptr;
}

void* NavigatingVisitor::visitInstanceVariableExpression(
    InstanceVariableExpression* expr)
{
    expr->instance()->visit(this);
    return nullptr;
}

void* NavigatingVisitor::visitLogicalExpression(LogicalExpression* expr)
{
    expr->leftHand()->visit(this);
    expr->rightHand()->visit(this);
    return nullptr;
}

void* NavigatingVisitor::visitLogicalNotExpression(LogicalNotExpression* expr)
{
    expr->operand()->visit(this);
    return nullptr;
}

} // namespace analysis
} // namespace soyac
