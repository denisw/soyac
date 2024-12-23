/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef SOYA_INTEGER_TYPE_H_
#define SOYA_INTEGER_TYPE_H_

#include "BuiltInType.h"
#include "IntegerValue.h"
#include "NodeList.h"
#include <map>
#include <set>

/**
 * The "int" type. This is a shortcut to IntegerType::getInt().
 */
#define TYPE_INT soyac::ast::IntegerType::getInt()

/**
 * The "uint" type. This is a shortcut to IntegerType::getUInt().
 */
#define TYPE_UINT soyac::ast::IntegerType::getUInt()

/**
 * The "long" type. This is a shortcut to IntegerType::getLong().
 */
#define TYPE_LONG soyac::ast::IntegerType::getLong()

/**
 * The "uloung" type. This is a shortcut to IntegerType::getULong().
 */
#define TYPE_ULONG soyac::ast::IntegerType::getULong()

namespace soyac {
namespace ast {

/**
 * Represents an integer type.
 *
 * (See the Soya Language Reference,)
 */
class IntegerType : public BuiltInType {
public:
    /**
     * Returns an IntegerType. Passing the same arguments will
     * always yield the exact same IntegerType instance.
     *
     * @param size      The integer type's size.
     * @param isSigned  Whether the integer type is signed.
     * @return          The requested IntegerType.
     */
    static IntegerType* get(int size, bool isSigned);

    /**
     * Returns the IntegerType singleton instance representing
     * the "int" type.
     *
     * @return  The "int" type.
     */
    static IntegerType* getInt();

    /**
     * Returns the IntegerType singleton instance representing
     * the "uint" type.
     *
     * @return  The "uint" type.
     */
    static IntegerType* getUInt();

    /**
     * Returns the IntegerType singleton instance representing
     * the "long" type.
     *
     * @return  The "long" type.
     */
    static IntegerType* getLong();

    /**
     * Returns the IntegerType singleton instance representing
     * the "ulong" type.
     *
     * @return  The "ulong" type.
     */
    static IntegerType* getULong();

    /**
     * Visits the IntegerType.
     *
     * @see Node::visit()
     */
    virtual void* visit(Visitor* v);

    /**
     * Override of Type::isConvertableTo().
     */
    virtual bool isConvertableTo(Type* other) const;

    /**
     * Override of Type::isImplicitlyConvertableTo().
     */
    virtual bool isImplicitlyConvertableTo(Type* other) const;

    /**
     * Returns the integer type's size.
     *
     * @returns  The type's size.
     */
    int size() const;

    /**
     * Returns @c true if the integer type is signed. If it is unsigned,
     * @c false is returned.
     *
     * @return  @c true if the integer type is signed;
     *          @c false otherwise
     */
    bool isSigned() const;

    /**
     * Returns the smallest value that can be represented with the integer
     * type.
     *
     * @return  The smallest representable value.
     */
    IntegerValue min() const;

    /**
     * Returns the biggest value that can be represented with the integer
     * type.
     *
     * @return  The biggest representable value.
     */
    IntegerValue max() const;

private:
    static std::map<std::pair<int, bool>, IntegerType*> sInstances;
    static bool sInitialized;

    static std::set<std::string> sArithmeticMethods;
    static std::set<std::string> sRelationalMethods;
    static std::set<std::string> sBitShiftMethods;

    /**
     * Initializes IntegerType's static members.
     */
    static void initialize();

    /**
     * Called by IntgerType::get() whenever a new IntegerType has been
     * created and added to the instances map.
     *
     * @param newType  The new IntegerType instance.
     */
    static void onNewInstance(IntegerType* newType);

    int mSize;
    bool mIsSigned;

    /**
     * Creates an IntegerType.
     *
     * @param size      The integer type's size.
     * @param isSigned  Whether the integer type is signed.
     */
    IntegerType(const Name& name, int size, bool isSigned);
};

} // namespace ast
} // namespace soyac

#endif
