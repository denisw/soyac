/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include <cassert>
#include "Property.hpp"
#include "UnknownType.hpp"
#include "Visitor.hpp"

namespace soyac {
namespace ast
{


Property::Property(const Name& name,
                   Type* type,
                   PropertyGetAccessor* getAccessor,
                   PropertySetAccessor* setAccessor)
    : DeclaredEntity(name),
      mType(type)
{
    assert (type != NULL);

    mGetAccessor.targetChanged().connect(
      sigc::mem_fun(this, &Property::onGetAccessorChanged));
    mSetAccessor.targetChanged().connect(
      sigc::mem_fun(this, &Property::onSetAccessorChanged));

    mGetAccessor.setTarget(getAccessor);
    mSetAccessor.setTarget(setAccessor);
}


void*
Property::visit(Visitor* v)
{
    return v->visitProperty(this);
}


Type*
Property::type() const
{
    return mType.target();
}


PropertyGetAccessor*
Property::getAccessor() const
{
    return mGetAccessor.target();
}


PropertySetAccessor*
Property::setAccessor() const
{
    return mSetAccessor.target();
}


void
Property::onGetAccessorChanged(PropertyGetAccessor* oldAcc,
                               PropertyGetAccessor* newAcc)
{
    if (oldAcc != NULL)
        removeChild(oldAcc);

    if (newAcc != NULL)
    {
        addChild(newAcc);
        newAcc->returnType()->replaceWith(type());
    }
}


void
Property::onSetAccessorChanged(PropertySetAccessor* oldAcc,
                               PropertySetAccessor* newAcc)
{
    if (oldAcc != NULL)
        removeChild(oldAcc);

    if (newAcc != NULL)
    {
        addChild(newAcc);
        newAcc->valueParameter()->type()->replaceWith(type());
    }
}


}}
