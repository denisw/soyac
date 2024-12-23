/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include "UnknownType.h"
#include "Visitor.h"

namespace soyac {
namespace ast {

UnknownType* UnknownType::sInstance = 0;

UnknownType::UnknownType(const Name& name)
    : Type(Name(name.last()))
{
    if (!name.isSimple()) {
        /*
         * As we need to support UnknownType instances with qualified names,
         * but NamedEntity's name() method guarantees to return a simple
         * name, we need a small hack: we create auxiliary NamedEntity
         * instances and chain them together with addChild() so that the
         * UnknownType's qualifiedName() method will return the correct
         * full name, while name() will only return the simple name.
         */

        Name::identifiers_iterator it = name.identifiers_begin();
        mQualifiedNameParents.push_back(new NamedEntity(Name(*it)));

        it++;

        for (; it != --name.identifiers_end(); it++) {
            NamedEntity* parent = new NamedEntity(Name(*it));

            mQualifiedNameParents.back()->addChild(parent);
            mQualifiedNameParents.push_back(parent);
        }

        mQualifiedNameParents.back()->addChild(this);
    }
}

UnknownType::~UnknownType()
{
    for (std::list<NamedEntity*>::iterator it = mQualifiedNameParents.begin();
        it != mQualifiedNameParents.end(); it++) {
        delete *it;
    }
}

UnknownType* UnknownType::getSingleton()
{
    if (!sInstance) {
        sInstance = new UnknownType("(unknown)");
        sInstance->ref();
    }

    return sInstance;
}

void* UnknownType::visit(Visitor* v) { return v->visitUnknownType(this); }

bool UnknownType::isImplicitlyConvertableTo(Type* other) const { return true; }

} // namespace ast
} // namespace soyac
