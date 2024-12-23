/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include <cassert>
#include <cstring>
#include <sstream>
#include <vector>

#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/GlobalVariable.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/IntrinsicInst.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/ValueSymbolTable.h>
#include <llvm/MC/TargetRegistry.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/TargetParser/Host.h>

#include "CodeGenerator.h"
#include <ast/ast.h>

#include "mangling.h"

namespace soyac {
namespace codegen {

CodeGenerator::CodeGenerator(Module* module)
    : mModule(module)
    , mLLVMModule(new llvm::Module(module->name().str(), mContext))
    , mTypeMapper(mContext, mLLVMModule->getDataLayout())
    , mBuilder(mContext)
    , mEnclosing(nullptr)
    , mLValue(false)
{
    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmPrinter();
}

void CodeGenerator::toLLVMAssembly(path destination, std::error_code& error)
{
    auto llvmModule = static_cast<llvm::Module*>(visitModule(mModule));

    if (!llvmModule) {
        return;
    }

    llvm::raw_fd_ostream out(destination.string(), error);

    if (error) {
        return;
    }

    llvmModule->print(out, nullptr);
}

void CodeGenerator::toObjectCode(path destination, std::error_code& error)
{
    auto llvmModule = static_cast<llvm::Module*>(visitModule(mModule));

    std::string errorCode;
    auto targetTriple = llvm::sys::getDefaultTargetTriple();
    auto target = llvm::TargetRegistry::lookupTarget(targetTriple, errorCode);

    if (!errorCode.empty()) {
        throw std::runtime_error("Unsupported target");
    }

    llvm::TargetOptions options;
    std::unique_ptr<llvm::TargetMachine> targetMachine {
        target->createTargetMachine(
            targetTriple, "generic", "", options, llvm::Reloc::PIC_)
    };

    llvm::raw_fd_ostream out(destination.string(), error);

    if (error) {
        return;
    }

    llvm::legacy::PassManager passManager;
    targetMachine->addPassesToEmitFile(passManager, out, nullptr,
        llvm::CodeGenFileType::ObjectFile, true, nullptr);

    llvmModule->setTargetTriple(targetTriple);
    llvmModule->setDataLayout(targetMachine->createDataLayout());
    passManager.run(*llvmModule);
}

///// Helper Methods
//////////////////////////////////////////////////////////////

void* CodeGenerator::visitLLValueExpression(LLValueExpression* expr)
{
    return expr->value();
}

llvm::PointerType* CodeGenerator::pointerType()
{
    return llvm::PointerType::getUnqual(mContext);
}

llvm::Type* CodeGenerator::sizeType()
{
    auto& layout = mLLVMModule->getDataLayout();
    return layout.getIntPtrType(mLLVMModule->getContext());
}

llvm::Constant* CodeGenerator::defaultValue(Type* type)
{
    if (type == TYPE_BOOL) {
        return llvm::ConstantInt::getFalse(mContext);
    }

    if (type == TYPE_CHAR) {
        return defaultValue(TYPE_INT);
    }

    if (dynamic_cast<IntegerType*>(type)) {
        return llvm::ConstantInt::get(mTypeMapper.valueType(type), 0, false);
    }

    if (dynamic_cast<FloatingPointType*>(type)) {
        return llvm::ConstantFP::get(mTypeMapper.valueType(type), 0);
    }

    if (auto enumType = dynamic_cast<EnumType*>(type)) {
        auto underlyingType = mTypeMapper.valueType(enumType->underlyingType());
        return llvm::ConstantInt::get(underlyingType, 0, false);
    }

    if (dynamic_cast<ArrayType*>(type) || dynamic_cast<ClassType*>(type)) {
        return llvm::ConstantPointerNull::get(pointerType());
    }

    if (dynamic_cast<StructType*>(type) || dynamic_cast<FunctionType*>(type)) {
        return llvm::ConstantAggregateZero::get(mTypeMapper.valueType(type));
    }

    throw std::runtime_error("Unknown type");
}

llvm::Function* CodeGenerator::llfunction(Function* func)
{
    llvm::Function* llfunc = mLLVMModule->getFunction(mangledName(func));

    if (llfunc == nullptr) {
        std::vector<llvm::Type*> params;
        Type* enclosingType;

        /*
         * If the function is a property accessor, it's enclosing type
         * is not equal to it's directly enclosing declaration, as that
         * is the property to which the accessor belongs. We need to
         * get that property's parent instead.
         */
        if (dynamic_cast<Property*>(func->parent()) != nullptr) {
            enclosingType = dynamic_cast<Type*>(func->parent()->parent());
        } else {
            enclosingType = dynamic_cast<Type*>(func->parent());
        }

        /*
         * If the function is an instance function or an accessor of an
         * instance property, it needs to accept a "this" pointer as the
         * first parameter.
         */
        if (enclosingType != nullptr) {
            params.push_back(pointerType());
        }

        for (Function::parameters_iterator it = func->parameters_begin();
            it != func->parameters_end(); it++) {
            params.push_back(mTypeMapper.valueType((*it)->type()));
        }

        llvm::FunctionType* llftype = llvm::FunctionType::get(
            mTypeMapper.valueType(func->returnType()), params, false);

        llfunc = llvm::Function::Create(llftype,
            llvm::Function::ExternalLinkage, mangledName(func), mLLVMModule);

        /*
         * Because we cannot set the parameter names with
         * llvm::Function::Create(), we need to do this
         * afterwards.
         */

        Function::parameters_iterator p = func->parameters_begin();
        llvm::Function::arg_iterator a = llfunc->arg_begin();

        if (enclosingType != nullptr) {
            a->setName("this");
            a++;
        }

        for (; a != llfunc->arg_end(); p++, a++) {
            a->setName(mangledName(*p));
        }
    }

    return llfunc;
}

llvm::Function* CodeGenerator::llinitializer(UserDefinedType* type)
{
    llvm::Function* llfunc
        = mLLVMModule->getFunction(mangledName(type) + "_init");

    if (llfunc == nullptr) {
        std::vector<llvm::Type*> params;

        llvm::Type* thisType = pointerType();
        params.push_back(thisType);

        llvm::FunctionType* llftype = llvm::FunctionType::get(
            llvm::Type::getVoidTy(mContext), params, false);

        llfunc = llvm::Function::Create(llftype, llvm::Function::PrivateLinkage,
            mangledName(type) + "_init", mLLVMModule);

        (*llfunc->arg_begin()).setName("this");
    }

    return llfunc;
}

llvm::Function* CodeGenerator::llallocator(ClassType* type)
{
    llvm::Function* llfunc
        = mLLVMModule->getFunction(mangledName(type) + "_new");

    if (llfunc == nullptr) {
        std::vector<llvm::Type*> params;

        llvm::FunctionType* llftype = llvm::FunctionType::get(
            mTypeMapper.valueType(type), params, false);

        llfunc
            = llvm::Function::Create(llftype, llvm::Function::ExternalLinkage,
                mangledName(type) + "_new", mLLVMModule);
    }

    return llfunc;
}

llvm::Value* CodeGenerator::createGetInstanceData(
    llvm::Value* instance, ClassType* type, bool lvalue)
{
    unsigned int classIndex = 0;
    ClassType* cls = type;

    while (cls->baseClass()) {
        classIndex++;
        cls = static_cast<ClassType*>(cls->baseClass());
    }

    auto pointerType = llvm::PointerType::getUnqual(mContext);

    llvm::Value* dataPointer = mBuilder.CreateGEP(pointerType, instance,
        {
            llvm::ConstantInt::get(sizeType(), 0),
            llvm::ConstantInt::get(sizeType(), classIndex),
        });

    return lvalue ? dataPointer : mBuilder.CreateLoad(pointerType, dataPointer);
}

void CodeGenerator::createInitializer(UserDefinedType* type)
{
    llvm::Function* tmpFunction = mFunction;
    mFunction = llinitializer(type);

    llvm::BasicBlock* tmpBlock = mBuilder.GetInsertBlock();
    mBuilder.SetInsertPoint(llvm::BasicBlock::Create(mContext, "", mFunction));

    if (auto ctype = dynamic_cast<ClassType*>(type)) {
        llvm::Value* data = createGCMalloc(mTypeMapper.objectType(ctype));

        llvm::Value* thisVal = (llvm::Value*)ThisExpression(ctype).visit(this);

        llvm::Value* dataPtr = createGetInstanceData(thisVal, ctype, true);
        mBuilder.CreateStore(data, dataPtr);
    }

    /*
     * Generate code for initializing all instance variables.
     */

    for (DeclarationBlock::declarations_iterator it
        = type->body()->declarations_begin();
        it != type->body()->declarations_end(); it++) {
        if (dynamic_cast<Variable*>((*it)->declaredEntity()) != nullptr) {
            Variable* var = (Variable*)(*it)->declaredEntity();

            Expression* lh
                = new InstanceVariableExpression(new ThisExpression(type), var);
            Expression* rh;

            if (var->initializer() != nullptr) {
                rh = var->initializer();
            } else {
                rh = new LLValueExpression(
                    var->type(), defaultValue(var->type()));
            }

            AssignmentExpression assign(lh, rh);
            assign.visit(this);
        }
    }

    mBuilder.CreateRetVoid();
    mBuilder.SetInsertPoint(tmpBlock);

    mFunction = tmpFunction;
}

llvm::Value* CodeGenerator::createBuiltInMethodCall(Expression* operand,
    const Name& methodName, Expression* argument, Expression* argument2)
{
    llvm::Value* lh = (llvm::Value*)operand->visit(this);
    llvm::Value* rh;
    llvm::Value* rh2;

    if (argument != nullptr) {
        rh = (llvm::Value*)argument->visit(this);
    }

    if (argument2 != nullptr) {
        rh2 = (llvm::Value*)argument2->visit(this);
    }

    /*
     * Integer Types
     */
    if (dynamic_cast<IntegerType*>(operand->type()) != nullptr) {
        /*
         * If the integer operand's type is not equal to the argument's
         * type, the operand must be cast first.
         */
        if (lh->getType() != rh->getType()) {
            if (((IntegerType*)operand->type())->isSigned()) {
                lh = mBuilder.CreateSExt(lh, rh->getType());
            } else {
                lh = mBuilder.CreateZExt(lh, rh->getType());
            }
        }

        /*
         * plus()
         */
        if (methodName == "plus") {
            return mBuilder.CreateAdd(lh, rh);
        }

        /*
         * minus()
         */
        else if (methodName == "minus") {
            return mBuilder.CreateSub(lh, rh);
        }

        /*
         * mul()
         */
        else if (methodName == "mul") {
            return mBuilder.CreateMul(lh, rh);
        }

        /*
         * div()
         */
        else if (methodName == "div") {
            if (((IntegerType*)operand->type())->isSigned()) {
                return mBuilder.CreateSDiv(lh, rh);
            } else {
                return mBuilder.CreateUDiv(lh, rh);
            }
        }

        /*
         * mod()
         */
        else if (methodName == "mod") {
            if (((IntegerType*)operand->type())->isSigned()) {
                return mBuilder.CreateSRem(lh, rh);
            } else {
                return mBuilder.CreateURem(lh, rh);
            }
        }

        /*
         * lshift()
         */
        else if (methodName == "lshift") {
            return mBuilder.CreateShl(lh, rh);
        }

        /*
         * rshift()
         */
        else if (methodName == "rshift") {
            return mBuilder.CreateAShr(lh, rh);
        }

        /*
         * equals()
         */
        else if (methodName == "equals") {
            return mBuilder.CreateICmpEQ(lh, rh);
        }

        /*
         * lessThan()
         */
        else if (methodName == "lessThan") {
            if (((IntegerType*)operand->type())->isSigned()) {
                return mBuilder.CreateICmpSLT(lh, rh);
            } else {
                return mBuilder.CreateICmpULT(lh, rh);
            }
        }

        /*
         * greaterThan()
         */
        else if (methodName == "greaterThan") {
            if (((IntegerType*)operand->type())->isSigned()) {
                return mBuilder.CreateICmpSGT(lh, rh);
            } else {
                return mBuilder.CreateICmpUGT(lh, rh);
            }
        }

        /*
         * ("!=" operator)
         */
        else if (methodName == "!=") {
            return mBuilder.CreateICmpNE(lh, rh);
        }

        /*
         * ("<=" operator)
         */
        else if (methodName == "<=") {
            if (((IntegerType*)operand->type())->isSigned()) {
                return mBuilder.CreateICmpSLE(lh, rh);
            } else {
                return mBuilder.CreateICmpULE(lh, rh);
            }
        }

        /*
         * (">=" operator)
         */
        else if (methodName == ">=") {
            if (((IntegerType*)operand->type())->isSigned()) {
                return mBuilder.CreateICmpSGE(lh, rh);
            } else {
                return mBuilder.CreateICmpUGE(lh, rh);
            }
        }
    }

    /*
     * "bool" Type
     */
    else if (operand->type() == TYPE_BOOL) {
        /*
         * equals()
         */
        if (methodName == "equals") {
            return mBuilder.CreateICmpEQ(lh, rh);
        }

        /*
         * ("!=" operator)
         */
        if (methodName == "!=") {
            return mBuilder.CreateICmpNE(lh, rh);
        }
    }

    /*
     * "char" Type
     */
    else if (operand->type() == TYPE_CHAR) {
        /*
         * equals()
         */
        if (methodName == "equals") {
            return mBuilder.CreateICmpEQ(lh, rh);
        }

        /*
         * ("!=" operator)
         */
        if (methodName == "!=") {
            return mBuilder.CreateICmpNE(lh, rh);
        }
    }

    /*
     * Floating-Point Types
     */
    else if (dynamic_cast<FloatingPointType*>(operand->type()) != nullptr) {
        /*
         * If the integer operand's type is not equal to the argument's
         * type, the operand must be cast first.
         */
        if (lh->getType() != rh->getType()) {
            lh = mBuilder.CreateFPExt(lh, rh->getType());
        }

        /*
         * plus()
         */
        if (methodName == "plus") {
            return mBuilder.CreateAdd(lh, rh);
        }

        /*
         * minus()
         */
        else if (methodName == "minus") {
            return mBuilder.CreateSub(lh, rh);
        }

        /*
         * mul()
         */
        else if (methodName == "mul") {
            return mBuilder.CreateMul(lh, rh);
        }

        /*
         * div()
         */
        else if (methodName == "div") {
            return mBuilder.CreateFDiv(lh, rh);
        }

        /*
         * equals()
         */
        else if (methodName == "equals") {
            return mBuilder.CreateFCmpOEQ(lh, rh);
        }

        /*
         * lessThan()
         */
        else if (methodName == "lessThan") {
            return mBuilder.CreateFCmpOLT(lh, rh);
        }

        /*
         * greaterThan()
         */
        else if (methodName == "greaterThan") {
            return mBuilder.CreateFCmpOGT(lh, rh);
        }

        /*
         * ("!=" operator)
         */
        else if (methodName == "!=") {
            return mBuilder.CreateFCmpONE(lh, rh);
        }

        /*
         * ("<=" operator)
         */
        else if (methodName == "<=") {
            return mBuilder.CreateFCmpOLE(lh, rh);
        }

        /*
         * (">=" operator)
         */
        else if (methodName == ">=") {
            return mBuilder.CreateFCmpOGE(lh, rh);
        }
    }

    /*
     * Array Types
     */
    else if (dynamic_cast<ArrayType*>(operand->type()) != nullptr) {
        auto llvmArrayType = mTypeMapper.objectType(operand->type());
        auto elementType
            = dynamic_cast<ArrayType*>(operand->type())->elementType();

        if (methodName == "getElement") {
            std::vector<llvm::Value*> indices;
            indices.push_back(llvm::ConstantInt::get(sizeType(), 0, false));
            indices.push_back(llvm::ConstantInt::get(sizeType(), 1, false));
            indices.push_back(rh);

            llvm::Value* address
                = mBuilder.CreateGEP(llvmArrayType, lh, indices);

            auto llvmElementType = mTypeMapper.valueType(elementType);
            return mBuilder.CreateLoad(llvmElementType, address);
        } else if (methodName == "setElement") {
            std::vector<llvm::Value*> indices;
            indices.push_back(llvm::ConstantInt::get(sizeType(), 0, false));
            indices.push_back(llvm::ConstantInt::get(sizeType(), 1, false));
            indices.push_back(rh);

            llvm::Value* address
                = mBuilder.CreateGEP(llvmArrayType, lh, indices);

            LLValueExpression lhExpr(elementType, address);
            LLValueExpression rhExpr(elementType, rh2);
            lhExpr.ref();
            rhExpr.ref();

            AssignmentExpression assign(&lhExpr, &rhExpr);
            return (llvm::Value*)assign.visit(this);
        }
    }

    /*
     * Enum Types
     */
    else if (dynamic_cast<EnumType*>(operand->type()) != nullptr) {
        /*
         * The only method that enums have is equals(), so we don't have to
         * check for the method name. (We check with an assertion to be
         * sure, though).
         */
        assert(methodName == "equals");
        return mBuilder.CreateICmpEQ(lh, rh);
    }

    assert(false);
}

llvm::Value* CodeGenerator::createBuiltInPropertyCall(
    Expression* operand, const Name& propertyName)
{
    llvm::Value* op = (llvm::Value*)operand->visit(this);

    /*
     * Array Types
     */
    if (dynamic_cast<ArrayType*>(operand->type()) != nullptr) {
        auto arrayType = mTypeMapper.objectType(operand->type());
        if (propertyName == "length") {
            llvm::Value* lengthField
                = mBuilder.CreateStructGEP(arrayType, op, 0);
            llvm::Value* length = mBuilder.CreateLoad(sizeType(), lengthField);
            return mBuilder.CreateZExt(
                length, llvm::IntegerType::getInt64Ty(mContext));
        }
    }

    assert(false);
}

llvm::Value* CodeGenerator::createSizeof(llvm::Type* type)
{
    llvm::PointerType* ptrType = llvm::PointerType::getUnqual(type);

    llvm::Value* tmp
        = mBuilder.CreateGEP(ptrType, llvm::ConstantPointerNull::get(ptrType),
            llvm::ConstantInt::get(
                llvm::IntegerType::getInt32Ty(mContext), 1, false));

    // FIXME: No 64-bit awareness!
    return mBuilder.CreatePtrToInt(tmp, sizeType());
}

llvm::Value* CodeGenerator::createGCMalloc(llvm::Value* size)
{
    llvm::Type* i8p
        = llvm::PointerType::getUnqual(llvm::IntegerType::getInt8Ty(mContext));

    std::vector<llvm::Value*> args;
    args.push_back(size);

    llvm::Function* gcMalloc = mLLVMModule->getFunction("GC_malloc");

    if (gcMalloc == nullptr) {
        std::vector<llvm::Type*> params;
        params.push_back(sizeType());

        llvm::FunctionType* gcMallocType
            = llvm::FunctionType::get(i8p, params, false);

        gcMalloc = llvm::Function::Create(gcMallocType,
            llvm::Function::ExternalLinkage, "GC_malloc", mLLVMModule);
    }

    return mBuilder.CreateCall(gcMalloc, args);
}

llvm::Value* CodeGenerator::createGCMalloc(llvm::Type* type, llvm::Value* n)
{
    llvm::Value* size = createSizeof(type);
    if (n != nullptr) {
        size = mBuilder.CreateMul(size, n);
    }
    return createGCMalloc(size);
}

///// Module
//////////////////////////////////////////////////////////////////////

void* CodeGenerator::visitModule(soyac::ast::Module* m)
{
    mLLVMModule = new llvm::Module(m->name().str(), mContext);

    std::string initFuncName;
    std::vector<llvm::Type*> params;
    llvm::FunctionType* initFuncType;

    if (m == Module::getProgram()) {
        initFuncName = "main";
        initFuncType = llvm::FunctionType::get(
            llvm::Type::getInt32Ty(mContext), params, false);
    } else {
        initFuncName = mangledName(m) + "_init";
        initFuncType = llvm::FunctionType::get(
            llvm::Type::getVoidTy(mContext), params, false);
    }

    mFunction = mInitFunction = llvm::Function::Create(initFuncType,
        llvm::Function::ExternalLinkage, initFuncName, mLLVMModule);

    llvm::BasicBlock* body = llvm::BasicBlock::Create(mContext, "", mFunction);
    mBuilder.SetInsertPoint(body);

    m->body()->visit(this);

    mBuilder.SetInsertPoint(&mFunction->back());

    if (m == Module::getProgram()) {
        mBuilder.CreateRet(
            llvm::ConstantInt::get(llvm::Type::getInt32Ty(mContext), 0, true));
    } else {
        mBuilder.CreateRetVoid();
    }

    return mLLVMModule;
}

///// Statements
//////////////////////////////////////////////////////////////////

void* CodeGenerator::visitBlock(Block* b)
{
    for (Block::statements_iterator it = b->statements_begin();
        it != b->statements_end(); it++) {
        (*it)->visit(this);
    }

    return nullptr;
}

void* CodeGenerator::visitDeclarationStatement(DeclarationStatement* stmt)
{
    stmt->declaredEntity()->visit(this);
    return nullptr;
}

void* CodeGenerator::visitDoStatement(DoStatement* stmt)
{
    llvm::BasicBlock* start;
    llvm::BasicBlock* end;

    start = llvm::BasicBlock::Create(mContext, "", mFunction);
    mBuilder.CreateBr(start);

    mBuilder.SetInsertPoint(start);
    stmt->body()->visit(this);

    llvm::Value* cond = (llvm::Value*)stmt->condition()->visit(this);

    end = llvm::BasicBlock::Create(mContext, "", mFunction);
    mBuilder.CreateCondBr(cond, start, end);
    mBuilder.SetInsertPoint(end);

    return nullptr;
}

void* CodeGenerator::visitExpressionStatement(ExpressionStatement* stmt)
{
    stmt->expression()->visit(this);
    return nullptr;
}

void* CodeGenerator::visitForStatement(ForStatement* stmt)
{
    llvm::BasicBlock* start;
    llvm::BasicBlock* loop;
    llvm::BasicBlock* end;

    for (ForStatement::initializers_iterator it = stmt->initializers_begin();
        it != stmt->initializers_end(); it++) {
        (*it)->visit(this);
    }

    start = llvm::BasicBlock::Create(mContext, "", mFunction);
    mBuilder.CreateBr(start);

    mBuilder.SetInsertPoint(start);
    llvm::Value* cond = (llvm::Value*)stmt->condition()->visit(this);

    loop = llvm::BasicBlock::Create(mContext, "", mFunction);
    mBuilder.SetInsertPoint(loop);

    stmt->body()->visit(this);

    for (ForStatement::iterators_iterator it = stmt->iterators_begin();
        it != stmt->iterators_end(); it++) {
        (*it)->visit(this);
    }

    mBuilder.CreateBr(start);

    end = llvm::BasicBlock::Create(mContext, "", mFunction);

    mBuilder.SetInsertPoint(start);
    mBuilder.CreateCondBr(cond, loop, end);
    mBuilder.SetInsertPoint(end);

    return nullptr;
}

void* CodeGenerator::visitIfStatement(IfStatement* stmt)
{
    llvm::Value* cond = (llvm::Value*)stmt->condition()->visit(this);

    llvm::BasicBlock* ifTrue
        = llvm::BasicBlock::Create(mContext, "", mFunction);
    llvm::BasicBlock* ifFalse
        = llvm::BasicBlock::Create(mContext, "", mFunction);

    mBuilder.CreateCondBr(cond, ifTrue, ifFalse);

    mBuilder.SetInsertPoint(ifTrue);
    stmt->body()->visit(this);

    if (stmt->elseBody() != nullptr) {
        llvm::BasicBlock* ifEnd
            = llvm::BasicBlock::Create(mContext, "", mFunction);
        mBuilder.CreateBr(ifEnd);

        mBuilder.SetInsertPoint(ifFalse);
        stmt->elseBody()->visit(this);

        mBuilder.CreateBr(ifEnd);
        mBuilder.SetInsertPoint(ifEnd);

    } else {
        mBuilder.CreateBr(ifFalse);
        mBuilder.SetInsertPoint(ifFalse);
    }

    return nullptr;
}

void* CodeGenerator::visitReturnStatement(ReturnStatement* stmt)
{
    if (stmt->returnValue() != nullptr) {
        llvm::Value* retval = (llvm::Value*)stmt->returnValue()->visit(this);
        mBuilder.CreateRet(retval);
    } else {
        mBuilder.CreateRetVoid();
    }

    return nullptr;
}

void* CodeGenerator::visitWhileStatement(WhileStatement* stmt)
{
    llvm::BasicBlock* start;
    llvm::BasicBlock* loop;
    llvm::BasicBlock* end;

    start = llvm::BasicBlock::Create(mContext, "", mFunction);
    mBuilder.CreateBr(start);

    mBuilder.SetInsertPoint(start);
    llvm::Value* cond = (llvm::Value*)stmt->condition()->visit(this);

    loop = llvm::BasicBlock::Create(mContext, "", mFunction);
    mBuilder.SetInsertPoint(loop);
    stmt->body()->visit(this);
    mBuilder.CreateBr(start);

    end = llvm::BasicBlock::Create(mContext, "", mFunction);

    mBuilder.SetInsertPoint(start);
    mBuilder.CreateCondBr(cond, loop, end);
    mBuilder.SetInsertPoint(end);

    return nullptr;
}

///// Declared Entities
///////////////////////////////////////////////////////////

void* CodeGenerator::visitClassType(ClassType* type)
{
    DeclaredEntity* tmpEnclosing = mEnclosing;
    mEnclosing = type;

    createInitializer(type);

    /*
     * Generate code for the allocation function (see llallocator()).
     */

    llvm::Function* tmpFunction = mFunction;
    mFunction = llallocator(type);

    llvm::BasicBlock* body = llvm::BasicBlock::Create(mContext, "", mFunction);
    llvm::BasicBlock* tmpBlock = mBuilder.GetInsertBlock();
    mBuilder.SetInsertPoint(body);

    llvm::Value* ret = createGCMalloc(mTypeMapper.objectType(type));
    mBuilder.CreateRet(ret);

    mBuilder.SetInsertPoint(tmpBlock);
    mFunction = tmpFunction;

    /*
     * Generate code for the class' members.
     */

    for (DeclarationBlock::declarations_iterator it
        = type->body()->declarations_begin();
        it != type->body()->declarations_end(); it++) {
        /*
         * Don't visit member variables, their initialization code has
         * already been taken care of in createInitializer().
         */
        if (dynamic_cast<Variable*>((*it)->declaredEntity()) == nullptr) {
            (*it)->visit(this);
        }
    }

    mEnclosing = tmpEnclosing;
    return nullptr;
}

void* CodeGenerator::visitConstructor(Constructor* cons)
{
    visitFunction(cons);

    /*
     * In addition to the code generated in visitFunction(), the constructor
     * must start with executing the constructor's initializer, if any.
     */

    llvm::Function* tmpFunction = mFunction;
    mFunction = llfunction(cons);

    llvm::BasicBlock* tmpBlock = mBuilder.GetInsertBlock();
    llvm::BasicBlock& first = *mFunction->begin();
    mBuilder.SetInsertPoint(&first, first.begin());

    if (cons->initializer() != nullptr) {
        ConstructorInitializer* init = cons->initializer();

        Expression* instance = new ThisExpression((Type*)cons->parent());
        Expression* callee
            = new InstanceFunctionExpression(instance, init->constructor());

        CallExpression call(
            callee, init->arguments_begin(), init->arguments_end());

        call.visit(this);

        /*
         * If the initializer hasn't called another constructor of the same
         * class or struct, the code generated below has already been
         * executed in that constructor, so it doesn't have to be generated
         * for this one.
         */
        if (init->constructor()->parent() == cons->parent()) {
            mBuilder.SetInsertPoint(tmpBlock);
            mFunction = tmpFunction;
            return nullptr;
        }
    }

    /*
     * Generate code to call the initialization function of the enclosing
     * struct or class. (See llinitializer().)
     */

    std::vector<llvm::Value*> args;
    ThisExpression texpr((Type*)cons->parent());
    args.push_back((llvm::Value*)texpr.visit(this));

    mBuilder.CreateCall(llinitializer((UserDefinedType*)cons->parent()), args);

    mBuilder.SetInsertPoint(tmpBlock);
    mFunction = tmpFunction;

    return nullptr;
}

void* CodeGenerator::visitConstructorInitializer(ConstructorInitializer* init)
{
    return nullptr;
}

void* CodeGenerator::visitFunction(Function* func)
{
    llvm::Function* tmpFunction = mFunction;
    mFunction = llfunction(func);

    if (func->body() != nullptr) {
        llvm::BasicBlock* body
            = llvm::BasicBlock::Create(mContext, "", mFunction);

        llvm::BasicBlock* tmp = mBuilder.GetInsertBlock();
        mBuilder.SetInsertPoint(body);

        func->body()->visit(this);

        llvm::BasicBlock* last = mBuilder.GetInsertBlock();

        if (last->empty() || !last->back().isTerminator()) {
            if (func->returnType() == TYPE_VOID) {
                mBuilder.CreateRetVoid();
            } else {
                mBuilder.CreateUnreachable();
            }
        }

        mBuilder.SetInsertPoint(tmp);
    }

    mFunction = tmpFunction;
    return nullptr;
}

void* CodeGenerator::visitProperty(Property* prop)
{
    if (prop->getAccessor() != nullptr) {
        prop->getAccessor()->visit(this);
    }

    if (prop->setAccessor() != nullptr) {
        prop->setAccessor()->visit(this);
    }

    return nullptr;
}

void* CodeGenerator::visitPropertyGetAccessor(PropertyGetAccessor* acc)
{
    return visitFunction(acc);
}

void* CodeGenerator::visitPropertySetAccessor(PropertySetAccessor* acc)
{
    return visitFunction(acc);
}

void* CodeGenerator::visitStructType(StructType* type)
{
    DeclaredEntity* tmpEnclosing = mEnclosing;
    mEnclosing = type;

    createInitializer(type);

    for (DeclarationBlock::declarations_iterator it
        = type->body()->declarations_begin();
        it != type->body()->declarations_end(); it++) {
        /*
         * Don't visit member variables, their initialization code has
         * already been taken care of in createInitializer().
         */
        if (dynamic_cast<Variable*>((*it)->declaredEntity()) == nullptr) {
            (*it)->visit(this);
        }
    }

    mEnclosing = tmpEnclosing;
    return nullptr;
}

void* CodeGenerator::visitVariable(Variable* var)
{
    llvm::Value* v;

    if (mFunction == mInitFunction) {
        v = new llvm::GlobalVariable(*mLLVMModule,
            mTypeMapper.valueType(var->type()), false,
            llvm::GlobalVariable::ExternalLinkage, defaultValue(var->type()),
            mangledName(var));
    } else {
        v = mBuilder.CreateAlloca(
            mTypeMapper.valueType(var->type()), nullptr, mangledName(var).c_str());

        if (var->initializer() == nullptr) {
            Expression* rh
                = new LLValueExpression(var->type(), defaultValue(var->type()));

            AssignmentExpression assign(new VariableExpression(var), rh);
            assign.visit(this);
        }
    }

    if (var->initializer() != nullptr) {
        AssignmentExpression assign(
            new VariableExpression(var), var->initializer());
        assign.visit(this);
    }

    return nullptr;
}

///// Expressions
/////////////////////////////////////////////////////////////////

void* CodeGenerator::visitArrayCreationExpression(ArrayCreationExpression* expr)
{
    /*
     * Generate code to evaluate all element expressions (if any).
     */

    std::vector<llvm::Value*> elems;

    for (ArrayCreationExpression::elements_iterator it = expr->elements_begin();
        it != expr->elements_end(); it++) {
        elems.push_back((llvm::Value*)(*it)->visit(this));
    }

    /*
     * Generate code for the array length evaluation.
     */

    llvm::Value* len;

    if (expr->lengthExpression() != nullptr) {
        len = (llvm::Value*)expr->lengthExpression()->visit(this);
    } else {
        len = llvm::ConstantInt::get(sizeType(), elems.size(), false);
    }

    /*
     * Create a call to GC_malloc() to allocate the space needed for
     * the array.
     */

    ArrayType* arrayType = (ArrayType*)expr->type();
    llvm::Type* llvmArrayType = mTypeMapper.objectType(arrayType);

    llvm::Value* allocSize
        = mBuilder.CreateMul(createSizeof(llvmArrayType), len);
    allocSize = mBuilder.CreateAdd(allocSize, createSizeof(sizeType()));

    llvm::Value* array = createGCMalloc(allocSize);

    /*
     * Finally, the array length and all element values need to be
     * copied into the array.
     */

    llvm::Value* lengthField
        = mBuilder.CreateStructGEP(llvmArrayType, array, 0);
    llvm::Value* dataField = mBuilder.CreateStructGEP(llvmArrayType, array, 1);

    mBuilder.CreateStore(len, lengthField);

    Type* elementType = arrayType->elementType();
    llvm::Type* llvmElementType = mTypeMapper.valueType(elementType);

    for (size_t index = 0; index < elems.size(); index++) {
        std::vector<llvm::Value*> indices;
        indices.push_back(llvm::ConstantInt::get(sizeType(), 0, false));
        indices.push_back(llvm::ConstantInt::get(sizeType(), index, false));

        llvm::Value* elemLH
            = mBuilder.CreateGEP(llvmElementType, dataField, indices);

        Expression* lh
            = new LLValueExpression(arrayType->elementType(), elemLH);
        Expression* rh
            = new LLValueExpression(arrayType->elementType(), elems[index]);

        AssignmentExpression assign(lh, rh);
        assign.visit(this);
    }

    return array;
}

void* CodeGenerator::visitAssignmentExpression(AssignmentExpression* expr)
{
    mLValue = true;
    llvm::Value* lh = (llvm::Value*)expr->leftHand()->visit(this);
    mLValue = false;

    llvm::Value* rh = (llvm::Value*)expr->rightHand()->visit(this);

    mBuilder.CreateStore(rh, lh);
    return rh;
}

void* CodeGenerator::visitBooleanLiteral(BooleanLiteral* lit)
{
    if (lit->value() == true) {
        return llvm::ConstantInt::getTrue(mContext);
    } else {
        return llvm::ConstantInt::getFalse(mContext);
    }
}

void* CodeGenerator::visitCallExpression(CallExpression* expr)
{
    /*
     * Check if this is a call to a built-in function or property. If it is,
     * create the corresponding code using createBuiltInMethodCall() or
     * createBuiltInPropertyCall(), respectively.
     */
    if (dynamic_cast<InstanceFunctionExpression*>(expr->callee()) != nullptr) {
        InstanceFunctionExpression* funcExpr
            = (InstanceFunctionExpression*)expr->callee();

        if (dynamic_cast<BuiltInType*>(funcExpr->instance()->type()) != nullptr
            || dynamic_cast<ArrayType*>(funcExpr->instance()->type()) != nullptr
            || dynamic_cast<EnumType*>(funcExpr->instance()->type()) != nullptr) {
            if (dynamic_cast<PropertyGetAccessor*>(funcExpr->target())
                != nullptr) {
                return createBuiltInPropertyCall(
                    funcExpr->instance(), funcExpr->target()->parent()->name());
            } else {
                Expression* arg1 = *expr->arguments_begin();
                Expression* arg2 = (funcExpr->target()->name() == "setElement")
                    ? *(++expr->arguments_begin())
                    : nullptr;

                return createBuiltInMethodCall(funcExpr->instance(),
                    funcExpr->target()->name(), arg1, arg2);
            }
        }
    }

    /*
     * Otherwise, we create an LLVM 'call' instruction that
     * matches the call expression.
     */

    std::vector<llvm::Value*> args;

    for (CallExpression::arguments_iterator it = expr->arguments_begin();
        it != expr->arguments_end(); it++) {
        args.push_back((llvm::Value*)(*it)->visit(this));
    }

    /*
     * If the call is indirect (to a function reference rather than to a
     * concrete function), we need to generate some logic to call the
     * denoted function correctly depending on whether the reference has an
     * associated environment (like a "this" reference) or not.
     */
    if (!dynamic_cast<FunctionExpression*>(expr->callee())
        && !dynamic_cast<InstanceFunctionExpression*>(expr->callee())) {
        llvm::Value* funcVal = (llvm::Value*)expr->callee()->visit(this);
        llvm::Value* fptr = mBuilder.CreateExtractValue(funcVal, 0);
        llvm::Value* env = mBuilder.CreateExtractValue(funcVal, 1);

        auto functionType = dynamic_cast<FunctionType*>(expr->callee()->type());
        assert(functionType != nullptr);

        auto llvmFunctionType = static_cast<llvm::FunctionType*>(
            mTypeMapper.objectType(functionType));

        llvm::BasicBlock* withEnv
            = llvm::BasicBlock::Create(mContext, "", mFunction);
        llvm::BasicBlock* withoutEnv
            = llvm::BasicBlock::Create(mContext, "", mFunction);
        llvm::BasicBlock* end
            = llvm::BasicBlock::Create(mContext, "", mFunction);

        llvm::Value* envNull = mBuilder.CreateIsNull(env);
        mBuilder.CreateCondBr(envNull, withoutEnv, withEnv);

        mBuilder.SetInsertPoint(withoutEnv);
        llvm::Value* ret1 = mBuilder.CreateCall(llvmFunctionType, fptr, args);
        mBuilder.CreateBr(end);

        mBuilder.SetInsertPoint(withEnv);
        std::vector<llvm::Type*> params(
            llvmFunctionType->param_begin(), llvmFunctionType->param_end());
        params.insert(params.begin(), pointerType());
        args.insert(args.begin(), env);
        llvm::Value* ret2 = mBuilder.CreateCall(llvmFunctionType, fptr, args);
        mBuilder.CreateBr(end);

        mBuilder.SetInsertPoint(end);

        auto returnType = mTypeMapper.valueType(functionType->returnType());
        llvm::PHINode* phi = mBuilder.CreatePHI(returnType, 2);
        phi->addIncoming(ret1, withoutEnv);
        phi->addIncoming(ret2, withEnv);

        return phi;
    }
    /*
     * If the call is direct, a simple LLVM 'call' instruction does the job.
     */
    else {
        llvm::Value* callee;

        if (dynamic_cast<FunctionExpression*>(expr->callee()) != nullptr) {
            FunctionExpression* fexpr = (FunctionExpression*)expr->callee();
            callee = llfunction(fexpr->target());
        } else if (dynamic_cast<InstanceFunctionExpression*>(expr->callee())
            != nullptr) {
            InstanceFunctionExpression* instanceFunc
                = (InstanceFunctionExpression*)expr->callee();

            callee = llfunction(instanceFunc->target());

            llvm::Value* instanceVal;

            if (dynamic_cast<StructType*>(instanceFunc->instance()->type())
                != nullptr) {
                bool tmp = mLValue;
                mLValue = true;

                instanceVal
                    = (llvm::Value*)instanceFunc->instance()->visit(this);

                mLValue = tmp;

            } else {
                instanceVal
                    = (llvm::Value*)instanceFunc->instance()->visit(this);

                llvm::Type* instanceType = ((llvm::Function*)callee)
                                               ->getFunctionType()
                                               ->getParamType(0);

                if (instanceVal->getType() != instanceType) {
                    instanceVal
                        = mBuilder.CreateBitCast(instanceVal, instanceType);
                }
            }

            args.insert(args.begin(), instanceVal);
        }

        return mBuilder.CreateCall(
            static_cast<llvm::FunctionType*>(
                mTypeMapper.objectType(expr->callee()->type())),
            callee, args);
    }
}

void* CodeGenerator::visitCastExpression(CastExpression* expr)
{
    llvm::Value* op = (llvm::Value*)expr->operand()->visit(this);
    llvm::Type* ctype = mTypeMapper.valueType(expr->type());

    /*
     * If the internal representation of the source and destination type
     * are equal, the cast is a no-op.
     */
    if (op->getType() == ctype) {
        return op;
    }

    else if (llvm::isa<llvm::IntegerType>(ctype)) {
        /*
         * Integer => Integer
         */
        if ((llvm::isa<llvm::IntegerType>(op->getType()))) {
            const llvm::IntegerType* op_itype
                = llvm::cast<llvm::IntegerType>(op->getType());
            const llvm::IntegerType* c_itype
                = llvm::cast<llvm::IntegerType>(ctype);

            if (c_itype->getBitWidth() < op_itype->getBitWidth()) {
                return mBuilder.CreateTrunc(op, ctype);
            } else if (((IntegerType*)expr->operand()->type())->isSigned()) {
                return mBuilder.CreateSExt(op, ctype);
            } else {
                return mBuilder.CreateZExt(op, ctype);
            }
        }

        /*
         * Integer => Floating-Point
         */
        else if (op->getType() == llvm::Type::getFloatTy(mContext)
            || op->getType() == llvm::Type::getDoubleTy(mContext)) {
            if (((IntegerType*)expr->type())->isSigned()) {
                return mBuilder.CreateFPToSI(op, ctype);
            } else {
                return mBuilder.CreateFPToUI(op, ctype);
            }
        }
    }

    else if (ctype == llvm::Type::getFloatTy(mContext)
        || ctype == llvm::Type::getDoubleTy(mContext)) {
        /*
         * 'double' => 'float'
         */
        if (op->getType() == llvm::Type::getFloatTy(mContext)) {
            return mBuilder.CreateFPExt(op, ctype);
        }

        /*
         * 'float' => 'double'
         */
        else if (op->getType() == llvm::Type::getDoubleTy(mContext)) {
            return mBuilder.CreateFPTrunc(op, ctype);
        }

        /*
         * Floating-Point => Integer
         */
        else if (llvm::isa<llvm::IntegerType>(op->getType())) {
            if (((IntegerType*)expr->operand()->type())->isSigned()) {
                return mBuilder.CreateSIToFP(op, ctype);
            } else {
                return mBuilder.CreateUIToFP(op, ctype);
            }
        }
    }

    /*
     * Reference conversions
     */
    else {
        return mBuilder.CreateBitCast(op, ctype);
    }

    assert(false);
}

void* CodeGenerator::visitCharacterLiteral(CharacterLiteral* lit)
{
    return llvm::ConstantInt::get(
        llvm::IntegerType::getInt32Ty(mContext), lit->value(), false);
}

void* CodeGenerator::visitCombinedRelationalExpression(
    CombinedRelationalExpression* expr)
{
    /*
     * If the left-hand operand is a built-in type, we can generate
     * optimized code for the combined relational expression using
     * createBuiltInMethodCall().
     */
    if (dynamic_cast<BuiltInType*>(expr->leftHand()->type()) != nullptr) {
        Name methodName = (expr->comparisonCallee()->name() == Name("lessThan"))
            ? Name("<=")
            : Name(">=");

        return createBuiltInMethodCall(
            expr->leftHand(), methodName, expr->rightHand());

    }
    /*
     * Otherwise, the expression is transformed into a combination of calls
     * to the left-hand operand's equals() and lessThan()/greaterThan()
     * methods.
     * ('a <= b ' is the same as 'a.equals(b) && a.lessThan(b)';
     *  'a >= b ' is the same as 'a.equals(b) && a.greaterThan(b)'.)
     */
    else {
        llvm::Value* lh = (llvm::Value*)expr->leftHand()->visit(this);
        llvm::Value* rh = (llvm::Value*)expr->rightHand()->visit(this);

        Expression* lhExpr
            = new LLValueExpression(expr->leftHand()->type(), lh);
        Expression* rhExpr
            = new LLValueExpression(expr->rightHand()->type(), rh);

        Expression* callee1 = new InstanceFunctionExpression(
            lhExpr, (Function*)expr->equalsCallee());
        Expression* callee2 = new InstanceFunctionExpression(
            lhExpr, (Function*)expr->comparisonCallee());

        Expression* call1 = new CallExpression(callee1, &rhExpr, &rhExpr + 1);
        Expression* call2 = new CallExpression(callee2, &rhExpr, &rhExpr + 1);

        Expression* orExpr
            = new LogicalExpression(LogicalExpression::OR, call1, call2);

        void* ret = orExpr->visit(this);
        delete orExpr;
        return ret;
    }
}

void* CodeGenerator::visitCompoundAssignmentExpression(
    CompoundAssignmentExpression* expr)
{
    // FIXME: Support for other callables
    assert(dynamic_cast<Function*>(expr->callee()) != nullptr);

    /*
     * Generate code for the left-hand expression and wrap the resulting
     * LLVM value into an LLValueExprression. That way, we ensure that
     * the left-hand expression is evaluated only once.
     */

    bool tmp = mLValue;
    mLValue = true;
    llvm::Value* lh = (llvm::Value*)expr->leftHand()->visit(this);
    mLValue = tmp;

    Expression* lhExpr = new LLValueExpression(expr->type(), lh);

    /*
     * Create a CallExpression that represents the operator method call.
     */
    Expression* instance;

    if (dynamic_cast<StructType*>(expr->leftHand()) != nullptr) {
        instance = lhExpr;
    } else {
        instance = new LLValueExpression(lhExpr->type(),
            mBuilder.CreateLoad(mTypeMapper.valueType(lhExpr->type()), lh));
    }

    Expression* callee
        = new InstanceFunctionExpression(instance, (Function*)expr->callee());

    Expression* arg = expr->rightHand();
    Expression* call = new CallExpression(callee, &arg, &arg + 1);

    /*
     * Let visitAssignmentExpression() do the rest.
     */
    AssignmentExpression assign(lhExpr, call);
    return assign.visit(this);
}

void* CodeGenerator::visitEnumValueExpression(EnumValueExpression* expr)
{
    return llvm::ConstantInt::get(mTypeMapper.valueType(expr->type()),
        expr->target()->value().data(), expr->target()->value().isSigned());
}

void* CodeGenerator::visitFloatingPointLiteral(FloatingPointLiteral* lit)
{
    return llvm::ConstantFP::get(
        mTypeMapper.valueType(lit->type()), lit->value().data());
}

void* CodeGenerator::visitFunctionExpression(FunctionExpression* expr)
{
    auto funcType = mTypeMapper.valueType(expr->type());
    auto funcVal = mBuilder.CreateAlloca(funcType);

    auto fptrField = mBuilder.CreateStructGEP(funcType, funcVal, 0);
    auto fptr = llfunction(expr->target());
    mBuilder.CreateStore(fptr, fptrField);

    auto envField = mBuilder.CreateStructGEP(funcType, funcVal, 1);
    auto env = llvm::ConstantPointerNull::get(pointerType());
    mBuilder.CreateStore(env, envField);

    return mBuilder.CreateLoad(funcType, funcVal);
}

void* CodeGenerator::visitFunctionParameterExpression(
    FunctionParameterExpression* expr)
{
    return mFunction->getValueSymbolTable()->lookup(
        mangledName(expr->target()));
}

void* CodeGenerator::visitInstanceFunctionExpression(
    InstanceFunctionExpression* expr)
{
    auto funcType = mTypeMapper.valueType(expr->type());
    auto funcVal = mBuilder.CreateAlloca(funcType);

    auto fptrField = mBuilder.CreateStructGEP(funcType, funcVal, 0);
    auto fptr = llfunction(expr->target());
    mBuilder.CreateStore(fptr, fptrField);

    auto envField = mBuilder.CreateStructGEP(funcType, funcVal, 1);
    mLValue = true;
    auto env = (llvm::Value*)expr->instance()->visit(this);
    mLValue = false;
    mBuilder.CreateStore(env, envField);

    return mBuilder.CreateLoad(funcType, funcVal);
}

void* CodeGenerator::visitInstanceVariableExpression(
    InstanceVariableExpression* expr)
{
    bool tmp = mLValue;
    mLValue = true;
    llvm::Value* instanceVal = (llvm::Value*)expr->instance()->visit(this);
    mLValue = tmp;

    UserDefinedType* utype
        = dynamic_cast<UserDefinedType*>(expr->instance()->type());

    assert(utype != nullptr);

    /*
     * If the instance's type is a class, we need to retrieve the correct
     * instance data pointer fist (see classPrivate()).
     */
    if (auto classType = dynamic_cast<ClassType*>(utype)) {
        instanceVal = createGetInstanceData(instanceVal, classType);
    }

    /*
     * To retrieve the right field from the struct or class, we need to know
     * its index. Because the order of fields in the representing LLVM
     * struct is the same as the instance variable declaration order in the
     * Soya struct body, we get the index by looking where the variable to
     * retrieve is placed in the struct's or class' body.
     */

    int index = 0;

    for (DeclarationBlock::declarations_iterator it
        = utype->body()->declarations_begin();
        it != utype->body()->declarations_end(); it++) {
        if ((*it)->declaredEntity() == expr->target()) {
            break;
        } else {
            index++;
        }
    }

    /*
     * Now that we have the index, we can retrieve the right field, using
     * either an LLVM 'getelementptr' instruction if the instance value is
     * a struct pointer, or 'extractvalue' if it is a direct struct value.
     */

    llvm::Value* result;

    if (llvm::isa<llvm::PointerType>(instanceVal->getType())) {
        auto instanceType = mTypeMapper.objectType(expr->instance()->type());
        result = mBuilder.CreateStructGEP(instanceType, instanceVal, index);

        if (!mLValue) {
            auto varType = mTypeMapper.valueType(expr->type());
            result = mBuilder.CreateLoad(varType, result);
        }
    } else {
        result = mBuilder.CreateExtractValue(instanceVal, index);
    }

    return result;
}

void* CodeGenerator::visitIntegerLiteral(IntegerLiteral* lit)
{
    return llvm::ConstantInt::get(mTypeMapper.valueType(lit->type()),
        lit->value().data(), lit->value().isSigned());
}

void* CodeGenerator::visitLogicalExpression(LogicalExpression* expr)
{
    llvm::Value* lh = (llvm::Value*)expr->leftHand()->visit(this);

    llvm::BasicBlock* previous = mBuilder.GetInsertBlock();

    llvm::BasicBlock* rhNeeded
        = llvm::BasicBlock::Create(mContext, "", mFunction);
    llvm::BasicBlock* end = llvm::BasicBlock::Create(mContext, "", mFunction);

    if (expr->kind() == LogicalExpression::AND) {
        mBuilder.CreateCondBr(lh, rhNeeded, end);
    } else {
        mBuilder.CreateCondBr(lh, end, rhNeeded);
    }

    mBuilder.SetInsertPoint(rhNeeded);
    llvm::Value* rh = (llvm::Value*)expr->rightHand()->visit(this);
    mBuilder.CreateBr(end);

    mBuilder.SetInsertPoint(end);

    llvm::PHINode* phi = mBuilder.CreatePHI(lh->getType(), 2);
    phi->addIncoming(lh, previous);
    phi->addIncoming(rh, rhNeeded);

    return phi;
}

void* CodeGenerator::visitLogicalNotExpression(LogicalNotExpression* expr)
{
    if (dynamic_cast<CallExpression*>(expr->operand()) != nullptr) {
        CallExpression* call = (CallExpression*)expr->operand();

        InstanceFunctionExpression* funcExpr
            = dynamic_cast<InstanceFunctionExpression*>(call->callee());

        if (funcExpr != nullptr && funcExpr->target()->name() == Name("equals")
            && dynamic_cast<BuiltInType*>(funcExpr->instance()->type())
                != nullptr) {
            return createBuiltInMethodCall(
                funcExpr->instance(), Name("!="), *call->arguments_begin());
        }
    }

    llvm::Value* op = (llvm::Value*)expr->operand()->visit(this);
    return mBuilder.CreateXor(op, llvm::ConstantInt::getTrue(mContext));
}

void* CodeGenerator::visitObjectCreationExpression(
    ObjectCreationExpression* expr)
{
    if (dynamic_cast<StructType*>(expr->type()) != nullptr
        || dynamic_cast<ClassType*>(expr->type()) != nullptr) {
        llvm::Function* constructorFunc = llfunction(expr->constructor());
        std::vector<llvm::Value*> args;
        llvm::Value* instance;

        /*
         * Allocate memory for the instance to create. If the case of a
         * class instance, we do this by calling the class' allocation
         * function (see llallocator()). For structs, we allocate the memory
         * on the heap using an LLVM "alloca" instruction.
         */
        if (dynamic_cast<ClassType*>(expr->type()) != nullptr) {
            instance
                = mBuilder.CreateCall(llallocator((ClassType*)expr->type()));
        } else {
            instance
                = mBuilder.CreateAlloca(mTypeMapper.valueType(expr->type()));
        }

        args.push_back(instance);

        for (ObjectCreationExpression::arguments_iterator it
            = expr->arguments_begin();
            it != expr->arguments_end(); it++) {
            args.push_back((llvm::Value*)(*it)->visit(this));
        }

        mBuilder.CreateCall(constructorFunc, args);

        if (dynamic_cast<StructType*>(expr->type()) != nullptr && !mLValue) {
            auto instanceType = mTypeMapper.valueType(expr->type());
            instance = mBuilder.CreateLoad(instanceType, instance);
        }

        return instance;
    } else {
        assert(false);
    }
}

void* CodeGenerator::visitThisExpression(ThisExpression* expr)
{
    return mFunction->getValueSymbolTable()->lookup("this");
}

void* CodeGenerator::visitVariableExpression(VariableExpression* expr)
{
    std::string name = mangledName(expr->target());
    llvm::Value* ret = mFunction->getValueSymbolTable()->lookup(name);

    if (ret == nullptr) {
        ret = mLLVMModule->getGlobalVariable(name);
    }

    if (!mLValue) {
        auto type = mTypeMapper.valueType(expr->type());
        ret = mBuilder.CreateLoad(type, ret);
    }

    assert(ret != nullptr);
    return ret;
}

} // namespace codegen
} // namespace soyac
