/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include <sstream>
#include "IntegerType.hpp"
#include "BooleanType.hpp"
#include "CharacterType.hpp"
#include "FloatingPointType.hpp"
#include "Function.hpp"
#include "FunctionParameter.hpp"
#include "Visitor.hpp"

namespace soyac {
namespace ast
{

std::map<std::pair<int, bool>, IntegerType*> IntegerType::sInstances;
bool IntegerType::sInitialized = false;

std::set<std::string> IntegerType::sArithmeticMethods;
std::set<std::string> IntegerType::sRelationalMethods;
std::set<std::string> IntegerType::sBitShiftMethods;


void
IntegerType::initialize()
{
    const std::string arithmetic[5] = {
        std::string("plus"),
        std::string("minus"),
        std::string("mul"),
        std::string("div"),
        std::string("mod")
    };

    const std::string relational[3] = {
        std::string("equals"),
        std::string("lessThan"),
        std::string("greaterThan")
    };

    const std::string bitShift[2] = {
        std::string("lshift"),
        std::string("rshift")
    };

    sArithmeticMethods = std::set<std::string>(arithmetic, arithmetic + 5);
    sRelationalMethods = std::set<std::string>(relational, relational + 3);
    sBitShiftMethods   = std::set<std::string>(bitShift, bitShift + 2);
}


IntegerType*
IntegerType::get(int size, bool isSigned)
{
    /*
     * We need to have all static members properly initialized before
     * creating any IntegerType, so do that if we haven't already done
     * so.
     */
    if (!sInitialized)
    {
        initialize();
        sInitialized = true;
    }

    std::pair<int, bool> p(size, isSigned);

    /*
     * If an instance with the specified size and signedness has not been
     * requested yet, create it now.
     */
    if (sInstances[p] == NULL)
    {
        /*
         * Determine the requested integer type's name. The name starts
         * with "int" for signed and with "uint" for unsigned integer
         * types and is suffixed with the type's size.
         */

        std::stringstream name;

        if (size == -1)
        {
            name << (isSigned ? "long" : "ulong");
        }
        else
        {
            assert (size >= 0);
            name << (isSigned ? "int" : "uint");

            /*
             * Only prepend the integer type's size to the name if the
             * requested type is not "int" or "uint" (those obviously don't
             * have a size in their name).
             */
            if (size > 0)
                name << size;
        }

        sInstances[p] = new IntegerType(Name(name.str()), size, isSigned);
        sInstances[p]->ref();

        /*
         * FIXME: This should be set depending on the target architecture.
         */
        if (size == 0)
            sInstances[p]->mSize = 32;
        else if (size == -1)
            sInstances[p]->mSize = 64;

        onNewInstance(sInstances[p]);
    }

    return sInstances[p];
}


IntegerType*
IntegerType::getInt()
{
    return IntegerType::get(0, true);
}


IntegerType*
IntegerType::getUInt()
{
    return IntegerType::get(0, false);
}


IntegerType*
IntegerType::getLong()
{
    return IntegerType::get(-1, true);
}


IntegerType*
IntegerType::getULong()
{
    return IntegerType::get(-1, false);
}


void
IntegerType::onNewInstance(IntegerType* newType)
{
    static const std::string arithmetic[5] = {
        std::string("plus"),
        std::string("minus"),
        std::string("mul"),
        std::string("div"),
        std::string("mod")
    };

    static const std::string relational[3] = {
        std::string("equals"),
        std::string("lessThan"),
        std::string("greaterThan")
    };

    static const std::string bitshift[2] = {
        std::string("lshift"),
        std::string("rshift")
    };

    /*
     * Add the "lshift()" and "rshift()" methods to the new type.
     */
    for (int i = 0; i < 2; i++)
    {
        FunctionParameter* p = new FunctionParameter("x", TYPE_INT);
        Function* m = new Function(bitshift[i], TYPE_BOOL, &p, &p + 1);
        newType->addMember(m);
    }

    /*
     * Add the built-in integer type methods to the new integer type, each
     * with one overload for every existing IntegerType to which this
     * type is implicitly convertable. At the same time, if there are
     * already-created IntegerType instances whose represented type is
     * implicitly convertable to the new type, those need overloads
     * accepting this type too, so we also create them here.
     */
    for (std::map<std::pair<int, bool>, IntegerType*>::iterator it =
           sInstances.begin();
         it != sInstances.end(); it++)
    {
        if ((newType->size() == it->second->size() &&
             newType->isSigned() == it->second->isSigned() &&
             newType != it->second) ||
            (it->second == TYPE_LONG && newType != TYPE_LONG) ||
            (it->second == TYPE_ULONG && newType != TYPE_ULONG))
        {
            continue;
        }

        IntegerType* callee;
        IntegerType* arg;

        if (newType->isImplicitlyConvertableTo(it->second))
        {
            callee = newType;
            arg = it->second;
        }
        else if (it->second->isImplicitlyConvertableTo(newType))
        {
            callee = it->second;
            arg = newType;
        }
        else
            continue;
        
        /*
         * Arithmetic Methods
         */
        for (int i = 0; i < 5; i++)
        {
            FunctionParameter* p = new FunctionParameter("x", arg);
            Function* m = new Function(arithmetic[i], arg, &p, &p + 1);
            callee->addMember(m);
        }

        /*
         * Relational Methods
         */
        for (int i = 0; i < 3; i++)
        {
            FunctionParameter* p = new FunctionParameter("x", arg);
            Function* m = new Function(relational[i], TYPE_BOOL, &p, &p + 1);
            callee->addMember(m);
        }
    }
}


IntegerType::IntegerType(const Name& name, int size, bool isSigned)
    : BuiltInType(name),
      mSize(size),
      mIsSigned(isSigned)
{
}


void*
IntegerType::visit(Visitor* v)
{
    return v->visitIntegerType(this);
}


bool
IntegerType::isConvertableTo(Type* other) const
{
    return (dynamic_cast<IntegerType*>(other) != NULL ||
            dynamic_cast<FloatingPointType*>(other) != NULL ||
            other == TYPE_CHAR);
}


bool
IntegerType::isImplicitlyConvertableTo(Type* other) const
{
    if (Type::isImplicitlyConvertableTo(other) ||
        dynamic_cast<FloatingPointType*>(other) != NULL)
    {
        return true;
    }
    else
    {
        IntegerType* int2 = dynamic_cast<IntegerType*>(other);

        if (int2 == NULL)
            return false;
        else
            return min() >= int2->min() && max() <= int2->max();
    }
}


int
IntegerType::size() const
{
    return mSize;
}


bool
IntegerType::isSigned() const
{
    return mIsSigned;
}


IntegerValue
IntegerType::min() const
{
    int64_t min;

    if (isSigned())
    {
        min = -2;

        for (int s = 2; s < size(); s++)
            min *= 2;
    }
    else
        min = 0;

    return IntegerValue(min, isSigned());
}


IntegerValue
IntegerType::max() const
{
    int64_t max = 2;

    for (int s = isSigned() ? 2 : 1; s < size(); s++)
        max *= 2;

    max -= 1;
    return IntegerValue(max, isSigned());
}


}}
