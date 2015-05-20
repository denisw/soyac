/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _INTEGER_VALUE_HPP
#define _INTEGER_VALUE_HPP

#include <stdint.h>

namespace soyac {
namespace ast
{

/**
 * Represents an integer value. In order to be able to represent
 * the complete range of possible integer values, an IntegerValue
 * consists of two parts:
 *
 * - A 64-bit "data" member which stores the value's bit representation.
 * - A signdness flag which determines whether the stored bits are
 *   interpreted as signed or unsigned value.
 */
class IntegerValue
{
public:
    /**
     * Creates an IntegerValue.
     *
     * @param data      The integer data.
     * @param isSigned  Whether the value should be interpreted as
     *                  a signed integer.
     */
    IntegerValue(int64_t data, bool isSigned);

    /**
     * Returns the integer value's data, without information about its
     * interpretation (signed or unsigned).
     *
     * @return  The integer data.
     */
    int64_t data() const;

    /**
     * Returns @c true if the integer data returned by data() is interpreted
     * as signed or unsigned integer value. Note that the return value isn't
     * necessarily the same as the one passed to the IntegerValue at
     * construction - for normalization purposes, isSigned() always returns
     * @c false if both the signed and unsigned interpretation would yield
     * the same mathematical value (that is, if the value is between 0 and
     * @c INT64_MAX).
     *
     * @return  @c true if the value is signed;
     *          @c false if it is unsigned.
     */
    bool isSigned() const;
    
    /**
     * Returns @c true if the two integer values are identical.
     *
     * @return  @c true if both values are identical;
     *          @c false otherwise.
     */
    bool operator==(const IntegerValue& other) const;

    /**
     * Returns the opposite of operator==().
     */
    bool operator!=(const IntegerValue& other) const;

    /**
     * Returns @c true if the integer value is smaller than the passed
     * other value.
     *
     * @return  @c true if the value is smaller than the other;
     *          @c false otherwise.
     */
    bool operator<(const IntegerValue& other) const;

    /**
     * Returns @c true if the integer value is larger than the passed
     * other value.
     *
     * @return  @c true if the value is larger than the other;
     *          @c false otherwise.
     */
    bool operator>(const IntegerValue& other) const;

    /**
     * Returns @c true if the integer value is equal to or smaller than the
     * passed other value.
     *
     * @return  @c true if the value is at most as large as the other;
     *          @c false otherwise.
     */
    bool operator<=(const IntegerValue& other) const;

    /**
     * Returns @c true if the integer value is equal to or larger than the
     * passed other value.
     *
     * @return  @c true if the value is at least as large as the other;
     *          @c false otherwise.
     */
    bool operator>=(const IntegerValue& other) const;

private:
    int64_t mData;
    bool mIsSigned;
};

}}

#endif
