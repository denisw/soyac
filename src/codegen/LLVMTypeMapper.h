/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef SOYA_LLVM_TYPE_MAPPER_H_
#define SOYA_LLVM_TYPE_MAPPER_H_

#include "ast/Type.h"
#include "ast/UserDefinedType.h"
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Type.h>

#include <llvm/IR/DataLayout.h>

namespace soyac::codegen {

/**
 * Implements the mapping from Soya data types to LLVM equivalents.
 */
class LLVMTypeMapper {
public:
    LLVMTypeMapper(
        llvm::LLVMContext& context, const llvm::DataLayout& dataLayout);

    /**
     * Given a Soya type, returns its value representation as an LLVM type.
     * For reference types, this is a pointer type.
     *
     * @param type  The Soya type.
     * @return      The matching LLVM value type.
     */
    llvm::Type* valueType(soyac::ast::Type* type);

    /**
     * Returns the LLVM type describing the data layout of the given Soya type.
     * For value types, this is equivalent to {@link valueType()}; for
     * reference types, this is the heap representation instead.
     *
     * @param type  The Soya type.
     * @return      The matching LLVM object type.
     */
    llvm::Type* objectType(ast::Type* type);

private:
    llvm::LLVMContext& mContext;
    const llvm::DataLayout& mDataLayout;

    llvm::Type* getPointerSizeType() const;

    llvm::Type* getStructOrClassInstanceDataType(ast::UserDefinedType* type);
};

} // namespace soyac::codegen

#endif
