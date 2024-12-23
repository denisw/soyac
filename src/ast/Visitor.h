/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef SOYA_VISITOR_H_
#define SOYA_VISITOR_H_

namespace soyac {
namespace ast {

/*
 * We don't want to introduce circular dependencies, so we just include
 * forward declarations to all node classes.
 */
class ArrayCreationExpression;
class ArrayType;
class AssignmentExpression;
class Block;
class BooleanLiteral;
class BooleanType;
class CallExpression;
class CastExpression;
class CharacterLiteral;
class CharacterType;
class ClassType;
class CombinedRelationalExpression;
class CompoundAssignmentExpression;
class Constructor;
class ConstructorInitializer;
class DeclarationBlock;
class DeclarationStatement;
class DoStatement;
class EntityImport;
class EnumConstant;
class EnumType;
class EnumValueExpression;
class ExpressionStatement;
class FloatingPointLiteral;
class FloatingPointType;
class ForStatement;
class Function;
class FunctionExpression;
class FunctionGroupImport;
class FunctionParameterExpression;
class FunctionParameter;
class FunctionType;
class IfStatement;
class InstanceFunctionExpression;
class InstancePropertyGetExpression;
class InstancePropertySetExpression;
class InstanceVariableExpression;
class IntegerLiteral;
class IntegerType;
class LogicalExpression;
class LogicalNotExpression;
class MemberAccessExpression;
class Module;
class ModuleExpression;
class ModuleImport;
class ObjectCreationExpression;
class Property;
class PropertyGetAccessor;
class PropertySetAccessor;
class ReturnStatement;
class StructType;
class ThisExpression;
class TypeExpression;
class UnknownArrayType;
class UnknownFunctionType;
class UnknownType;
class UnresolvedSimpleNameExpression;
class UnresolvedBaseConstructorInitializer;
class UnresolvedBinaryExpression;
class UnresolvedConstructorInitializer;
class UnresolvedDefaultConstructorInitializer;
class UnresolvedElementAccessExpression;
class UnresolvedEntityImport;
class UnresolvedFunctionExpression;
class UnresolvedMemberAccessExpression;
class UnresolvedInstanceFunctionExpression;
class UnresolvedInstancePropertyExpression;
class UnresolvedModuleImport;
class UnresolvedObjectCreationExpression;
class UnresolvedThisExpression;
class Variable;
class VariableExpression;
class VoidType;
class WhileStatement;

/**
 * Th base class of all "visitors", that is, operators which process an
 * abstract syntax tree by walking through it and "visiting" each of its
 * nodes. Examples for visitors are code analysis passes or code generators.
 *
 * For every type of Node, the Visitor class has a virtual visit
 * method; a class inheriting from Visitor can override the methods it
 * needs. Note that, in the interest of flexibility, every node has to be
 * visited manually inside the visit methods - there is no automatic
 * walking through the tree.
 *
 * Every visit method has a @c void* return type, which enables inheriting
 * classes to return arbitrary values as results of the visit. For instance,
 * the code generator's visit methods return the LLVM object which match the
 * visited abstract syntax tree node. The default implementation of every
 * visit method just returns null.
 */
class Visitor {
protected:
    /*
     * All AST node classes need to be friends of the Visitor class to call
     * the corresponding visit method in their visit() implementations.
     */
    friend class ArrayCreationExpression;
    friend class ArrayType;
    friend class AssignmentExpression;
    friend class Block;
    friend class BooleanLiteral;
    friend class BooleanType;
    friend class CallExpression;
    friend class CastExpression;
    friend class CharacterLiteral;
    friend class CharacterType;
    friend class ClassType;
    friend class CombinedRelationalExpression;
    friend class CompoundAssignmentExpression;
    friend class Constructor;
    friend class ConstructorInitializer;
    friend class DeclarationBlock;
    friend class DeclarationStatement;
    friend class DoStatement;
    friend class EntityImport;
    friend class EnumConstant;
    friend class EnumType;
    friend class EnumValueExpression;
    friend class ExpressionStatement;
    friend class FloatingPointLiteral;
    friend class FloatingPointType;
    friend class ForStatement;
    friend class Function;
    friend class FunctionExpression;
    friend class FunctionGroupImport;
    friend class FunctionParameter;
    friend class FunctionParameterExpression;
    friend class FunctionType;
    friend class IfStatement;
    friend class InstanceFunctionExpression;
    friend class InstancePropertyGetExpression;
    friend class InstancePropertySetExpression;
    friend class InstanceVariableExpression;
    friend class IntegerLiteral;
    friend class IntegerType;
    friend class LogicalExpression;
    friend class LogicalNotExpression;
    friend class MemberAccessExpression;
    friend class Module;
    friend class ModuleExpression;
    friend class ModuleImport;
    friend class ObjectCreationExpression;
    friend class Property;
    friend class PropertyGetAccessor;
    friend class PropertySetAccessor;
    friend class ReturnStatement;
    friend class StructType;
    friend class ThisExpression;
    friend class TypeExpression;
    friend class UnknownArrayType;
    friend class UnknownFunctionType;
    friend class UnknownType;
    friend class UnresolvedSimpleNameExpression;
    friend class UnresolvedBaseConstructorInitializer;
    friend class UnresolvedBinaryExpression;
    friend class UnresolvedConstructorInitializer;
    friend class UnresolvedDefaultConstructorInitializer;
    friend class UnresolvedElementAccessExpression;
    friend class UnresolvedEntityImport;
    friend class UnresolvedFunctionExpression;
    friend class UnresolvedMemberAccessExpression;
    friend class UnresolvedModuleImport;
    friend class UnresolvedObjectCreationExpression;
    friend class UnresolvedInstanceFunctionExpression;
    friend class UnresolvedInstancePropertyExpression;
    friend class UnresolvedThisExpression;
    friend class Variable;
    friend class VariableExpression;
    friend class VoidType;
    friend class WhileStatement;

    virtual ~Visitor() { }

    /**
     * Visits an ArrayCreationExpression.
     *
     * @param expr  The ArrayCreationExpression to visit.
     * @return      The subclass-specific return value.
     */
    virtual void* visitArrayCreationExpression(ArrayCreationExpression* expr)
    {
        return nullptr;
    }

    /**
     * Visits an ArrayType.
     *
     * @param type  The ArrayType to visit.
     * @return      The subclass-specific return value.
     */
    virtual void* visitArrayType(ArrayType* type) { return nullptr; }

    /**
     * Visits an AssignmentExpression.
     *
     * @param expr  The AssignmentExpression to visit.
     * @return      The subclass-specific return value.
     */
    virtual void* visitAssignmentExpression(AssignmentExpression* expr)
    {
        return nullptr;
    }

    /**
     * Visits a Block.
     *
     * @param b  The Block to visit.
     * @return   The subclass-specific return value.
     */
    virtual void* visitBlock(Block* b) { return nullptr; }

    /**
     * Visits a BooleanLiteral.
     *
     * @param lit  The BooleanLiteral to visit.
     * @return     The subclass-specific return value.
     */
    virtual void* visitBooleanLiteral(BooleanLiteral* lit) { return nullptr; }

    /**
     * Visits a BooleanType.
     *
     * @param type  The BooleanType to visit.
     * @return      The subclass-specific return value.
     */
    virtual void* visitBooleanType(BooleanType* type) { return nullptr; }

    /**
     * Visits a CallExpression.
     *
     * @param expr  The CallExpression to visit.
     * @return      The subclass-specific return value.
     */
    virtual void* visitCallExpression(CallExpression* expr) { return nullptr; }

    /**
     * Visits a CastExpression.
     *
     * @param expr  The CastExpression to visit.
     * @return      The subclass-specific return value.
     */
    virtual void* visitCastExpression(CastExpression* expr) { return nullptr; }

    /**
     * Visits a CharacterLiteral.
     *
     * @param lit  The CharacterLiteral to visit.
     * @return     The subclass-specific return value.
     */
    virtual void* visitCharacterLiteral(CharacterLiteral* lit) { return nullptr; }

    /**
     * Visits a CharacterType.
     *
     * @param type  The CharacterType to visit.
     * @return      The subclass-specific return value.
     */
    virtual void* visitCharacterType(CharacterType* type) { return nullptr; }

    /**
     * Visits a ClassType.
     *
     * @param type  The ClassType to visit.
     * @return      The subclass-specific return value.
     */
    virtual void* visitClassType(ClassType* type) { return nullptr; }

    /**
     * Visits a CombinedRelationalExpression.
     *
     * @param expr  The CombinedRelationalExpression to visit.
     * @return      The subclass-specific return value.
     */
    virtual void* visitCombinedRelationalExpression(
        CombinedRelationalExpression* expr)
    {
        return nullptr;
    }

    /**
     * Visits an CompoundAssignmentExpression.
     *
     * @param expr  The CompoundAssignmentExpression to visit.
     * @return      The subclass-specific return value.
     */
    virtual void* visitCompoundAssignmentExpression(
        CompoundAssignmentExpression* expr)
    {
        return nullptr;
    }

    /**
     * Visits a Constructor.
     *
     * @param cons  The Constructor to visit.
     * @return      The subclass-specific return value.
     */
    virtual void* visitConstructor(Constructor* cons) { return nullptr; }

    /**
     * Visits a ConstructorInitializer.
     *
     * @param init  The ConstructorInitializer to visit.
     * @return      The subclass-specific return value.
     */
    virtual void* visitConstructorInitializer(ConstructorInitializer* init)
    {
        return nullptr;
    }

    /**
     * Visits a DeclarationBlock.
     *
     * @param stmt  The DeclarationBlock to visit.
     * @return      The subclass-specific return value.
     */
    virtual void* visitDeclarationBlock(DeclarationBlock* b) { return nullptr; }

    /**
     * Visits a DeclarationStatement.
     *
     * @param stmt  The DeclarationStatement to visit.
     * @return      The subclass-specific return value.
     */
    virtual void* visitDeclarationStatement(DeclarationStatement* stmt)
    {
        return nullptr;
    }

    /**
     * Visits a DoStatement.
     *
     * @param stmt  The DoStatement to visit.
     * @return      The subclass-specific return value.
     */
    virtual void* visitDoStatement(DoStatement* stmt) { return nullptr; }

    /**
     * Visits an EntityImport.
     *
     * @param imp  The EntityImport to visit.
     * @return     The subclass-specific return value.
     */
    virtual void* visitEntityImport(EntityImport* imp) { return nullptr; }

    /**
     * Visits an EnumConstant.
     *
     * @param ec  The EnumConstant to visit.
     * @return    The subclass-specific return value.
     */
    virtual void* visitEnumConstant(EnumConstant* ec) { return nullptr; }

    /**
     * Visits an EnumType.
     *
     * @param type  The EnumType to visit.
     * @return      The subclass-specific return value.
     */
    virtual void* visitEnumType(EnumType* type) { return nullptr; }

    /**
     * Visits an EnumValueExpression.
     *
     * @param expr  The EnumValueExpression to visit.
     * @return      The subclass-specific return value.
     */
    virtual void* visitEnumValueExpression(EnumValueExpression* expr)
    {
        return nullptr;
    }

    /**
     * Visits an ExpressionStatement.
     *
     * @param stmt  The ExpressionStatement to visit.
     * @return      The subclass-specific return value.
     */
    virtual void* visitExpressionStatement(ExpressionStatement* stmt)
    {
        return nullptr;
    }

    /**
     * Visits a FloatingPointLiteral.
     *
     * @param type  The FloatingPointLiteral to visit.
     * @return      The subclass-specific return value.
     */
    virtual void* visitFloatingPointLiteral(FloatingPointLiteral* lit)
    {
        return nullptr;
    }

    /**
     * Visits a FloatingPointType.
     *
     * @param type  The FloatingPointType to visit.
     * @return      The subclass-specific return value.
     */
    virtual void* visitFloatingPointType(FloatingPointType* type)
    {
        return nullptr;
    }

    /**
     * Visits a ForStatement.
     *
     * @param stmt  The ForStatement to visit.
     * @return      The subclass-specific return value.
     */
    virtual void* visitForStatement(ForStatement* stmt) { return nullptr; }

    /**
     * Visits a Function.
     *
     * @param func  The Function to visit.
     * @return      The subclass-specific return value.
     */
    virtual void* visitFunction(Function* func) { return nullptr; }

    /**
     * Visits a FunctionExpression.
     *
     * @param expr  The FunctionExpression to visit.
     * @return      The subclass-specific return value.
     */
    virtual void* visitFunctionExpression(FunctionExpression* expr)
    {
        return nullptr;
    }

    /**
     * Visits a FunctionGroupImport.
     *
     * @param imp  The FunctionGroupImport to visit.
     * @return     The subclass-specific return value.
     */
    virtual void* visitFunctionGroupImport(FunctionGroupImport* imp)
    {
        return nullptr;
    }

    /**
     * Visits a FunctionParameter.
     *
     * @param param  The FunctionParameter to visit.
     * @return       The subclass-specific return value.
     */
    virtual void* visitFunctionParameter(FunctionParameter* param)
    {
        return nullptr;
    }

    /**
     * Visits a FunctionParameterExpression.
     *
     * @param expr  The FunctionParameterExpression to visit.
     * @return      The subclass-specific return value.
     */
    virtual void* visitFunctionParameterExpression(
        FunctionParameterExpression* expr)
    {
        return nullptr;
    }

    /**
     * Visits a FunctionType.
     *
     * @param type  The FunctionType to visit.
     * @return      The subclass-specific return value.
     */
    virtual void* visitFunctionType(FunctionType* type) { return nullptr; }

    /**
     * Visits an IfStatement.
     *
     * @param stmt  The IfStatement to visit.
     * @return      The subclass-specific return value.
     */
    virtual void* visitIfStatement(IfStatement* stmt) { return nullptr; }

    /**
     * Visits an InstanceFunctionExpression.
     *
     * @param expr  The InstanceFunctionExpression to visit.
     * @return      The subclass-specific return value.
     */
    virtual void* visitInstanceFunctionExpression(
        InstanceFunctionExpression* expr)
    {
        return nullptr;
    }

    /**
     * Visits an InstancePropertyGetExpression.
     *
     * @param expr  The InstancePropertyGetExpression to visit.
     * @return      The subclass-specific return value.
     */
    virtual void* visitInstancePropertyGetExpression(
        InstancePropertyGetExpression* expr)
    {
        return nullptr;
    }

    /**
     * Visits an InstancePropertySetExpression.
     *
     * @param expr  The InstancePropertySetExpression to visit.
     * @return      The subclass-specific return value.
     */
    virtual void* visitInstancePropertySetExpression(
        InstancePropertySetExpression* expr)
    {
        return nullptr;
    }

    /**
     * Visits an InstanceVariableExpression.
     *
     * @param expr  The InstanceVariableExpression to visit.
     * @return      The subclass-specific return value.
     */
    virtual void* visitInstanceVariableExpression(
        InstanceVariableExpression* expr)
    {
        return nullptr;
    }

    /**
     * Visits a IntegerLiteral.
     *
     * @param lit  The IntegerLiteral to visit.
     * @return     The subclass-specific return value.
     */
    virtual void* visitIntegerLiteral(IntegerLiteral* lit) { return nullptr; }

    /**
     * Visits a IntegerType.
     *
     * @param type  The IntegerType to visit.
     * @return      The subclass-specific return value.
     */
    virtual void* visitIntegerType(IntegerType* type) { return nullptr; }

    /**
     * Visits a LogicalExpression.
     *
     * @param param  The LogicalExpression to visit.
     * @return       The subclass-specific return value.
     */
    virtual void* visitLogicalExpression(LogicalExpression* expr)
    {
        return nullptr;
    }

    /**
     * Visits a LogicalNotExpression.
     *
     * @param param  The LogicalNotExpression to visit.
     * @return       The subclass-specific return value.
     */
    virtual void* visitLogicalNotExpression(LogicalNotExpression* expr)
    {
        return nullptr;
    }

    /**
     * Visits a MemberAccessExpression.
     *
     * @param expr  The MemberAccessExpression to visit.
     * @return      The subclass-specific return value.
     */
    virtual void* visitMemberAccessExpression(MemberAccessExpression* expr)
    {
        return nullptr;
    }

    /**
     * Visits a Module.
     *
     * @param m  The Module to visit.
     * @return   The subclass-specific return value.
     */
    virtual void* visitModule(Module* m) { return nullptr; }

    /**
     * Visits a ModuleExpression.
     *
     * @param expr  The ModuleExpression to visit.
     * @return      The subclass-specific return value.
     */
    virtual void* visitModuleExpression(ModuleExpression* expr) { return nullptr; }

    /**
     * Visits a ModuleImport.
     *
     * @param imp  The ModuleImport to visit.
     * @return     The subclass-specific return value.
     */
    virtual void* visitModuleImport(ModuleImport* imp) { return nullptr; }

    /**
     * Visits an ObjectCreationExpression.
     *
     * @param expr  The ObjectCreationExpression to visit.
     * @return      The subclass-specific return value.
     */
    virtual void* visitObjectCreationExpression(ObjectCreationExpression* expr)
    {
        return nullptr;
    }

    /**
     * Visits a Property.
     *
     * @param prop  The Property to visit.
     * @return      The subclass-specific return value.
     */
    virtual void* visitProperty(Property* prop) { return nullptr; }

    /**
     * Visits a PropertyGetAccessor.
     *
     * @param acc  The PropertyGetAccessor to visit.
     * @return     The subclass-specific return value.
     */
    virtual void* visitPropertyGetAccessor(PropertyGetAccessor* acc)
    {
        return nullptr;
    }

    /**
     * Visits a PropertySetAccessor.
     *
     * @param acc  The PropertySetAccessor to visit.
     * @return     The subclass-specific return value.
     */
    virtual void* visitPropertySetAccessor(PropertySetAccessor* acc)
    {
        return nullptr;
    }

    /**
     * Visits a ReturnStatement.
     *
     * @param stmt  The ReturnStatement to visit.
     * @return      The subclass-specific return value.
     */
    virtual void* visitReturnStatement(ReturnStatement* stmt) { return nullptr; }

    /**
     * Visits a StructType.
     *
     * @param type  The StructType to visit.
     * @return      The subclass-specific return value.
     */
    virtual void* visitStructType(StructType* type) { return nullptr; }

    /**
     * Visits a ThisExpression.
     *
     * @param expr  The ThisExpression to visit.
     * @return      The subclass-specific return value.
     */
    virtual void* visitThisExpression(ThisExpression* expr) { return nullptr; }

    /**
     * Visits a TypeExpression.
     *
     * @param expr  The TypeExpression to visit.
     * @return      The subclass-specific return value.
     */
    virtual void* visitTypeExpression(TypeExpression* expr) { return nullptr; }

    /**
     * Visits an UnknownType.
     *
     * @param type  The UnknownType to visit.
     * @return      The subclass-specific return value.
     */
    virtual void* visitUnknownType(UnknownType* type) { return nullptr; }

    /**
     * Visits an UnknownArrayType.
     *
     * @param type  The UnknownArrayType to visit.
     * @return      The subclass-specific return value.
     */
    virtual void* visitUnknownArrayType(UnknownArrayType* type) { return nullptr; }

    /**
     * Visits an UnknownFunctionType.
     *
     * @param type  The UnknownFunctionType to visit.
     * @return      The subclass-specific return value.
     */
    virtual void* visitUnknownFunctionType(UnknownFunctionType* type)
    {
        return nullptr;
    }

    /**
     * Visits an UnresolvedBaseConstructorInitializer.
     *
     * @param init  The UnresolvedBaseConstructorInitializer to visit.
     * @return      The subclass-specific return value.
     */
    virtual void* visitUnresolvedBaseConstructorInitializer(
        UnresolvedBaseConstructorInitializer* init)
    {
        return nullptr;
    }

    /**
     * Visits an UnresolvedBinaryExpression.
     *
     * @param expr  The UnresolvedBinaryExpression to visit.
     * @return      The subclass-specific return value.
     */
    virtual void* visitUnresolvedBinaryExpression(
        UnresolvedBinaryExpression* expr)
    {
        return nullptr;
    }

    /**
     * Visits an UnresolvedConstructorInitializer.
     *
     * @param init  The UnresolvedConstructorInitializer to visit.
     * @return      The subclass-specific return value.
     */
    virtual void* visitUnresolvedConstructorInitializer(
        UnresolvedConstructorInitializer* init)
    {
        return nullptr;
    }

    /**
     * Visits an UnresolvedDefaultConstructorInitializer.
     *
     * @param init  The UnresolvedDefaultConstructorInitializer to visit.
     * @return      The subclass-specific return value.
     */
    virtual void* visitUnresolvedDefaultConstructorInitializer(
        UnresolvedDefaultConstructorInitializer* init)
    {
        return nullptr;
    }

    /**
     * Visits an UnresolvedElementAccessExpression.
     *
     * @param expr  The UnresolvedElementAccessExpression to visit.
     * @return      The subclass-specific return value.
     */
    virtual void* visitUnresolvedElementAccessExpression(
        UnresolvedElementAccessExpression* expr)
    {
        return nullptr;
    }

    /**
     * Visits an UnresolvedEntityImport.
     *
     * @param imp  The UnresolvedEntityImport to visit.
     * @return     The subclass-specific return value.
     */
    virtual void* visitUnresolvedEntityImport(UnresolvedEntityImport* imp)
    {
        return nullptr;
    }

    /**
     * Visits an UnresolvedFunctionExpression.
     *
     * @param expr  The UnresolvedFunctionExpression to visit.
     * @return      The subclass-specific return value.
     */
    virtual void* visitUnresolvedFunctionExpression(
        UnresolvedFunctionExpression* expr)
    {
        return nullptr;
    }

    /**
     * Visits an UnresolvedInstanceFunctionExpression.
     *
     * @param expr  The UnresolvedInstanceFunctionExpression to visit.
     * @return      The subclass-specific return value.
     */
    virtual void* visitUnresolvedInstanceFunctionExpression(
        UnresolvedInstanceFunctionExpression* expr)
    {
        return nullptr;
    }

    /**
     * Visits an UnresolvedInstancePropertyExpression.
     *
     * @param expr  The UnresolvedInstancePropertyExpression to visit.
     * @return      The subclass-specific return value.
     */
    virtual void* visitUnresolvedInstancePropertyExpression(
        UnresolvedInstancePropertyExpression* expr)
    {
        return nullptr;
    }

    /**
     * Visits an UnresolvedMemberAccessExpression.
     *
     * @param expr  The UnresolvedMemberAccessExpression to visit.
     * @return      The subclass-specific return value.
     */
    virtual void* visitUnresolvedMemberAccessExpression(
        UnresolvedMemberAccessExpression* expr)
    {
        return nullptr;
    }

    /**
     * Visits an UnresolvedModuleImport.
     *
     * @param imp  The UnresolvedModuleImport to visit.
     * @return     The subclass-specific return value.
     */
    virtual void* visitUnresolvedModuleImport(UnresolvedModuleImport* imp)
    {
        return nullptr;
    }

    /**
     * Visits an UnresolvedObjectCreationExpression.
     *
     * @param expr  The UnresolvedObjectCreationExpression to visit.
     * @return      The subclass-specific return value.
     */
    virtual void* visitUnresolvedObjectCreationExpression(
        UnresolvedObjectCreationExpression* expr)
    {
        return nullptr;
    }

    /**
     * Visits a UnresolvedSimpleNameExpression.
     *
     * @param expr  The UnresolvedSimpleNameExpression to visit.
     * @return      The subclass-specific return value.
     */
    virtual void* visitUnresolvedSimpleNameExpression(
        UnresolvedSimpleNameExpression* expr)
    {
        return nullptr;
    }

    /**
     * Visits an UnresolvedThisExpression.
     *
     * @param expr  The UnresolvedThisExpression to visit.
     * @return      The subclass-specific return value.
     */
    virtual void* visitUnresolvedThisExpression(UnresolvedThisExpression* expr)
    {
        return nullptr;
    }

    /**
     * Visits a Variable.
     *
     * @param var  The Variable to visit.
     * @return     The subclass-specific return value.
     */
    virtual void* visitVariable(Variable* var) { return nullptr; }

    /**
     * Visits a VariableExpression.
     *
     * @param expr  The VariableExpression to visit.
     * @return      The subclass-specific return value.
     */
    virtual void* visitVariableExpression(VariableExpression* expr)
    {
        return nullptr;
    }

    /**
     * Visits an VoidType.
     *
     * @param type  The VoidType to visit.
     * @return      The subclass-specific return value.
     */
    virtual void* visitVoidType(VoidType* type) { return nullptr; }

    /**
     * Visits a WhileStatement.
     *
     * @param expr  The WhileStatement to visit.
     * @return      The subclass-specific return value.
     */
    virtual void* visitWhileStatement(WhileStatement* stmt) { return nullptr; }
};

} // namespace ast
} // namespace soyac

#endif
