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

#include <llvm/DerivedTypes.h>
#include <llvm/Instructions.h>
#include <llvm/IntrinsicInst.h>
#include <llvm/ValueSymbolTable.h>

#include <ast/ast.hpp>
#include "CodeGenerator.hpp"

namespace soyac {
namespace codegen
{


  CodeGenerator::CodeGenerator()
    : mBuilder(),
      mEnclosing(NULL),
      mLValue(false)
{
}


llvm::Module*
CodeGenerator::generateCode(Module* m)
{
    return (llvm::Module*) visitModule(m);
}


///// Helper Methods ///////////////////////////////////////////////////////////


void*
CodeGenerator::visitLLValueExpression(LLValueExpression* expr)
{
    return expr->value();
}


std::string
CodeGenerator::mangledName(NamedEntity* entity)
{
    /*
     * If the entity has no qualified name, it is a local entity; in this
     * case, we simply use the unqualified name as mangled name.
     */
    if (entity->qualifiedName().isSimple())
        return mangledSimpleName(entity->name().str());

    /*
     * Otherwise, we have to build a more complex mangled name. It will
     * have the following components:
     *
     * (1) A leading underscore ("_").
     *
     * (2) For each identifier of the module name (the first name component),
     *     a number that specifies the length of the identifier, plus the
     *     identifier itself. For instance, the module name "foo::bar::blubb"
     *     would become "3foo3bar5blubb". This part of the mangled name does
     *     not appear in the mangled names of the program module and the
     *     entities declared in it.
     *
     * The following components only applies to mangled names of declared
     * entities (not those of modules):
     *
     * (3) Another underscore.
     *
     * (4) For each following identifier of the name, the same as described
     *     in (2).
     *
     * The following component only applies to mangled names of functions:
     *
     * (5) The mangled name of every parameter type, in the order the
     *     parameters are declared.
     *
     * There are a few special cases:
     *
     * - The mangled name of the "bool" type is "_b".
     * - The mangled name of the "char" type is "_c".
     * - The mangled name of the "double" type is "_d".
     * - The mangled name of the "float" type is "_f".
     * - The mangled name of the "int" type is "_i".
     * - The mangled name of the "long" type is "_l".
     * - The mangled name of the "uint" type is "_u".
     * - The mangled name of the "ulong" type is "_ul".
     */
    else
    {
        Name name = entity->qualifiedName();
        std::stringstream result;

        result << '_';

        Name::identifiers_iterator it = name.identifiers_begin();

        /*
         * Mangle the module identifier.
         */
        if (Module::getProgram() == NULL ||
            *it != Module::getProgram()->name().str())
        {
            char* modname = new char[(*it).length() + 1];
            strcpy(modname, (*it).c_str());

            const char* mID = std::strtok(modname, ":");

            do
            {
                std::string sname = mangledSimpleName(mID);
                result << sname.length();
                result << sname;
            }
            while ((mID = std::strtok(0, ":")) != 0);

            delete modname;
        }

        it++;

        /*
         * Mangle the remaining identifiers if the entity is not a module.
         */
        if (dynamic_cast<Module*>(entity) == NULL)
        {
            result << '_';

            for (; it != name.identifiers_end(); it++)
            {
                std::string sname = mangledSimpleName(*it);
                result << sname.length();
                result << sname;
            }
        }

        /*
         * If the entity is a function, mangle its parameters.
         */
        if (dynamic_cast<Function*>(entity) != NULL)
        {
            Function* func = (Function*) entity;

            for (Function::parameters_iterator it = func->parameters_begin();
                 it != func->parameters_end(); it++)
            {
                Type* paramType = (*it)->type();

                if (paramType == TYPE_BOOL)
                    result << "_b";
                else if (paramType == TYPE_CHAR)
                    result << "_c";
                else if (paramType == TYPE_FLOAT)
                    result << "_f";
                else if (paramType == TYPE_DOUBLE)
                        result << "_d";
                else if (paramType == TYPE_LONG)
                        result << "_l";
                else if (paramType == TYPE_ULONG)
                        result << "_ul";
                else if (dynamic_cast<IntegerType*>(paramType) != NULL)
                {
                    result <<
                      (((IntegerType*) paramType)->isSigned() ?
                       "_i" : "_u");

                    if (paramType != TYPE_INT &&
                        paramType != TYPE_UINT)
                    {
                        result <<
                          ((IntegerType*) paramType)->size();
                    }
                }

                else
                    result << mangledName(paramType);
            }
        }

        return result.str();
    }
}


std::string
CodeGenerator::mangledSimpleName(const std::string& name)
{
    if (name == CONSTRUCTOR_NAME)
        return "constructor";
    else
        return name;
}


const llvm::Type*
CodeGenerator::lltype(Type* type)
{
    /*
     * 'bool'
     */
    if (type == TYPE_BOOL)
        return llvm::Type::Int1Ty;

    /*
     * 'char'
     */
    else if (type == TYPE_CHAR)
        return llvm::Type::Int32Ty;

    /*
     * 'float'
     */
    else if (type == TYPE_FLOAT)
        return llvm::Type::FloatTy;

    /*
     * 'double'
     */
    else if (type == TYPE_DOUBLE)
        return llvm::Type::DoubleTy;

    /*
     * 'void'
     */
    else if (type == TYPE_VOID)
        return llvm::Type::VoidTy;

    /*
     * Integer Types
     */
    else if (dynamic_cast<IntegerType*>(type) != NULL)
        return llvm::IntegerType::get(((IntegerType*) type)->size());

    /*
     * Enum Types
     */
    else if (dynamic_cast<EnumType*>(type) != NULL)
        return lltype(((EnumType*) type)->underlyingType());

    /*
     * Array Types
     */
    else if (dynamic_cast<ArrayType*>(type) != NULL)
    {
        std::vector<const llvm::Type*> memberTypes;

        memberTypes.push_back(sizeType());
        memberTypes.push_back(
          llvm::ArrayType::get(
            lltype(((ArrayType*) type)->elementType()), 0));

        const llvm::Type* arrayStruct =
          llvm::StructType::get(memberTypes, false);

        return llvm::PointerType::getUnqual(arrayStruct);
    }

    /*
     * Function Types
     */
    else if (dynamic_cast<FunctionType*>(type) != NULL)
    {
        FunctionType* ftype = (FunctionType*) type;
        std::vector<const llvm::Type*> params;

        for (FunctionType::parameterTypes_iterator it =
               ftype->parameterTypes_begin();
             it != ftype->parameterTypes_end(); it++)
        {
            params.push_back(lltype(*it));
        }

        llvm::FunctionType* llftype =
          llvm::FunctionType::get(lltype(ftype->returnType()),
                                  params,
                                  false);

        std::vector<const llvm::Type*> members;
        members.push_back(llvm::PointerType::getUnqual(llftype));
        members.push_back(llvm::PointerType::getUnqual(llvm::Type::Int8Ty));

        return llvm::StructType::get(members, false);
    }

    /*
     * Struct Types
     */
    else if (dynamic_cast<StructType*>(type) != NULL)
        return llstructtype((StructType*) type);

    /*
     * Class Types
     */
    else if (dynamic_cast<ClassType*>(type) != NULL)
    {
        const llvm::Type* llvmType =
          mModule->getTypeByName(mangledName(type));

        if (llvmType == NULL)
        {
            /*
             * A class instance is represented by a pointer to an array,
             * where each element in return points to the instance data
             * of each class the instance is composed of (that is, the
             * instance's class and all base classes).
             */

            int numClasses = 1;

            ClassType* ctype = (ClassType*) type;

            while (ctype->baseClass() != NULL)
            {
                numClasses++;
                ctype = (ClassType*) ctype->baseClass();
            }

            const llvm::Type* i8 = llvm::Type::Int8Ty;
            const llvm::Type* i8p = llvm::PointerType::getUnqual(i8);

            const llvm::Type* refType =
              llvm::PointerType::getUnqual(
                llvm::ArrayType::get(i8p, numClasses));

            const llvm::StructType* wrapperStruct =
              llvm::StructType::get(refType);

            mModule->addTypeName(mangledName(type), wrapperStruct);
            llvmType = wrapperStruct;
        }

        return llvmType;
    }

    /*
     * Other Types
     */
    else
        return mModule->getTypeByName(mangledName(type));
}


const llvm::Type*
CodeGenerator::llstructtype(UserDefinedType* type)
{
    std::string name;

    if (dynamic_cast<ClassType*>(type) != NULL)
        name = mangledName(type) + "_real";
    else
        name = mangledName(type);

    const llvm::Type* llvmType = mModule->getTypeByName(name);

    if (llvmType == NULL)
    {
        UserDefinedType* utype = (UserDefinedType*) type;
        std::vector<const llvm::Type*> memberTypes;

        for (DeclarationBlock::declarations_iterator it =
               utype->body()->declarations_begin();
             it != utype->body()->declarations_end(); it++)
        {
            Variable* var =
              dynamic_cast<Variable*>((*it)->declaredEntity());

            if (var != NULL)
                memberTypes.push_back(lltype(var->type()));
        }

        llvmType = llvm::StructType::get(memberTypes);
    }

    return llvmType;
}


const llvm::Type*
CodeGenerator::sizeType() const
{
    // FIXME: 64-bit awareness!
    return llvm::Type::Int32Ty;
}


llvm::Constant*
CodeGenerator::defaultValue(Type* type)
{
    /*
     * 'bool'
     */
    if (type == TYPE_BOOL)
        return llvm::ConstantInt::getFalse();

    /*
     * 'char'
     */
    else if (type == TYPE_CHAR)
        return defaultValue(TYPE_INT);

    /*
     * Integer Types
     */
    else if (dynamic_cast<IntegerType*>(type) != NULL)
        return llvm::ConstantInt::get(lltype(type), 0, false);

    /*
     * Floating-Point Types
     */
    else if (dynamic_cast<FloatingPointType*>(type) != NULL)
        return llvm::ConstantFP::get(lltype(type), 0);

    /*
     * Enum Types
     */
    else if (dynamic_cast<EnumType*>(type) != NULL)
        return llvm::ConstantInt::get(
          lltype(((EnumType*) type)->underlyingType()), 0, false);

    /*
     * Array  & Class Types
     */
    else if (dynamic_cast<ArrayType*>(type) != NULL ||
             dynamic_cast<ClassType*>(type) != NULL)
    {
        return llvm::ConstantPointerNull::get(
          (const llvm::PointerType*) lltype(type));
    }

    /*
     * Struct & Function Types
     */
    else if (dynamic_cast<StructType*>(type) != NULL ||
             dynamic_cast<FunctionType*>(type) != NULL)
    {
        return llvm::ConstantAggregateZero::get(lltype(type));
    }

    /*
     * (Should never be reached.)
     */
    else
        assert (false);
}


llvm::Function*
CodeGenerator::llfunction(Function* func)
{
    llvm::Function* llfunc =
      mModule->getFunction(mangledName(func));

    if (llfunc == NULL)
    {
        std::vector<const llvm::Type*> params;
        Type* enclosingType;

        /*
         * If the function is a property accessor, it's enclosing type
         * is not equal to it's directly enclosing declaration, as that
         * is the property to which the accessor belongs. We need to
         * get that property's parent instead.
         */
        if (dynamic_cast<Property*>(func->parent()) != NULL)
            enclosingType = dynamic_cast<Type*>(func->parent()->parent());
        else
            enclosingType = dynamic_cast<Type*>(func->parent());

        /*
         * If the function is an instance function or an accessor of an
         * instance property, it needs to accept a "this" reference as
         * first parameter.
         */
        if (enclosingType != NULL)
        {
            const llvm::Type* thisType;

            if (dynamic_cast<StructType*>(enclosingType) != NULL)
            {
                thisType =
                  llvm::PointerType::getUnqual(lltype(enclosingType));
            }
            else
            {
                thisType = lltype(enclosingType);
            }

            params.push_back(thisType);
        }

        for (Function::parameters_iterator it = func->parameters_begin();
             it != func->parameters_end(); it++)
        {
            params.push_back(lltype((*it)->type()));
        }

        const llvm::FunctionType* llftype =
          llvm::FunctionType::get(lltype(func->returnType()),
                                  params,
                                  false);

        llfunc =
          llvm::Function::Create(llftype,
                                 llvm::Function::ExternalLinkage,
                                 mangledName(func),
                                 mModule);

        /*
         * Because we cannot set the parameter names with
         * llvm::Function::Create(), we need to do this
         * afterwards.
         */

        Function::parameters_iterator p = func->parameters_begin();
        llvm::Function::arg_iterator a = llfunc->arg_begin();

        if (enclosingType != NULL)
        {
            a->setName("this");
            a++;
        }

        for (; a != llfunc->arg_end(); p++, a++)
        {
            a->setName(mangledName(*p));
        }
    }

    return llfunc;
}


llvm::Function*
CodeGenerator::llinitializer(UserDefinedType* type)
{
    llvm::Function* llfunc =
      mModule->getFunction(mangledName(type) + "_init");

    if (llfunc == NULL)
    {
        std::vector<const llvm::Type*> params;
        const llvm::Type* thisType;

        if (dynamic_cast<StructType*>(type) != NULL)
        {
            thisType =
              llvm::PointerType::getUnqual(lltype(type));
        }
        else
            thisType = lltype(type);

        params.push_back(thisType);

        llvm::FunctionType* llftype =
          llvm::FunctionType::get(llvm::Type::VoidTy,
                                  params,
                                  false);

        llfunc =
          llvm::Function::Create(llftype,
                                 llvm::Function::PrivateLinkage,
                                 mangledName(type) + "_init",
                                 mModule);


        (*llfunc->arg_begin()).setName("this");
    }

    return llfunc;
}


llvm::Function*
CodeGenerator::llallocator(ClassType* type)
{
    llvm::Function* llfunc =
      mModule->getFunction(mangledName(type) + "_new");

    if (llfunc == NULL)
    {
        std::vector<const llvm::Type*> params;

        llvm::FunctionType* llftype =
          llvm::FunctionType::get(lltype(type),
                                  params,
                                  false);

        llfunc =
          llvm::Function::Create(llftype,
                                 llvm::Function::ExternalLinkage,
                                 mangledName(type) + "_new",
                                 mModule);
    }

    return llfunc;
}


llvm::Value*
CodeGenerator::classPrivate(llvm::Value* instance,
                            ClassType* type,
                            bool lvalue)
{
    unsigned int classIndex = 0;
    ClassType* cls = type;

    while (cls->baseClass() != NULL)
    {
        classIndex++;
        cls = (ClassType*) cls->baseClass();
    }

    std::vector<llvm::Value*> indices;
    indices.push_back(llvm::ConstantInt::get(sizeType(), 0, false));
    indices.push_back(llvm::ConstantInt::get(sizeType(), classIndex, false));

    llvm::Value* ret =
      mBuilder.CreateGEP(instance, indices.begin(), indices.end());

    ret =
      mBuilder.CreateBitCast(
        ret,
        llvm::PointerType::getUnqual(
          llvm::PointerType::getUnqual(llstructtype(type))));

    if (lvalue)
        return ret;
    else
        return mBuilder.CreateLoad(ret);
}


void
CodeGenerator::createInitializer(UserDefinedType* type)
{
    llvm::Function* tmpFunction = mFunction;
    mFunction = llinitializer(type);

    llvm::BasicBlock* tmpBlock = mBuilder.GetInsertBlock();
    mBuilder.SetInsertPoint(llvm::BasicBlock::Create("", mFunction));

    if (dynamic_cast<ClassType*>(type) != NULL)
    {
        ClassType* ctype = (ClassType*) type;
        llvm::Value* data = createGCMalloc(llstructtype(ctype));

        llvm::Value* thisVal =
          (llvm::Value*) ThisExpression(ctype).visit(this);

        llvm::Value* priv = classPrivate(thisVal, ctype, true);
        mBuilder.CreateStore(data, priv);
    }

    /*
     * Generate code for initializing all instance variables.
     */

    for (DeclarationBlock::declarations_iterator it =
           type->body()->declarations_begin();
         it != type->body()->declarations_end(); it++)
    {
        if (dynamic_cast<Variable*>((*it)->declaredEntity()) != NULL)
        {
            Variable* var = (Variable*) (*it)->declaredEntity();

            Expression* lh =
              new InstanceVariableExpression(new ThisExpression(type), var);
            Expression* rh;

            if (var->initializer() != NULL)
                rh = var->initializer();
            else
                rh = new LLValueExpression(var->type(), defaultValue(var->type()));

            AssignmentExpression assign(lh, rh);
            assign.visit(this);
        }
    }

    mBuilder.CreateRetVoid();
    mBuilder.SetInsertPoint(tmpBlock);

    mFunction = tmpFunction;
}


llvm::Value*
CodeGenerator::createBuiltInMethodCall(Expression* operand,
                                       const Name& methodName,
                                       Expression* argument,
                                       Expression* argument2)
{
    llvm::Value* lh = (llvm::Value*) operand->visit(this);
    llvm::Value* rh;
    llvm::Value* rh2;

    if (argument != NULL)
        rh = (llvm::Value*) argument->visit(this);

    if (argument2 != NULL)
        rh2 = (llvm::Value*) argument2->visit(this);

    /*
     * Integer Types
     */
    if (dynamic_cast<IntegerType*>(operand->type()) != NULL)
    {
        /*
         * If the integer operand's type is not equal to the argument's type,
         * the operand must be cast first.
         */
        if (lh->getType() != rh->getType())
        {
            if (((IntegerType*) operand->type())->isSigned())
                lh = mBuilder.CreateSExt(lh, rh->getType());
            else
                lh = mBuilder.CreateZExt(lh, rh->getType());
        }

        /*
         * plus()
         */
        if (methodName == "plus")
            return mBuilder.CreateAdd(lh, rh);

        /*
         * minus()
         */
        else if (methodName == "minus")
            return mBuilder.CreateSub(lh, rh);

        /*
         * mul()
         */
        else if (methodName == "mul")
            return mBuilder.CreateMul(lh, rh);

        /*
         * div()
         */
        else if (methodName == "div")
        {
            if (((IntegerType*) operand->type())->isSigned())
                return mBuilder.CreateSDiv(lh, rh);
            else
                return mBuilder.CreateUDiv(lh, rh);
        }

        /*
         * mod()
         */
        else if (methodName == "mod")
        {
            if (((IntegerType*) operand->type())->isSigned())
                return mBuilder.CreateSRem(lh, rh);
            else
                return mBuilder.CreateURem(lh, rh);
        }

        /*
         * lshift()
         */
        else if (methodName == "lshift")
            return mBuilder.CreateShl(lh, rh);

        /*
         * rshift()
         */
        else if (methodName == "rshift")
            return mBuilder.CreateAShr(lh, rh);

        /*
         * equals()
         */
        else if (methodName == "equals")
            return mBuilder.CreateICmpEQ(lh, rh);

        /*
         * lessThan()
         */
        else if (methodName == "lessThan")
        {
            if (((IntegerType*) operand->type())->isSigned())
                return mBuilder.CreateICmpSLT(lh, rh);
            else
                return mBuilder.CreateICmpULT(lh, rh);
        }

        /*
         * greaterThan()
         */
        else if (methodName == "greaterThan")
        {
            if (((IntegerType*) operand->type())->isSigned())
                return mBuilder.CreateICmpSGT(lh, rh);
            else
                return mBuilder.CreateICmpUGT(lh, rh);
        }

        /*
         * ("!=" operator)
         */
        else if (methodName == "!=")
            return mBuilder.CreateICmpNE(lh, rh);

        /*
         * ("<=" operator)
         */
        else if (methodName == "<=")
        {
            if (((IntegerType*) operand->type())->isSigned())
                return mBuilder.CreateICmpSLE(lh, rh);
            else
                return mBuilder.CreateICmpULE(lh, rh);
        }

        /*
         * (">=" operator)
         */
        else if (methodName == ">=")
        {
            if (((IntegerType*) operand->type())->isSigned())
                return mBuilder.CreateICmpSGE(lh, rh);
            else
                return mBuilder.CreateICmpUGE(lh, rh);
        }
    }

    /*
     * "bool" Type
     */
    else if (operand->type() == TYPE_BOOL)
    {
        /*
         * equals()
         */
        if (methodName == "equals")
            return mBuilder.CreateICmpEQ(lh, rh);

        /*
         * ("!=" operator)
         */
        if (methodName == "!=")
            return mBuilder.CreateICmpNE(lh, rh);
    }

    /*
     * "char" Type
     */
    else if (operand->type() == TYPE_CHAR)
    {
        /*
         * equals()
         */
        if (methodName == "equals")
            return mBuilder.CreateICmpEQ(lh, rh);

        /*
         * ("!=" operator)
         */
        if (methodName == "!=")
            return mBuilder.CreateICmpNE(lh, rh);
    }

    /*
     * Floating-Point Types
     */
    else if (dynamic_cast<FloatingPointType*>(operand->type()) != NULL)
    {
        /*
         * If the integer operand's type is not equal to the argument's type,
         * the operand must be cast first.
         */
        if (lh->getType() != rh->getType())
            lh = mBuilder.CreateFPExt(lh, rh->getType());

        /*
         * plus()
         */
        if (methodName == "plus")
            return mBuilder.CreateAdd(lh, rh);

        /*
         * minus()
         */
        else if (methodName == "minus")
            return mBuilder.CreateSub(lh, rh);

        /*
         * mul()
         */
        else if (methodName == "mul")
            return mBuilder.CreateMul(lh, rh);

        /*
         * div()
         */
        else if (methodName == "div")
            return mBuilder.CreateFDiv(lh, rh);

        /*
         * equals()
         */
        else if (methodName == "equals")
            return mBuilder.CreateFCmpOEQ(lh, rh);

        /*
         * lessThan()
         */
        else if (methodName == "lessThan")
            return mBuilder.CreateFCmpOLT(lh, rh);

        /*
         * greaterThan()
         */
        else if (methodName == "greaterThan")
            return mBuilder.CreateFCmpOGT(lh, rh);

        /*
         * ("!=" operator)
         */
        else if (methodName == "!=")
            return mBuilder.CreateFCmpONE(lh, rh);

        /*
         * ("<=" operator)
         */
        else if (methodName == "<=")
            return mBuilder.CreateFCmpOLE(lh, rh);

        /*
         * (">=" operator)
         */
        else if (methodName == ">=")
            return mBuilder.CreateFCmpOGE(lh, rh);
    }

    /*
     * Array Types
     */
    else if (dynamic_cast<ArrayType*>(operand->type()) != NULL)
    {
        if (methodName == "getElement")
        {
            std::vector<llvm::Value*> indices;
            indices.push_back(llvm::ConstantInt::get(sizeType(), 0, false));
            indices.push_back(llvm::ConstantInt::get(sizeType(), 1, false));
            indices.push_back(rh);

            llvm::Value* address =
              mBuilder.CreateGEP(lh, indices.begin(), indices.end());

            return mBuilder.CreateLoad(address);
        }
        else if (methodName == "setElement")
        {
            std::vector<llvm::Value*> indices;
            indices.push_back(llvm::ConstantInt::get(sizeType(), 0, false));
            indices.push_back(llvm::ConstantInt::get(sizeType(), 1, false));
            indices.push_back(rh);

            llvm::Value* address =
              mBuilder.CreateGEP(lh, indices.begin(), indices.end());

            Type* elemType = ((ArrayType*) operand->type())->elementType();

            LLValueExpression lhExpr(elemType, address);
            LLValueExpression rhExpr(elemType, rh2);
            lhExpr.ref();
            rhExpr.ref();

            AssignmentExpression assign(&lhExpr, &rhExpr);
            return (llvm::Value*) assign.visit(this);
        }
    }

    /*
     * Enum Types
     */
    else if (dynamic_cast<EnumType*>(operand->type()) != NULL)
    {
        /*
         * The only method that enums have is equals(), so we don't have to
         * check for the method name. (We check with an assertion to be sure,
         * though).
         */
        assert (methodName == "equals");
        return mBuilder.CreateICmpEQ(lh, rh);
    }

    assert (false);
}


llvm::Value*
CodeGenerator::createBuiltInPropertyCall(Expression* operand,
                                         const Name& propertyName)
{
    llvm::Value* op = (llvm::Value*) operand->visit(this);

    /*
     * Array Types
     */
    if (dynamic_cast<ArrayType*>(operand->type()) != NULL)
    {
        if (propertyName == "length")
        {
            llvm::Value* lengthField = mBuilder.CreateStructGEP(op, 0);
            llvm::Value* length = mBuilder.CreateLoad(lengthField);
            return mBuilder.CreateZExt(length, llvm::IntegerType::Int64Ty);
        }
    }

    assert (false);
}


llvm::Value*
CodeGenerator::createSizeof(const llvm::Type* type)
{
    const llvm::PointerType* ptrType = llvm::PointerType::getUnqual(type);

    llvm::Value* tmp =
      mBuilder.CreateGEP(llvm::ConstantPointerNull::get(ptrType),
                         llvm::ConstantInt::get(llvm::IntegerType::Int32Ty,
                                                1, false));

    // FIXME: No 64-bit awareness!
    return mBuilder.CreatePtrToInt(tmp, llvm::IntegerType::Int32Ty);
}


llvm::Value*
CodeGenerator::createGCMalloc(llvm::Value* size)
{
    const llvm::Type* i8p =
      llvm::PointerType::getUnqual(llvm::IntegerType::Int8Ty);

    std::vector<llvm::Value*> args;
    args.push_back(size);

    llvm::Function* gcMalloc = mModule->getFunction("GC_malloc");

    if (gcMalloc == NULL)
    {
        std::vector<const llvm::Type*> params;
        params.push_back(sizeType());


        llvm::FunctionType* gcMallocType =
          llvm::FunctionType::get(i8p, params, false);

        gcMalloc = llvm::Function::Create(gcMallocType,
                                          llvm::Function::ExternalLinkage,
                                          "GC_malloc",
                                          mModule);
    }

    return mBuilder.CreateCall(gcMalloc, args.begin(), args.end());
}


llvm::Value*
CodeGenerator::createGCMalloc(const llvm::Type* type, llvm::Value* n)
{
    llvm::Value* size = createSizeof(type);

    if (n != NULL)
        size = mBuilder.CreateMul(size, n);

    return mBuilder.CreateBitCast(
      createGCMalloc(size),
      llvm::PointerType::getUnqual(type));
}


///// Module ///////////////////////////////////////////////////////////////////


void*
CodeGenerator::visitModule(soyac::ast::Module* m)
{
    mModule = new llvm::Module(m->name().str());

    std::string initFuncName;
    std::vector<const llvm::Type*> params;
    llvm::FunctionType* initFuncType;

    if (m == Module::getProgram())
    {
        initFuncName = "main";
        initFuncType = llvm::FunctionType::get(llvm::Type::Int32Ty,
                                               params,
                                               false);
    }
    else
    {
        initFuncName = mangledName(m) + "_init";
        initFuncType = llvm::FunctionType::get(llvm::Type::VoidTy,
                                               params,
                                               false);
    }

    mFunction = mInitFunction =
      llvm::Function::Create(initFuncType,
                             llvm::Function::ExternalLinkage,
                             initFuncName,
                             mModule);

    llvm::BasicBlock* body = llvm::BasicBlock::Create("", mFunction);
    mBuilder.SetInsertPoint(body);

    m->body()->visit(this);

    mBuilder.SetInsertPoint(&mFunction->back());

    if (m == Module::getProgram())
        mBuilder.CreateRet(llvm::ConstantInt::get(llvm::Type::Int32Ty, 0, true));
    else
        mBuilder.CreateRetVoid();

    return mModule;
}


///// Statements ///////////////////////////////////////////////////////////////


void*
CodeGenerator::visitBlock(Block* b)
{
    for (Block::statements_iterator it = b->statements_begin();
         it != b->statements_end(); it++)
    {
        (*it)->visit(this);
    }

    return NULL;
}


void*
CodeGenerator::visitDeclarationStatement(DeclarationStatement* stmt)
{
    stmt->declaredEntity()->visit(this);
    return NULL;
}


void*
CodeGenerator::visitDoStatement(DoStatement* stmt)
{
    llvm::BasicBlock* start;
    llvm::BasicBlock* end;

    start = llvm::BasicBlock::Create("", mFunction);
    mBuilder.CreateBr(start);

    mBuilder.SetInsertPoint(start);
    stmt->body()->visit(this);

    llvm::Value* cond = (llvm::Value*) stmt->condition()->visit(this);

    end = llvm::BasicBlock::Create("", mFunction);
    mBuilder.CreateCondBr(cond, start, end);
    mBuilder.SetInsertPoint(end);

    return NULL;
}


void*
CodeGenerator::visitExpressionStatement(ExpressionStatement* stmt)
{
    stmt->expression()->visit(this);
    return NULL;
}


void*
CodeGenerator::visitForStatement(ForStatement* stmt)
{
    llvm::BasicBlock* start;
    llvm::BasicBlock* loop;
    llvm::BasicBlock* end;

    for (ForStatement::initializers_iterator it = stmt->initializers_begin();
         it != stmt->initializers_end(); it++)
    {
        (*it)->visit(this);
    }

    start = llvm::BasicBlock::Create("", mFunction);
    mBuilder.CreateBr(start);

    mBuilder.SetInsertPoint(start);
    llvm::Value* cond = (llvm::Value*) stmt->condition()->visit(this);

    loop = llvm::BasicBlock::Create("", mFunction);
    mBuilder.SetInsertPoint(loop);

    stmt->body()->visit(this);

    for (ForStatement::iterators_iterator it = stmt->iterators_begin();
         it != stmt->iterators_end(); it++)
    {
        (*it)->visit(this);
    }

    mBuilder.CreateBr(start);

    end = llvm::BasicBlock::Create("", mFunction);

    mBuilder.SetInsertPoint(start);
    mBuilder.CreateCondBr(cond, loop, end);
    mBuilder.SetInsertPoint(end);

    return NULL;
}


void*
CodeGenerator::visitIfStatement(IfStatement* stmt)
{
    llvm::Value* cond = (llvm::Value*) stmt->condition()->visit(this);

    llvm::BasicBlock* ifTrue = llvm::BasicBlock::Create("", mFunction);
    llvm::BasicBlock* ifFalse = llvm::BasicBlock::Create("", mFunction);

    mBuilder.CreateCondBr(cond, ifTrue, ifFalse);

    mBuilder.SetInsertPoint(ifTrue);
    stmt->body()->visit(this);

    if (stmt->elseBody() != NULL)
    {
        llvm::BasicBlock* ifEnd = llvm::BasicBlock::Create("", mFunction);
        mBuilder.CreateBr(ifEnd);

        mBuilder.SetInsertPoint(ifFalse);
        stmt->elseBody()->visit(this);

        mBuilder.CreateBr(ifEnd);
        mBuilder.SetInsertPoint(ifEnd);

    }
    else
    {
        mBuilder.CreateBr(ifFalse);
        mBuilder.SetInsertPoint(ifFalse);
    }

    return NULL;
}


void*
CodeGenerator::visitReturnStatement(ReturnStatement* stmt)
{
    if (stmt->returnValue() != NULL)
    {
        llvm::Value* retval = (llvm::Value*) stmt->returnValue()->visit(this);
        mBuilder.CreateRet(retval);
    }
    else
        mBuilder.CreateRetVoid();

    return NULL;
}


void*
CodeGenerator::visitWhileStatement(WhileStatement* stmt)
{
    llvm::BasicBlock* start;
    llvm::BasicBlock* loop;
    llvm::BasicBlock* end;

    start = llvm::BasicBlock::Create("", mFunction);
    mBuilder.CreateBr(start);

    mBuilder.SetInsertPoint(start);
    llvm::Value* cond = (llvm::Value*) stmt->condition()->visit(this);

    loop = llvm::BasicBlock::Create("", mFunction);
    mBuilder.SetInsertPoint(loop);
    stmt->body()->visit(this);
    mBuilder.CreateBr(start);

    end = llvm::BasicBlock::Create("", mFunction);

    mBuilder.SetInsertPoint(start);
    mBuilder.CreateCondBr(cond, loop, end);
    mBuilder.SetInsertPoint(end);

    return NULL;
}


///// Declared Entities ////////////////////////////////////////////////////////


void*
CodeGenerator::visitClassType(ClassType* type)
{
    DeclaredEntity* tmpEnclosing = mEnclosing;
    mEnclosing = type;

    createInitializer(type);

    /*
     * Generate code for the allocation function (see llallocator()).
     */

    llvm::Function* tmpFunction = mFunction;
    mFunction = llallocator(type);

    llvm::BasicBlock* body = llvm::BasicBlock::Create("", mFunction);
    llvm::BasicBlock* tmpBlock = mBuilder.GetInsertBlock();
    mBuilder.SetInsertPoint(body);

    llvm::Value* ret = createGCMalloc(llstructtype(type));
    ret = mBuilder.CreateBitCast(ret, lltype(type));
    mBuilder.CreateRet(ret);

    mBuilder.SetInsertPoint(tmpBlock);
    mFunction = tmpFunction;

    /*
     * Generate code for the class' members.
     */

    for (DeclarationBlock::declarations_iterator it =
           type->body()->declarations_begin();
         it != type->body()->declarations_end(); it++)
    {
        /*
         * Don't visit member variables, their initialization code has
         * already been taken care of in createInitializer().
         */
        if (dynamic_cast<Variable*>((*it)->declaredEntity()) == NULL)
            (*it)->visit(this);
    }

    mEnclosing = tmpEnclosing;
    return NULL;
}


void*
CodeGenerator::visitConstructor(Constructor* cons)
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

    if (cons->initializer() != NULL)
    {
        ConstructorInitializer* init = cons->initializer();

        Expression* instance = new ThisExpression((Type*) cons->parent());
        Expression* callee = new InstanceFunctionExpression(
          instance,
          init->constructor());

        CallExpression call(
          callee,
          init->arguments_begin(),
          init->arguments_end());

        call.visit(this);

        /*
         * If the initializer hasn't called another constructor of the same
         * class or struct, the code generated below has already been executed
         * in that constructor, so it doesn't have to be generated for
         * this one.
         */
        if (init->constructor()->parent() == cons->parent())
        {
            mBuilder.SetInsertPoint(tmpBlock);
            mFunction = tmpFunction;
            return NULL;
        }
    }

    /*
     * Generate code to call the initialization function of the enclosing
     * struct or class. (See llinitializer().)
     */

    std::vector<llvm::Value*> args;
    ThisExpression texpr((Type*) cons->parent());
    args.push_back((llvm::Value*) texpr.visit(this));

    mBuilder.CreateCall(
      llinitializer((UserDefinedType*) cons->parent()),
      args.begin(), args.end());

    mBuilder.SetInsertPoint(tmpBlock);
    mFunction = tmpFunction;

    return NULL;
}

void*
CodeGenerator::visitConstructorInitializer(ConstructorInitializer* init)
{
    return NULL;
}


void*
CodeGenerator::visitFunction(Function* func)
{
    llvm::Function* tmpFunction = mFunction;
    mFunction = llfunction(func);

    if (func->body() != NULL)
    {
        llvm::BasicBlock* body = llvm::BasicBlock::Create("", mFunction);

        llvm::BasicBlock* tmp = mBuilder.GetInsertBlock();
        mBuilder.SetInsertPoint(body);

        func->body()->visit(this);

        llvm::BasicBlock* last = mBuilder.GetInsertBlock();

        if (last->empty() || !last->back().isTerminator())
        {
            if (func->returnType() == TYPE_VOID)
                mBuilder.CreateRetVoid();
            else
                mBuilder.CreateUnreachable();
        }

        mBuilder.SetInsertPoint(tmp);
    }

    mFunction = tmpFunction;
    return NULL;
}


void*
CodeGenerator::visitProperty(Property* prop)
{
    if (prop->getAccessor() != NULL)
        prop->getAccessor()->visit(this);

    if (prop->setAccessor() != NULL)
        prop->setAccessor()->visit(this);

    return NULL;
}


void*
CodeGenerator::visitPropertyGetAccessor(PropertyGetAccessor* acc)
{
    return visitFunction(acc);
}


void*
CodeGenerator::visitPropertySetAccessor(PropertySetAccessor* acc)
{
    return visitFunction(acc);
}


void*
CodeGenerator::visitStructType(StructType* type)
{
    DeclaredEntity* tmpEnclosing = mEnclosing;
    mEnclosing = type;

    createInitializer(type);

    for (DeclarationBlock::declarations_iterator it =
           type->body()->declarations_begin();
         it != type->body()->declarations_end(); it++)
    {
        /*
         * Don't visit member variables, their initialization code has
         * already been taken care of in createInitializer().
         */
        if (dynamic_cast<Variable*>((*it)->declaredEntity()) == NULL)
            (*it)->visit(this);
    }

    mEnclosing = tmpEnclosing;
    return NULL;
}


void*
CodeGenerator::visitVariable(Variable* var)
{
    llvm::Value* v;

    if (mFunction == mInitFunction)
    {
        v = new llvm::GlobalVariable(lltype(var->type()),
                                     false,
                                     llvm::GlobalVariable::ExternalLinkage,
                                     defaultValue(var->type()),
                                     mangledName(var),
                                     mModule);
    }
    else
    {
        v = mBuilder.CreateAlloca(lltype(var->type()),
                                  NULL,
                                  mangledName(var).c_str());

        if (var->initializer() == NULL)
        {
            Expression* rh =
              new LLValueExpression(var->type(), defaultValue(var->type()));

            AssignmentExpression assign(new VariableExpression(var), rh);
            assign.visit(this);
        }
    }

    if (var->initializer() != NULL)
    {
        AssignmentExpression assign(new VariableExpression(var),
                                    var->initializer());
        assign.visit(this);
    }

    return NULL;
}


///// Expressions //////////////////////////////////////////////////////////////


void*
CodeGenerator::visitArrayCreationExpression(ArrayCreationExpression* expr)
{
    /*
     * Generate code to evaluate all element expressions (if any).
     */

    std::vector<llvm::Value*> elems;

    for (ArrayCreationExpression::elements_iterator it =
           expr->elements_begin();
         it != expr->elements_end(); it++)
    {
        elems.push_back((llvm::Value*) (*it)->visit(this));
    }

    /*
     * Generate code for the array length evaluation.
     */

    llvm::Value* len;

    if (expr->lengthExpression() != NULL)
        len = (llvm::Value*) expr->lengthExpression()->visit(this);
    else
        len = llvm::ConstantInt::get(sizeType(), elems.size(), false);

    /*
     * Create a call to GC_malloc() to allocate the space needed for
     * the array.
     */

    ArrayType* type = (ArrayType*) expr->type();

    llvm::Value* allocSize =
      mBuilder.CreateMul(createSizeof(lltype(type)), len);
    allocSize =
      mBuilder.CreateAdd(allocSize, createSizeof(sizeType()));

    llvm::Value* array = createGCMalloc(allocSize);
    array = mBuilder.CreateBitCast(array, lltype(type));

    /*
     * Finally, the array length and all element values need to be
     * copied into the array.
     */

    llvm::Value* lengthField = mBuilder.CreateStructGEP(array, 0);
    llvm::Value* dataField = mBuilder.CreateStructGEP(array, 1);

    mBuilder.CreateStore(len, lengthField);

    for (size_t index = 0; index < elems.size(); index++)
    {
        std::vector<llvm::Value*> indices;
        indices.push_back(llvm::ConstantInt::get(sizeType(), 0, false));
        indices.push_back(llvm::ConstantInt::get(sizeType(), index, false));

        llvm::Value* elemLH =
          mBuilder.CreateGEP(dataField, indices.begin(), indices.end());

        Expression* lh = new LLValueExpression(type->elementType(), elemLH);
        Expression* rh = new LLValueExpression(type->elementType(),
                                               elems[index]);

        AssignmentExpression assign(lh, rh);
        assign.visit(this);
    }

    return array;
}


void*
CodeGenerator::visitAssignmentExpression(AssignmentExpression* expr)
{
    mLValue = true;
    llvm::Value* lh = (llvm::Value*) expr->leftHand()->visit(this);
    mLValue = false;

    llvm::Value* rh = (llvm::Value*) expr->rightHand()->visit(this);

    mBuilder.CreateStore(rh, lh);
    return rh;
}


void*
CodeGenerator::visitBooleanLiteral(BooleanLiteral* lit)
{
    if (lit->value() == true)
        return llvm::ConstantInt::getTrue();
    else
        return llvm::ConstantInt::getFalse();
}


void*
CodeGenerator::visitCallExpression(CallExpression* expr)
{
    /*
     * Check if this is a call to a built-in function or property. If it is,
     * create the corresponding code using createBuiltInMethodCall() or
     * createBuiltInPropertyCall(), respectively.
     */
    if (dynamic_cast<InstanceFunctionExpression*>(expr->callee()) != NULL)
    {
        InstanceFunctionExpression* funcExpr =
          (InstanceFunctionExpression*) expr->callee();

        if (dynamic_cast<BuiltInType*>(funcExpr->instance()->type()) != NULL ||
            dynamic_cast<ArrayType*>(funcExpr->instance()->type()) != NULL ||
            dynamic_cast<EnumType*>(funcExpr->instance()->type()) != NULL)
        {
            if (dynamic_cast<PropertyGetAccessor*>(funcExpr->target()) != NULL)
            {
                return createBuiltInPropertyCall(
                  funcExpr->instance(),
                  funcExpr->target()->parent()->name());
            }
            else
            {
                Expression* arg1 = *expr->arguments_begin();
                Expression* arg2 =
                  (funcExpr->target()->name() == "setElement") ?
                  *(++expr->arguments_begin()) : NULL;

                return createBuiltInMethodCall(funcExpr->instance(),
                                               funcExpr->target()->name(),
                                               arg1, arg2);
            }
        }
    }

    /*
     * Otherwise, we create an LLVM 'call' instruction that
     * matches the call expression.
     */

    std::vector<llvm::Value*> args;

    for (CallExpression::arguments_iterator it = expr->arguments_begin();
         it != expr->arguments_end(); it++)
    {
        args.push_back((llvm::Value*) (*it)->visit(this));
    }

    /*
     * If the call is indirect (to a function reference rather than to a
     * concrete function), we need to generate some logic to call the denoted
     * function correctly depending on whether the reference has an associated
     * environment (like a "this" reference) or not.
     */
    if (dynamic_cast<FunctionExpression*>(expr->callee()) == NULL &&
        dynamic_cast<InstanceFunctionExpression*>(expr->callee()) == NULL)
    {
        llvm::Value* funcVal = (llvm::Value*) expr->callee()->visit(this);
        llvm::Value* fptr = mBuilder.CreateExtractValue(funcVal, 0);
        llvm::Value* env = mBuilder.CreateExtractValue(funcVal, 1);

        const llvm::FunctionType* ftype =
          (const llvm::FunctionType*)
            ((llvm::PointerType*) fptr->getType())->getElementType();

        const llvm::Type* rtype =
          ftype->getReturnType();
        const llvm::Type* i8p =
          llvm::PointerType::getUnqual(llvm::IntegerType::Int8Ty);

        std::vector<const llvm::Type*> params(
          ftype->param_begin(),
          ftype->param_end());

        params.insert(params.begin(), i8p);

        llvm::Value* fptrEnv =
          mBuilder.CreateBitCast(
            fptr,
            llvm::PointerType::getUnqual(
              llvm::FunctionType::get(rtype, params, false)));

        llvm::BasicBlock* withEnv = llvm::BasicBlock::Create("", mFunction);
        llvm::BasicBlock* withoutEnv = llvm::BasicBlock::Create("", mFunction);
        llvm::BasicBlock* end = llvm::BasicBlock::Create("", mFunction);

        llvm::Value* envNull = mBuilder.CreateIsNull(env);
        mBuilder.CreateCondBr(envNull, withoutEnv, withEnv);

        mBuilder.SetInsertPoint(withoutEnv);
        llvm::Value* ret1 = mBuilder.CreateCall(fptr, args.begin(), args.end());
        mBuilder.CreateBr(end);

        mBuilder.SetInsertPoint(withEnv);
        args.insert(args.begin(), mBuilder.CreateBitCast(env, i8p));
        llvm::Value* ret2 =
          mBuilder.CreateCall(fptrEnv, args.begin(), args.end());
        mBuilder.CreateBr(end);

        mBuilder.SetInsertPoint(end);

        llvm::PHINode* phi = mBuilder.CreatePHI(rtype);
        phi->addIncoming(ret1, withoutEnv);
        phi->addIncoming(ret2, withEnv);

        return phi;
    }
    /*
     * If the call is direct, a simple LLVM 'call' instruction does the job.
     */
    else
    {
        llvm::Value* callee;

        if (dynamic_cast<FunctionExpression*>(expr->callee()) != NULL)
        {
            FunctionExpression* fexpr = (FunctionExpression*) expr->callee();
            callee = llfunction(fexpr->target());
        }
        else if (dynamic_cast<InstanceFunctionExpression*>(expr->callee()) !=
                   NULL)
        {
            InstanceFunctionExpression* instanceFunc =
              (InstanceFunctionExpression*) expr->callee();

            callee = llfunction(instanceFunc->target());

            llvm::Value* instanceVal;

            if (dynamic_cast<StructType*>(instanceFunc->instance()->type()) !=
                  NULL)
            {
                bool tmp = mLValue;
                mLValue = true;

                instanceVal =
                  (llvm::Value*) instanceFunc->instance()->visit(this);

                mLValue = tmp;

            }
            else
            {
                instanceVal =
                  (llvm::Value*) instanceFunc->instance()->visit(this);

                const llvm::Type* instanceType =
                  ((llvm::Function*) callee)->getFunctionType()->getParamType(0);

                if (instanceVal->getType() != instanceType)
                    instanceVal = mBuilder.CreateBitCast(instanceVal, instanceType);
            }

            args.insert(args.begin(), instanceVal);
        }

        return mBuilder.CreateCall(callee, args.begin(), args.end());
    }
}


void*
CodeGenerator::visitCastExpression(CastExpression* expr)
{
    llvm::Value* op = (llvm::Value*) expr->operand()->visit(this);
    const llvm::Type* ctype = lltype(expr->type());

    /*
     * If the internal representation of the source and destination type
     * are equal, the cast is a no-op.
     */
    if (op->getType() == ctype)
    {
        return op;
    }

    else if (llvm::isa<llvm::IntegerType>(ctype))
    {
        /*
         * Integer => Integer
         */
        if ((llvm::isa<llvm::IntegerType>(op->getType())))
        {
            const llvm::IntegerType* op_itype =
              llvm::cast<llvm::IntegerType>(op->getType());
            const llvm::IntegerType* c_itype =
              llvm::cast<llvm::IntegerType>(ctype);

            if (c_itype->getBitWidth() < op_itype->getBitWidth())
                return mBuilder.CreateTrunc(op, ctype);
            else if (((IntegerType*) expr->operand()->type())->isSigned())
                return mBuilder.CreateSExt(op, ctype);
            else
                return mBuilder.CreateZExt(op, ctype);
        }

        /*
         * Integer => Floating-Point
         */
        else if (op->getType() == llvm::Type::FloatTy ||
                 op->getType() == llvm::Type::DoubleTy)
        {
            if (((IntegerType*) expr->type())->isSigned())
                return mBuilder.CreateFPToSI(op, ctype);
            else
                return mBuilder.CreateFPToUI(op, ctype);

        }
    }

    else if (ctype == llvm::Type::FloatTy ||
             ctype == llvm::Type::DoubleTy)
    {
        /*
         * 'double' => 'float'
         */
        if (op->getType() == llvm::Type::FloatTy)
            return mBuilder.CreateFPExt(op, ctype);

        /*
         * 'float' => 'double'
         */
        else if (op->getType() == llvm::Type::DoubleTy)
            return mBuilder.CreateFPTrunc(op, ctype);

        /*
         * Floating-Point => Integer
         */
        else if (llvm::isa<llvm::IntegerType>(op->getType()))
        {
            if (((IntegerType*) expr->operand()->type())->isSigned())
                return mBuilder.CreateSIToFP(op, ctype);
            else
                return mBuilder.CreateUIToFP(op, ctype);
        }
    }

    /*
     * Reference conversions
     */
    else
        return mBuilder.CreateBitCast(op, ctype);

    assert (false);
}


void*
CodeGenerator::visitCharacterLiteral(CharacterLiteral* lit)
{
    return llvm::ConstantInt::get(llvm::IntegerType::Int32Ty,
                                  lit->value(),
                                  false);
}


void*
CodeGenerator::visitCombinedRelationalExpression(
    CombinedRelationalExpression* expr)
{
    /*
     * If the left-hand operand is a built-in type, we can generate
     * optimized code for the combined relational expression using
     * createBuiltInMethodCall().
     */
    if (dynamic_cast<BuiltInType*>(expr->leftHand()->type()) != NULL)
    {
        Name methodName =
          (expr->comparisonCallee()->name() == Name("lessThan"))
            ? Name("<=")
            : Name(">=");

        return createBuiltInMethodCall(expr->leftHand(),
                                       methodName,
                                       expr->rightHand());

    }
    /*
     * Otherwise, the expression is transformed into a combination of calls
     * to the left-hand operand's equals() and lessThan()/greaterThan()
     * methods.
     * ('a <= b ' is the same as 'a.equals(b) && a.lessThan(b)';
     *  'a >= b ' is the same as 'a.equals(b) && a.greaterThan(b)'.)
     */
    else
    {
        llvm::Value* lh = (llvm::Value*) expr->leftHand()->visit(this);
        llvm::Value* rh = (llvm::Value*) expr->rightHand()->visit(this);

        Expression* lhExpr =
          new LLValueExpression(expr->leftHand()->type(), lh);
        Expression* rhExpr =
          new LLValueExpression(expr->rightHand()->type(), rh);

        Expression* callee1 =
          new InstanceFunctionExpression(lhExpr,
                                         (Function*) expr->equalsCallee());
        Expression* callee2 =
          new InstanceFunctionExpression(lhExpr,
                                         (Function*) expr->comparisonCallee());

        Expression* call1 = new CallExpression(callee1, &rhExpr, &rhExpr + 1);
        Expression* call2 = new CallExpression(callee2, &rhExpr, &rhExpr + 1);

        Expression* orExpr =
          new LogicalExpression(LogicalExpression::OR, call1, call2);

        void* ret = orExpr->visit(this);
        delete orExpr;
        return ret;
    }
}


void*
CodeGenerator::visitCompoundAssignmentExpression(CompoundAssignmentExpression* expr)
{
    // FIXME: Support for other callables
    assert (dynamic_cast<Function*>(expr->callee()) != NULL);

    /*
     * Generate code for the left-hand expression and wrap the resulting
     * LLVM value into an LLValueExprression. That way, we ensure that
     * the left-hand expression is evaluated only once.
     */

    bool tmp = mLValue;
    mLValue = true;
    llvm::Value* lh = (llvm::Value*) expr->leftHand()->visit(this);
    mLValue = tmp;

    Expression* lhExpr =  new LLValueExpression(expr->type(), lh);

    /*
     * Create a CallExpression that represents the operator method call.
     */
    Expression* instance;

    if (dynamic_cast<StructType*>(expr->leftHand()) != NULL)
        instance = lhExpr;
    else
        instance = new LLValueExpression(lhExpr->type(), mBuilder.CreateLoad(lh));

    Expression* callee =
      new InstanceFunctionExpression(instance, (Function*) expr->callee());

    Expression* arg = expr->rightHand();
    Expression* call = new CallExpression(callee, &arg, &arg + 1);

    /*
     * Let visitAssignmentExpression() do the rest.
     */
    AssignmentExpression assign(lhExpr, call);
    return assign.visit(this);
}

void*
CodeGenerator::visitEnumValueExpression(EnumValueExpression* expr)
{
    return llvm::ConstantInt::get(lltype(expr->type()),
                                  expr->target()->value().data(),
                                  expr->target()->value().isSigned());
}


void*
CodeGenerator::visitFloatingPointLiteral(FloatingPointLiteral* lit)
{
    return llvm::ConstantFP::get(lltype(lit->type()),
                                 lit->value().data());
}


void*
CodeGenerator::visitFunctionExpression(FunctionExpression* expr)
{
    llvm::Value* funcVal = mBuilder.CreateAlloca(lltype(expr->type()));

    llvm::Value* fptrField = mBuilder.CreateStructGEP(funcVal, 0);
    llvm::Function* fptr = llfunction(expr->target());

    llvm::Value* envField = mBuilder.CreateStructGEP(funcVal, 1);
    llvm::Value* env =
      llvm::ConstantPointerNull::get(
        llvm::PointerType::getUnqual(
          llvm::IntegerType::Int8Ty));

    mBuilder.CreateStore(fptr, fptrField);
    mBuilder.CreateStore(env, envField);

    return mBuilder.CreateLoad(funcVal);
}


void*
CodeGenerator::visitFunctionParameterExpression(
    FunctionParameterExpression* expr)
{
    return mFunction->getValueSymbolTable().lookup(mangledName(expr->target()));
}


void*
CodeGenerator::visitInstanceFunctionExpression(InstanceFunctionExpression* expr)
{
    const llvm::Type* funcType = lltype(expr->type());
    llvm::Value* funcVal = mBuilder.CreateAlloca(funcType);

    llvm::Value* fptrField = mBuilder.CreateStructGEP(funcVal, 0);
    llvm::Function* fptr = llfunction(expr->target());

    /*
     * Because the LLVM instance function's signature includes a leading
     * instance reference parameter, but the LLVM function type has only
     * the other parameters, we need to cast the function pointer.
     */
    llvm::Value* fptrCast =
      mBuilder.CreateBitCast(
        fptr,
        ((llvm::PointerType*) fptrField->getType())->getElementType());

    llvm::Value* envField = mBuilder.CreateStructGEP(funcVal, 1);
    mLValue = true;
    llvm::Value* env =
      mBuilder.CreateBitCast(
        (llvm::Value*) expr->instance()->visit(this),
        llvm::PointerType::getUnqual(llvm::IntegerType::Int8Ty));
    mLValue = false;

    mBuilder.CreateStore(fptrCast, fptrField);
    mBuilder.CreateStore(env, envField);

    return mBuilder.CreateLoad(funcVal);
}


void*
CodeGenerator::visitInstanceVariableExpression(InstanceVariableExpression* expr)
{
    bool tmp = mLValue;
    mLValue = true;
    llvm::Value* instanceVal = (llvm::Value*) expr->instance()->visit(this);
    mLValue = tmp;

    UserDefinedType* utype =
      dynamic_cast<UserDefinedType*>(expr->instance()->type());

    assert (utype != NULL);

    /*
     * If the instance's type is a class, we need to retrieve the correct
     * instance data pointer fist (see classPrivate()).
     */
    if (dynamic_cast<ClassType*>(utype) != NULL)
        instanceVal = classPrivate(instanceVal, (ClassType*) utype);

    /*
     * To retrieve the right field from the struct or class, we need to know
     * its index. Because the order of fields in the representing LLVM struct
     * is the same as the instance variable declaration order in the Soya
     * struct body, we get the index by looking where the variable to retrieve
     * is placed in the struct's or class' body.
     */

    int index = 0;

    for (DeclarationBlock::declarations_iterator it =
           utype->body()->declarations_begin();
         it != utype->body()->declarations_end(); it++)
    {
        if ((*it)->declaredEntity() == expr->target())
            break;
        else
            index++;
    }

    /*
     * Now that we have the index, we can retrieve the right field, using
     * either an LLVM 'getelementptr' instruction when the the instance
     * value is a struct pointer, or 'extractvalue' if it is a direct
     * struct value.
     */

    llvm::Value* ret;

    if (llvm::isa<llvm::PointerType>(instanceVal->getType()))
    {
        ret = mBuilder.CreateStructGEP(instanceVal, index);

        if (!mLValue)
            ret = mBuilder.CreateLoad(ret);
    }
    else
        ret = mBuilder.CreateExtractValue(instanceVal, index);

    return ret;
}


void*
CodeGenerator::visitIntegerLiteral(IntegerLiteral* lit)
{
    return llvm::ConstantInt::get(lltype(lit->type()),
                                  lit->value().data(),
                                  lit->value().isSigned());
}


void*
CodeGenerator::visitLogicalExpression(LogicalExpression* expr)
{
    llvm::Value* lh = (llvm::Value*) expr->leftHand()->visit(this);

    llvm::BasicBlock* previous = mBuilder.GetInsertBlock();

    llvm::BasicBlock* rhNeeded = llvm::BasicBlock::Create("", mFunction);
    llvm::BasicBlock* end = llvm::BasicBlock::Create("", mFunction);

    if (expr->kind() == LogicalExpression::AND)
        mBuilder.CreateCondBr(lh, rhNeeded, end);
    else
        mBuilder.CreateCondBr(lh, end, rhNeeded);

    mBuilder.SetInsertPoint(rhNeeded);
    llvm::Value* rh = (llvm::Value*) expr->rightHand()->visit(this);
    mBuilder.CreateBr(end);

    mBuilder.SetInsertPoint(end);

    llvm::PHINode* phi = mBuilder.CreatePHI(lh->getType());
    phi->addIncoming(lh, previous);
    phi->addIncoming(rh, rhNeeded);

    return phi;
}


void*
CodeGenerator::visitLogicalNotExpression(LogicalNotExpression* expr)
{
    if (dynamic_cast<CallExpression*>(expr->operand()) != NULL)
    {
        CallExpression* call = (CallExpression*) expr->operand();

        InstanceFunctionExpression* funcExpr =
          dynamic_cast<InstanceFunctionExpression*>(call->callee());

        if (funcExpr != NULL &&
            funcExpr->target()->name() == Name("equals") &&
            dynamic_cast<BuiltInType*>(funcExpr->instance()->type()) != NULL)
        {
            return createBuiltInMethodCall(funcExpr->instance(),
                                     Name("!="),
                                     *call->arguments_begin());
        }
    }

    llvm::Value* op = (llvm::Value*) expr->operand()->visit(this);
    return mBuilder.CreateXor(op, llvm::ConstantInt::getTrue());
}


void*
CodeGenerator::visitObjectCreationExpression(ObjectCreationExpression* expr)
{
    if (dynamic_cast<StructType*>(expr->type()) != NULL ||
        dynamic_cast<ClassType*>(expr->type()) != NULL)
    {
        llvm::Function* constructorFunc = llfunction(expr->constructor());
        std::vector<llvm::Value*> args;
        llvm::Value* instance;

        /*
         * Allocate memory for the instance to create. If the case of a class
         * instance, we do this by calling the class' allocation function
         * (see llallocator()). For structs, we allocate the memory on the
         * heap using an LLVM "alloca" instruction.
         */
        if (dynamic_cast<ClassType*>(expr->type()) != NULL)
        {
            instance =
              mBuilder.CreateCall(llallocator((ClassType*) expr->type()));
        }
        else
            instance = mBuilder.CreateAlloca(lltype(expr->type()));

        args.push_back(instance);

        for (ObjectCreationExpression::arguments_iterator it =
               expr->arguments_begin();
             it != expr->arguments_end(); it++)
        {
            args.push_back((llvm::Value*) (*it)->visit(this));
        }

        mBuilder.CreateCall(constructorFunc, args.begin(), args.end());

        if (dynamic_cast<StructType*>(expr->type()) != NULL &&
            !mLValue)
        {
            instance = mBuilder.CreateLoad(instance);
        }

        return instance;
    }
    else
    {
        assert (false);
        return NULL;
    }
}


void*
CodeGenerator::visitThisExpression(ThisExpression* expr)
{
    return mFunction->getValueSymbolTable().lookup("this");
}


void*
CodeGenerator::visitVariableExpression(VariableExpression* expr)
{
    std::string name = mangledName(expr->target());
    llvm::Value* ret = mFunction->getValueSymbolTable().lookup(name);

    if (ret == NULL)
        ret = mModule->getGlobalVariable(name);

    if (!mLValue)
        ret = mBuilder.CreateLoad(ret);

    assert (ret != NULL);
    return ret;
}


}}
