/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef SOYA_UNKNOWN_TYPE_H_
#define SOYA_UNKNOWN_TYPE_H_

#include "Type.h"
#include <list>

/**
 * The UnknownType singleton.
 */
#define TYPE_UNKNOWN soyac::ast::UnknownType::getSingleton()

namespace soyac {
namespace ast {

/**
 * Placeholder for a type which cannot or must still be determined.
 *
 * If a generic placeholder for an unknown type is needed,
 * UnknownType::getSingleton() or the TYPE_UNKNOWN macro is used.
 * However, individual UnknownType instances are created to represent
 * types with a known name (which can be passed to the UnknownType()
 * constructor). The name is then used by the BasicAnalyzer to determine
 * the denoted type.
 */
class UnknownType : public Type {
public:
    /**
     * Creates an UnknownType with a known (possibly qualified) name.
     *
     * @param name  The unknown type's name. May be qualified. (name()
     *              still only returns the simple name in this case, but
     *              qualifiedName() will always returns the full passed
     *              name.)
     */
    UnknownType(const Name& name);

    /**
     * UnknownType destructor.
     */
    virtual ~UnknownType();

    /**
     * Returns the UnknownType singleton instance. This can be used if
     * nothing is known about the type to represent, not even the name.
     *
     * @return The UnknownType singleton.
     */
    static UnknownType* getSingleton();

    /**
     * Visits the UnknownType.
     *
     * @see Node::visit()
     */
    virtual void* visit(Visitor* v);

    /**
     * Override of Type::isImplicitlyConvertable() that ensures
     * that an unknown type won't produce type mismatch errors.
     */
    virtual bool isImplicitlyConvertableTo(Type* other) const;

private:
    static UnknownType* sInstance;

    /*
     * This is needed when a qualified name is passed to the UnknownType
     * constructor. See the comment in UnknownType for more details.
     */
    std::list<NamedEntity*> mQualifiedNameParents;
};

} // namespace ast
} // namespace soyac

#endif
