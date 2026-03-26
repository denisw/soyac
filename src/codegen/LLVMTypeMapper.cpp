/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include "LLVMTypeMapper.h"

#include "ast/ast.h"
#include "mangling.h"

#include <algorithm>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Type.h>

using namespace soyac::ast;

namespace soyac::codegen {

LLVMTypeMapper::LLVMTypeMapper(
    llvm::LLVMContext& context, const llvm::DataLayout& dataLayout)
    : mContext(context)
    , mDataLayout(dataLayout)
{
}

llvm::Type* LLVMTypeMapper::valueType(Type* type)
{
    if (type == TYPE_BOOL) {
        return llvm::Type::getInt1Ty(mContext);
    }

    if (type == TYPE_CHAR) {
        return llvm::Type::getInt32Ty(mContext);
    }

    if (type == TYPE_FLOAT) {
        return llvm::Type::getFloatTy(mContext);
    }

    if (type == TYPE_DOUBLE) {
        return llvm::Type::getDoubleTy(mContext);
    }

    if (type == TYPE_VOID) {
        return llvm::Type::getVoidTy(mContext);
    }

    if (auto integerType = dynamic_cast<IntegerType*>(type)) {
        return llvm::IntegerType::get(mContext, integerType->size());
    }

    if (auto enumType = dynamic_cast<EnumType*>(type)) {
        return valueType(enumType->underlyingType());
    }

    if (dynamic_cast<ArrayType*>(type)) {
        return llvm::PointerType::getUnqual(mContext);
    }

    if (auto structType = dynamic_cast<StructType*>(type)) {
        return getStructOrClassInstanceDataType(structType);
    }

    if (dynamic_cast<FunctionType*>(type)) {
        auto pointerType = llvm::PointerType::getUnqual(mContext);

        std::vector<llvm::Type*> members;
        members.push_back(pointerType); // function pointer
        members.push_back(pointerType); // environment pointer

        return llvm::StructType::get(mContext, members, false);
    }

    /*
     * Class Types
     */
    if (dynamic_cast<ClassType*>(type) != nullptr) {
        return llvm::PointerType::getUnqual(mContext);
    }

    /*
     * Unknown Type
     */
    throw std::runtime_error("Unknown type");
}

llvm::Type* LLVMTypeMapper::objectType(Type* type)
{
    if (auto arrayType = dynamic_cast<ArrayType*>(type)) {
        auto llvmSizeType = mDataLayout.getIntPtrType(mContext);
        auto llvmElementType = valueType(arrayType->elementType());
        auto llvmPayloadType = llvm::ArrayType::get(llvmElementType, 0);
        return llvm::StructType::get(
            mContext, { llvmSizeType, llvmPayloadType });
    }

    if (auto functionType = dynamic_cast<FunctionType*>(type)) {
        auto returnType = valueType(functionType->returnType());
        std::vector<llvm::Type*> params;

        for (FunctionType::parameterTypes_iterator it
            = functionType->parameterTypes_begin();
            it != functionType->parameterTypes_end(); it++) {
            params.push_back(valueType(*it));
        }

        return llvm::FunctionType::get(returnType, params, false);
    }

    if (auto structType = dynamic_cast<StructType*>(type)) {
        return getStructOrClassInstanceDataType(structType);
    }

    if (auto classType = dynamic_cast<ClassType*>(type)) {
        return getStructOrClassInstanceDataType(classType);
    }

    /*
     * Unknown Type
     */
    throw std::runtime_error("Unknown type");
}

llvm::Type* LLVMTypeMapper::getPointerSizeType() const
{
    return mDataLayout.getIntPtrType(mContext);
}

llvm::Type* LLVMTypeMapper::getStructOrClassInstanceDataType(
    UserDefinedType* type)
{
    assert(dynamic_cast<StructType*>(type) || dynamic_cast<ClassType*>(type));

    auto name = mangledName(type);
    auto existingLLVMType = llvm::StructType::getTypeByName(mContext, name);
    if (existingLLVMType) {
        return existingLLVMType;
    }

    std::vector<llvm::Type*> memberTypes;

    if (auto classType = dynamic_cast<ClassType*>(type)) {
        if (auto baseClass = static_cast<ClassType*>(classType->baseClass())) {
            auto llvmBase = getStructOrClassInstanceDataType(baseClass);
            for (unsigned i = 0; i < llvmBase->getStructNumElements(); i++) {
                memberTypes.push_back(llvmBase->getStructElementType(i));
            }
        }
    }

    for (DeclarationBlock::declarations_iterator it
        = type->body()->declarations_begin();
        it != type->body()->declarations_end(); ++it) {
        if (auto var = dynamic_cast<Variable*>((*it)->declaredEntity())) {
            memberTypes.push_back(valueType(var->type()));
        }
    }

    return llvm::StructType::create(mContext, memberTypes, name);
}

} // namespace soyac::codegen
