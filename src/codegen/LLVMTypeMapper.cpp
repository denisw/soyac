/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include "LLVMTypeMapper.hpp"

#include "ast/ast.hpp"
#include "mangling.h"

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
        auto name = mangledName(structType);
        auto llvmType = llvm::StructType::getTypeByName(mContext, name);

        if (!llvmType) {
            UserDefinedType* utype = (UserDefinedType*)type;
            std::vector<llvm::Type*> memberTypes;

            for (DeclarationBlock::declarations_iterator it
                = utype->body()->declarations_begin();
                it != utype->body()->declarations_end(); ++it) {
                if (auto var
                    = dynamic_cast<Variable*>((*it)->declaredEntity())) {
                    memberTypes.push_back(valueType(var->type()));
                }
            }

            llvmType = llvm::StructType::create(memberTypes);
        }

        return llvmType;
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
    else if (dynamic_cast<ClassType*>(type) != NULL) {
        llvm::Type* llvmType
            = llvm::StructType::getTypeByName(mContext, mangledName(type));

        if (llvmType == NULL) {
            /*
             * A class instance is represented by a pointer to an array,
             * where each element in return points to the instance data
             * of each class the instance is composed of (that is, the
             * instance's class and all base classes).
             */

            int numClasses = 1;

            ClassType* ctype = (ClassType*)type;

            while (ctype->baseClass() != NULL) {
                numClasses++;
                ctype = (ClassType*)ctype->baseClass();
            }

            llvm::Type* i8 = llvm::Type::getInt8Ty(mContext);
            llvm::Type* i8p = llvm::PointerType::getUnqual(i8);

            llvm::Type* refType = llvm::PointerType::getUnqual(
                llvm::ArrayType::get(i8p, numClasses));

            llvm::StructType* wrapperStruct = llvm::StructType::get(refType);

            wrapperStruct->setName(mangledName(type));
            llvmType = wrapperStruct;
        }

        return llvmType;
    }

    /*
     * Unknown Type
     */
    else {
        throw std::runtime_error("Unknown type");
    }
}

llvm::Type* LLVMTypeMapper::objectType(Type* type)
{
    if (auto arrayType = dynamic_cast<ArrayType*>(type)) {
        return llvm::StructType::get(mContext,
            { mDataLayout.getIntPtrType(mContext),
                llvm::ArrayType::get(valueType(arrayType->elementType()), 0) });
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
        return getStructOrClassLayoutType(structType);
    }

    if (dynamic_cast<ClassType*>(type) != NULL) {
        llvm::Type* llvmType
            = llvm::StructType::getTypeByName(mContext, mangledName(type));

        if (llvmType == NULL) {
            /*
             * A class instance is represented by a pointer to an array,
             * where each element in return points to the instance data
             * of each class the instance is composed of (that is, the
             * instance's class and all base classes).
             */

            int numClasses = 1;

            ClassType* ctype = (ClassType*)type;

            while (ctype->baseClass() != NULL) {
                numClasses++;
                ctype = (ClassType*)ctype->baseClass();
            }

            llvm::Type* i8 = llvm::Type::getInt8Ty(mContext);
            llvm::Type* i8p = llvm::PointerType::getUnqual(i8);

            llvm::Type* refType = llvm::PointerType::getUnqual(
                llvm::ArrayType::get(i8p, numClasses));

            llvm::StructType* wrapperStruct = llvm::StructType::get(refType);

            wrapperStruct->setName(mangledName(type));
            llvmType = wrapperStruct;
        }

        return llvmType;
    }

    /*
     * Unknown Type
     */
    else {
        throw std::runtime_error("Unknown type");
    }
}

llvm::Type* LLVMTypeMapper::getPointerSizeType()
{
    return mDataLayout.getIntPtrType(mContext);
}

llvm::Type* LLVMTypeMapper::getStructOrClassLayoutType(UserDefinedType* type)
{
    auto name = mangledName(type);
    auto llvmType = llvm::StructType::getTypeByName(mContext, name);

    if (!llvmType) {
        std::vector<llvm::Type*> memberTypes;

        for (DeclarationBlock::declarations_iterator it
            = type->body()->declarations_begin();
            it != type->body()->declarations_end(); ++it) {
            if (auto var = dynamic_cast<Variable*>((*it)->declaredEntity())) {
                memberTypes.push_back(valueType(var->type()));
            }
        }

        llvmType = llvm::StructType::create(memberTypes);
    }

    return llvmType;
}

} // namespace soyac::codegen