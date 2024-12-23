/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include "FloatingPointValue.h"

namespace soyac {
namespace ast {

FloatingPointValue::FloatingPointValue(double data, bool singlePrecision)
    : mData(data)
    , mIsSinglePrecision(singlePrecision)
{
}

double FloatingPointValue::data() const { return mData; }

bool FloatingPointValue::isSinglePrecision() const
{
    return mIsSinglePrecision;
}

} // namespace ast
} // namespace soyac
