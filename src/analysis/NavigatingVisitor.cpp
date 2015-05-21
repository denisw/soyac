/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include <ast/ast.hpp>
#include "NavigatingVisitor.hpp"

namespace soyac {
namespace analysis
{


///// Module ///////////////////////////////////////////////////////////////////


void*
NavigatingVisitor::visitModule(Module* m)
{
    for (Module::imports_iterator it = m->imports_begin();
         it != m->imports_end(); it++)
    {
        (*it)->visit(this);
    }

    m->body()->visit(this);
    return NULL;
}


///// Statements ///////////////////////////////////////////////////////////////


void*
NavigatingVisitor::visitBlock(Block* b)
{
    for (Block::statements_iterator it = b->statements_begin();
         it != b->statements_end(); it++)
    {
        (*it)->visit(this);
    }

    return NULL;
}


void*
NavigatingVisitor::visitDeclarationBlock(DeclarationBlock* b)
{
    for (DeclarationBlock::declarations_iterator it = b->declarations_begin();
         it != b->declarations_end(); it++)
    {
        (*it)->visit(this);
    }

    return NULL;
}


void*
NavigatingVisitor::visitDeclarationStatement(DeclarationStatement* stmt)
{
    stmt->declaredEntity()->visit(this);
    return NULL;
}


void*
NavigatingVisitor::visitDoStatement(DoStatement* stmt)
{
    stmt->condition()->visit(this);
    stmt->body()->visit(this);
    return NULL;
}


void*
NavigatingVisitor::visitExpressionStatement(ExpressionStatement* stmt)
{
    stmt->expression()->visit(this);
    return NULL;
}


void*
NavigatingVisitor::visitForStatement(ForStatement* stmt)
{
    for (ForStatement::initializers_iterator it = stmt->initializers_begin();
         it != stmt->initializers_end(); it++)
    {
        (*it)->visit(this);
    }

    stmt->condition()->visit(this);

    for (ForStatement::iterators_iterator it = stmt->iterators_begin();
         it != stmt->iterators_end(); it++)
    {
        (*it)->visit(this);
    }

    stmt->body()->visit(this);
    return NULL;
}


void*
NavigatingVisitor::visitIfStatement(IfStatement* stmt)
{
    stmt->condition()->visit(this);
    stmt->body()->visit(this);

    if (stmt->elseBody())
        stmt->elseBody()->visit(this);

    return NULL;
}


void*
NavigatingVisitor::visitReturnStatement(ReturnStatement* stmt)
{
    if (stmt->returnValue() != NULL)
        stmt->returnValue()->visit(this);

    return NULL;
}


void*
NavigatingVisitor::visitWhileStatement(WhileStatement* stmt)
{
    stmt->condition()->visit(this);
    stmt->body()->visit(this);
    return NULL;
}


///// Declared Entities ////////////////////////////////////////////////////////


void*
NavigatingVisitor::visitClassType(ClassType* type)
{
    type->body()->visit(this);
    return NULL;
}


void*
NavigatingVisitor::visitConstructor(Constructor* cons)
{
    for (Constructor::parameters_iterator it = cons->parameters_begin();
         it != cons->parameters_end(); it++)
    {
        (*it)->visit(this);
    }

    cons->initializer()->visit(this);

    if (cons->body() != NULL)
        cons->body()->visit(this);

    return NULL;
}


void*
NavigatingVisitor::visitConstructorInitializer(ConstructorInitializer* init)
{
    for (ConstructorInitializer::arguments_iterator it =
           init->arguments_begin();
         it != init->arguments_end(); it++)
    {
        (*it)->visit(this);
    }

    return NULL;
}


void*
NavigatingVisitor::visitEnumType(EnumType* type)
{
    for (EnumType::values_iterator it = type->values_begin();
         it != type->values_end(); it++)
    {
        (*it)->visit(this);
    }

    return NULL;
}


void*
NavigatingVisitor::visitFunction(Function* func)
{
    for (Function::parameters_iterator it = func->parameters_begin();
         it != func->parameters_end(); it++)
    {
        (*it)->visit(this);
    }

    if (func->body() != NULL)
        func->body()->visit(this);

    return NULL;
}


void*
NavigatingVisitor::visitProperty(Property* prop)
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
NavigatingVisitor::visitPropertyGetAccessor(PropertyGetAccessor* acc)
{
    if (acc->body() != NULL)
        acc->body()->visit(this);

    return NULL;
}


void*
NavigatingVisitor::visitPropertySetAccessor(PropertySetAccessor* acc)
{
    if (acc->body() != NULL)
        acc->body()->visit(this);

    return NULL;
}


void*
NavigatingVisitor::visitStructType(StructType* type)
{
    type->body()->visit(this);
    return NULL;
}


void*
NavigatingVisitor::visitVariable(Variable* var)
{
    if (var->initializer() != NULL)
        var->initializer()->visit(this);

    return NULL;
}


///// Expressions //////////////////////////////////////////////////////////////


void*
NavigatingVisitor::visitArrayCreationExpression(ArrayCreationExpression* expr)
{
    if (expr->lengthExpression() != NULL)
        expr->lengthExpression()->visit(this);

    for (ArrayCreationExpression::elements_iterator it =
           expr->elements_begin();
         it != expr->elements_end(); it++)
    {
        (*it)->visit(this);
    }

    return NULL;
}


void*
NavigatingVisitor::visitAssignmentExpression(AssignmentExpression* expr)
{
    expr->leftHand()->visit(this);
    expr->rightHand()->visit(this);
    return NULL;
}


void*
NavigatingVisitor::visitCallExpression(CallExpression* expr)
{
    expr->callee()->visit(this);

    for (CallExpression::arguments_iterator it = expr->arguments_begin();
         it != expr->arguments_end(); it++)
    {
        (*it)->visit(this);
    }

    return NULL;
}

void*
NavigatingVisitor::visitCastExpression(CastExpression* expr)
{
    expr->operand()->visit(this);
    return NULL;
}


void*
NavigatingVisitor::visitInstanceFunctionExpression(
  InstanceFunctionExpression* expr)
{
    expr->instance()->visit(this);
    return NULL;
}


void*
NavigatingVisitor::visitInstanceVariableExpression(
  InstanceVariableExpression* expr)
{
    expr->instance()->visit(this);
    return NULL;
}


void*
NavigatingVisitor::visitLogicalExpression(LogicalExpression* expr)
{
    expr->leftHand()->visit(this);
    expr->rightHand()->visit(this);
    return NULL;
}


void*
NavigatingVisitor::visitLogicalNotExpression(LogicalNotExpression* expr)
{
    expr->operand()->visit(this);
    return NULL;
}

}}
