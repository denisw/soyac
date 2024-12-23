/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _CODE_GENERATOR_HPP
#define _CODE_GENERATOR_HPP

#include "LLVMTypeMapper.hpp"

#include <filesystem>
#include <stdint.h>

#include "LLValueExpression.hpp"
#include <ast/FunctionParameter.hpp>
#include <ast/NamedEntity.hpp>
#include <ast/NodeList.hpp>
#include <ast/UserDefinedType.hpp>
#include <ast/Visitor.hpp>
#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>

namespace soyac {
namespace codegen {

using namespace soyac::ast;
using std::filesystem::path;

/**
 * Translates an abstract syntax tree to its LLVM IR representation.
 */
class CodeGenerator : soyac::ast::Visitor {
public:
    /**
     * Creates a CodeGenerator.
     */
    CodeGenerator(Module* module);

    void toLLVMAssembly(path destination, std::error_code& error);

    void toObjectCode(path destination, std::error_code& error);

protected:
    /**
     * Visits an ArrayCreationExpression.
     *
     * @param expr  The ArrayCreationExpression to visit.
     * @return      The equivalent LLVM value.
     */
    virtual void* visitArrayCreationExpression(ArrayCreationExpression* expr);

    /**
     * Visits an AssignmentExpression.
     *
     * @param expr  The AssignmentExpression to visit.
     * @return      The equivalent LLVM value.
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
     * Visits a BooleanLiteral.
     *
     * @param lit  The BooleanLiteral to visit.
     * @return     The equivalent LLVM value.
     */
    virtual void* visitBooleanLiteral(BooleanLiteral* lit);

    /**
     * Visits a CallExpression.
     *
     * @param expr  The CallExpression to visit.
     * @return      The equivalent LLVM value.
     */
    virtual void* visitCallExpression(CallExpression* expr);

    /**
     * Visits a CastExpression.
     *
     * @param expr  The CastExpression to visit.
     * @return      The equivalent LLVM value.
     */
    virtual void* visitCastExpression(CastExpression* expr);

    /**
     * Visits a CharacterLiteral.
     *
     * @param lit  The CharacterLiteral to visit.
     * @return     The equivalent LLVM value.
     */
    virtual void* visitCharacterLiteral(CharacterLiteral* lit);

    /**
     * Visits a ClassType.
     *
     * @param type  The ClassType to visit.
     * @return      Nothing.
     */
    virtual void* visitClassType(ClassType* type);

    /**
     * Visits a CombinedRelationalExpression.
     *
     * @param expr  The CombinedRelationalExpression to visit.
     * @return      The equivalent LLVM value.
     */
    virtual void* visitCombinedRelationalExpression(
        CombinedRelationalExpression* expr);

    /**
     * Visits an CompoundAssignmentExpression.
     *
     * @param expr  The CompoundAssignmentExpression to visit.
     * @return      The equivalent LLVM value.
     */
    virtual void* visitCompoundAssignmentExpression(
        CompoundAssignmentExpression* expr);

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
     * Visits an EnumValueExpression.
     *
     * @param expr  The EnumValueExpression to visit.
     * @return      The equivalent LLVM value.
     */
    virtual void* visitEnumValueExpression(EnumValueExpression* expr);

    /**
     * Visits an ExpressionStatement.
     *
     * @param stmt  The ExpressionStatement to visit.
     * @return      Nothing.
     */
    virtual void* visitExpressionStatement(ExpressionStatement* stmt);

    /**
     * Visits a FloatingPointLiteral.
     *
     * @param lit  The FloatingPointLiteral to visit.
     * @return     The equivalent LLVM value.
     */
    virtual void* visitFloatingPointLiteral(FloatingPointLiteral* lit);

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
     * Visits a FunctionExpression.
     *
     * @param expr  The FunctionExpression to visit.
     * @return      The equivalent LLVM value.
     */
    virtual void* visitFunctionExpression(FunctionExpression* expr);

    /**
     * Visits a FunctionParameterExpression.
     *
     * @param expr  The FunctionParameterExpression to visit.
     * @return      The equivalent LLVM value.
     */
    virtual void* visitFunctionParameterExpression(
        FunctionParameterExpression* expr);

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
     * @return      The equivalent LLVM value.
     */
    virtual void* visitInstanceFunctionExpression(
        InstanceFunctionExpression* expr);

    /**
     * Visits an InstanceVariableExpression.
     *
     * @param expr  The InstanceVariableExpression to visit.
     * @return      The equivalent LLVM value.
     */
    virtual void* visitInstanceVariableExpression(
        InstanceVariableExpression* expr);

    /**
     * Visits an IntegerLiteral.
     *
     * @param lit  The IntegerLiteral to visit.
     * @return     The equivalent LLVM value.
     */
    virtual void* visitIntegerLiteral(IntegerLiteral* lit);

    /**
     * Visits a LogicalExpression.
     *
     * @param expr  The LogicalNotExpression to visit.
     * @return      The equivalent LLVM value.
     */
    virtual void* visitLogicalExpression(LogicalExpression* expr);

    /**
     * Visits a LogicalNotExpression.
     *
     * @param expr  The LogicalNotExpression to visit.
     * @return      The equivalent LLVM value.
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
     * Visits an ObjectCreationExpression.
     *
     * @param expr  The ObjectCreationExpression to visit.
     * @return      The equivalent LLVM value.
     */
    virtual void* visitObjectCreationExpression(ObjectCreationExpression* expr);

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
     * Visits a ThisExpression.
     *
     * @param expr  The ThisExpression to visit.
     * @return      The equivalent LLVM value.
     */
    virtual void* visitThisExpression(ThisExpression* expr);

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
     * @return      The equivalent LLVM value.
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
    friend class LLValueExpression;

    Module* mModule;

    llvm::LLVMContext mContext;
    llvm::IRBuilder<> mBuilder;
    llvm::Module* mLLVMModule;
    LLVMTypeMapper mTypeMapper;

    llvm::Function* mFunction;
    llvm::Function* mInitFunction;
    DeclaredEntity* mEnclosing;
    bool mLValue;

    /**
     * Visits an LLValueExpression (a CodeGenerator-only node class).
     *
     * @param expr  The LLValueExpression to visit.
     * @return      The equivalent LLVM value.
     */
    void* visitLLValueExpression(LLValueExpression* expr);

    /**
     * Returns the default pointer type.
     */
    llvm::PointerType* pointerType();

    /**
     * Returns the LLVM "size type", that is, the LLVM integer type whose
     * size equals the target architecture's pointer size. In it's current
     * implementation, this method always returns the 'i32' type, but in
     * the future the returned type will depend on the target architecture.
     *
     * @return  The "size type".
     */
    llvm::Type* sizeType();

    /**
     * Returns the default value for the passed type.
     *
     * @param type  The type.
     * @return      The type's default value.
     */
    llvm::Constant* defaultValue(Type* type);

    /**
     * Returns the llvm::Function* instance representing the passed Soya
     * function. If no such instance already exists, it is created and
     * added to the LLVM module.
     *
     * @param func  The Soya function.
     * @return      The corresponding LLVM function.
     */
    llvm::Function* llfunction(Function* func);

    /**
     * Returns a llvm::Function* instance representing a private LLVM
     * function where the passed user-defined type's instance variable
     * initialization code is located.
     *
     * @param type  The type.
     * @return      The type's private initialization function.
     */
    llvm::Function* llinitializer(UserDefinedType* type);

    /**
     * Returns a llvm::Function* instance representing the "allocation
     * function" of the passed class type, which is called by users
     * of the class to allocate memory for a new instance (the user
     * cannot allocate memory itself as it cannot know the class instance
     * size).
     *
     * @param type  The class type.
     * @return      The class type's allocation function.
     */
    llvm::Function* llallocator(ClassType* type);

    /*
     * Given an LLVM value representing a class instance reference,
     * return a value that points to the data of that instance specific
     * to the passed class of which the instance is composed (that is,
     * the instance's concrete class or any of its base classes). The
     * pointed-to data is represented as an LLVM struct as returned
     * by llstructtype() with the passed class type as argument.
     *
     * @param instance  The instance value.
     * @param type      The class type.
     * @param lvalue    Whether the returned value should be an Lvalue
     *                  (needed for class instance initialization).
     * @return          An LLVM pointer value which points to the
     *                  class-specific data of the instance.
     */
    llvm::Value* createGetInstanceData(
        llvm::Value* instance, ClassType* type, bool lvalue = false);

    /**
     * Generates the initializer function (see llinitializer()) for the
     * passed class or struct type.
     *
     * @param type  The type.
     */
    void createInitializer(UserDefinedType* type);

    /**
     * Generates the instructions corresponding to a built-in method call,
     * given the expression that evaluates to the called instance, the
     * name of the built-in method to call, and the argument expressions
     * (if required). The resulting LLVM value is returned.
     *
     * @param operand     The call operand.
     * @param methodName  The built-in method's name.
     * @param argument    The method argument, or null.
     * @param argument2   The second method argument, or null.
     * @return            The resulting LLVM value.
     */
    llvm::Value* createBuiltInMethodCall(Expression* operand,
        const Name& methodName, Expression* argument = NULL,
        Expression* argument2 = NULL);

    /**
     * Generates the instructions corresponding to a built-in property's
     * get accessor call, given the expression that evaluates to the called
     * instance and the name of the built-in property whose get accessor
     * should be called. The resulting LLVM value is returned.
     *
     * @param operand       The call operand.
     * @param propertyName  The built-in method's name.
     * @return              The resulting LLVM value.
     */
    llvm::Value* createBuiltInPropertyCall(
        Expression* operand, const Name& propertyName);

    /**
     * Creates instructions that determine the object size of the passed
     * LLVM type. The resulting size value is then returned.
     *
     * Note that the created instructions only deal with constant data,
     * so they will be optimized away by the LLVM code generator.
     *
     * @param type  The LLVM type whose size should be determined
     * @return      An LLVM value representing the type's object size.
     */
    llvm::Value* createSizeof(llvm::Type* type);

    /**
     * Creates instructions to allocate a garbage-colloected chunk of
     * memory of the specified size and returns an LLVM value representing
     * a pointer to the allocated memory.
     *
     * @param size  The allocated memory chunk's size.
     */
    llvm::Value* createGCMalloc(llvm::Value* size);

    /**
     * Like createGCMalloc(llvm::Value*), but with the allocated memory's
     * size being the size of the passed LLVM type. If a value is specified
     * as the second argument, the allocated memory's size is additionally
     * multiplied by that value.
     *
     * @param type  The type for which memory should be allocated.
     * @param n     The size multiplier.
     */
    llvm::Value* createGCMalloc(llvm::Type* type, llvm::Value* n = nullptr);
};

} // namespace codegen
} // namespace soyac

#endif
