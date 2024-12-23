/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include "Property.h"
#include "UnknownType.h"
#include "Visitor.h"
#include <cassert>

namespace soyac {
namespace ast {

Property::Property(const Name& name, Type* type,
    PropertyGetAccessor* getAccessor, PropertySetAccessor* setAccessor)
    : DeclaredEntity(name)
    , mType(type)
{
    assert(type != nullptr);

    mGetAccessor.targetChanged().connect([this](auto oldAcc, auto newAcc) {
        onGetAccessorChanged(oldAcc, newAcc);
    });
    mSetAccessor.targetChanged().connect([this](auto oldAcc, auto newAcc) {
        onSetAccessorChanged(oldAcc, newAcc);
    });

    mGetAccessor.setTarget(getAccessor);
    mSetAccessor.setTarget(setAccessor);
}

void* Property::visit(Visitor* v) { return v->visitProperty(this); }

Type* Property::type() const { return mType.target(); }

PropertyGetAccessor* Property::getAccessor() const
{
    return mGetAccessor.target();
}

PropertySetAccessor* Property::setAccessor() const
{
    return mSetAccessor.target();
}

void Property::onGetAccessorChanged(
    PropertyGetAccessor* oldAcc, PropertyGetAccessor* newAcc)
{
    if (oldAcc != nullptr) {
        removeChild(oldAcc);
    }

    if (newAcc != nullptr) {
        addChild(newAcc);
        newAcc->returnType()->replaceWith(type());
    }
}

void Property::onSetAccessorChanged(
    PropertySetAccessor* oldAcc, PropertySetAccessor* newAcc)
{
    if (oldAcc != nullptr) {
        removeChild(oldAcc);
    }

    if (newAcc != nullptr) {
        addChild(newAcc);
        newAcc->valueParameter()->type()->replaceWith(type());
    }
}

} // namespace ast
} // namespace soyac
