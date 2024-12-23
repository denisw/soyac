/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef SOYA_FLOATING_POINT_VALUE_H_
#define SOYA_FLOATING_POINT_VALUE_H_

namespace soyac {
namespace ast {

/**
 * Represents a floating-point value.
 */
class FloatingPointValue {
public:
    /**
     * Creates a FloatingPointValue.
     *
     * @param data             The floating-point data.
     * @param singlePrecision  Whether the value should be interpreted as
     *                         single-precision floating-point number.
     */
    FloatingPointValue(double data, bool singlePrecision);

    /**
     * Returns the integer value's data.
     *
     * @return  The floating-point data.
     */
    double data() const;

    /**
     * Returns @c true if the value should be interpreted as a
     * single-precision floating-point number. If it is meant
     * to be interpreted as double-precision value, @c false is
     * returned instead.
     *
     * @return  @c true if the value is single-precision;
     *          @c false otherwise.
     */
    bool isSinglePrecision() const;

private:
    double mData;
    bool mIsSinglePrecision;
};

} // namespace ast
} // namespace soyac

#endif
