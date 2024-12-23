/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _TYPE_HPP
#define _TYPE_HPP

#include "DeclaredEntity.hpp"

namespace soyac {
namespace ast {

/**
 * Inherited by node classes representing types.
 *
 * (See the Soya Language Reference,)
 */
class Type : public DeclaredEntity {
public:
    /**
     * Creates a Type.
     *
     * @param name  The type's name.
     */
    Type(const Name& name);

    /**
     * Returns @c true if values of the type can be converted to the passed
     * other type (implicitly or explicitly).
     *
     * Every class inheriting from Type can override this method to
     * implement the conversion rules for the represented types. The
     * default implementation returns @c true if isImplicitlyConvertable()
     * returns @c true for the passed type.
     *
     * @param type  The other type.
     * @return      @c true if the declared type is castable to the other,
     *              @c false otherwise.
     */
    virtual bool isConvertableTo(Type* other) const;

    /**
     * Returns @c true if values of the type can be converted to the passed
     * other type implicitly.
     *
     * Every class inheriting from Type can override this method to
     * implement the conversion rules for the represented types. The
     * default implementation returns @c true if both types are identical,
     * or if the type is a subtype of the other (see isSubtypeOf())..
     *
     * @param type  The other type.
     * @return      @c true if the declared type is compatible with the
     * other;
     *              @c false otherwise.
     */
    virtual bool isImplicitlyConvertableTo(Type* other) const;

    /**
     * Returns @c true is the type is a subtype of the passed other type.
     *
     * Every class inheriting from Type can override this method to
     * implement the subtype relations of the represented types. The
     * default implementation returns @c false unconditionally.
     *
     * @param type  The other type.
     * @return      @c true if the type is a subtype of the other;
     *              @c false otherwise.
     */
    virtual bool isSubtypeOf(Type* other) const;
};

} // namespace ast
} // namespace soyac

#endif
