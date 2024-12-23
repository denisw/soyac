/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef SOYA_LLVM_TYPE_MAPPER_HPP
#define SOYA_LLVM_TYPE_MAPPER_HPP

#include "ast/Type.hpp"
#include "ast/UserDefinedType.hpp"
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
     * For reference types, an LLVM pointer type is returned.
     *
     * @param type  The Soya type.
     * @return      The matching LLVM type for stac.
     */
    llvm::Type* valueType(soyac::ast::Type* type);

    /**
     * Returns the LLVM type describing the data layout of the given Soya type.
     * For value types, this is equivalent to {@link valueType()}; for
     * reference types, the heap
     *
     * @param type  The Soya type.
     * @return      The matching LLVM type for stac.
     */
    llvm::Type* objectType(ast::Type* type);

private:
    llvm::LLVMContext& mContext;
    const llvm::DataLayout& mDataLayout;

    llvm::Type* getPointerSizeType();

    llvm::Type* getStructOrClassLayoutType(ast::UserDefinedType* type);
};

} // namespace soyac::codegen

#endif
