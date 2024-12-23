/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include "IntegerValue.h"

namespace soyac {
namespace ast {

IntegerValue::IntegerValue(int64_t data, bool isSigned)
    : mData(data)
    , mIsSigned(false)
{
    /*
     * If true was passed for "isSigned" but no negative value was
     * provided, we leave mIsSigned as false. This normalization saves
     * us some work in the comparison operators.
     */
    if (isSigned && data < 0) {
        mIsSigned = true;
    }
}

int64_t IntegerValue::data() const { return mData; }

bool IntegerValue::isSigned() const { return mIsSigned; }

bool IntegerValue::operator==(const IntegerValue& other) const
{
    return (mData == other.mData && mIsSigned == other.mIsSigned);
}

bool IntegerValue::operator!=(const IntegerValue& other) const
{
    return !(*this == other);
}

bool IntegerValue::operator<(const IntegerValue& other) const
{
    if (mIsSigned) {
        if (!other.mIsSigned && other.mData < 0) {
            return true;
        } else {
            return mData < other.mData;
        }
    } else {
        if (other.mIsSigned) {
            return false;
        } else {
            return ((uint64_t)mData) < ((uint64_t)other.mData);
        }
    }
}

bool IntegerValue::operator>(const IntegerValue& other) const
{
    if (mIsSigned) {
        if (!other.mIsSigned && other.mData < 0) {
            return false;
        } else {
            return mData > other.mData;
        }
    } else {
        if (other.mIsSigned) {
            return true;
        } else {
            return ((uint64_t)mData) > ((uint64_t)other.mData);
        }
    }
}

bool IntegerValue::operator<=(const IntegerValue& other) const
{
    return (*this == other || *this < other);
}

bool IntegerValue::operator>=(const IntegerValue& other) const
{
    return (*this == other || *this > other);
}

} // namespace ast
} // namespace soyac
