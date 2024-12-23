/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef SOYA_ARRAY_CREATION_EXPRESSION_H_
#define SOYA_ARRAY_CREATION_EXPRESSION_H_

#include "ArrayType.h"
#include "Expression.h"
#include "NodeList.h"
#include "UnknownArrayType.h"
#include <cassert>

namespace soyac {
namespace ast {

/**
 * Represnts an array creation expression.
 *
 * @see ObjectCreationExpression
 */
class ArrayCreationExpression : public Expression {
public:
    /**
     * Iterates over the created array's elements (constant).
     */
    typedef NodeList<Expression>::const_iterator elements_iterator;

    /**
     * Creates an ArrayCreationExpression with elements.
     *
     * @param type              The created array's type. May be an
     * ArrayType or an UnknownArrayType instance.
     * @param lengthExpression  An expression yielding the created array's
     *                          length; may be null.
     * @param elements_begin    Start iterator for the created array's
     * elements.
     * @param elements_end      End iterator for the created array's
     * elements.
     */
    template <class InputIterator>
    ArrayCreationExpression(Type* type, Expression* lengthExpression,
        InputIterator elements_begin, InputIterator elements_end)
        : mType(type)
        , mLengthExpression(lengthExpression)
        , mElements(elements_begin, elements_end)

    {
        assert(type != nullptr);
        assert(dynamic_cast<ArrayType*>(type) != nullptr
            || dynamic_cast<UnknownArrayType*>(type) != nullptr);

        if (mElements.size() == 0) {
            assert(lengthExpression != nullptr);
        }
    }

    /**
     * Creates an ArrayCreationExpression.
     *
     * @param type              The created array's type.
     * @param lengthExpression  An expression yielding the created array's
     *                          length; may be null.
     */
    ArrayCreationExpression(ArrayType* type, Expression* lengthExpression);

    /**
     * Visits the ArrayCreationExpression.
     *
     * @see Node::visit()
     */
    virtual void* visit(Visitor* v);

    /**
     * Implementation of Expression::type().
     */
    Type* type() const;

    /**
     * Returns the expression yielding the length of the array created by
     * the expression. May be null if no such expression is defined for
     * the array creation expression.
     *
     * @return  The array length expression, or null.
     */
    Expression* lengthExpression() const;

    /**
     * Returns a constant iterator pointing to the first element
     * of the array created by the expression.
     *
     * @return  The start iterator for the created array's elements.
     */
    elements_iterator elements_begin() const;

    /**
     * Returns a constant iterator pointing past the last element
     * of the array created by the expression.
     *
     * @return  The start iterator for the created array's elements.
     */
    elements_iterator elements_end() const;

private:
    Link<Type> mType;
    Link<Expression> mLengthExpression;
    NodeList<Expression> mElements;
};

} // namespace ast
} // namespace soyac

#endif
