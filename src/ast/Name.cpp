/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include "Name.hpp"

namespace soyac {
namespace ast {

Name::Name(const std::string& identifier)
{
    mIdentifiers.push_back(identifier);
}

Name::Name(const char* identifier)
{
    mIdentifiers.push_back(std::string(identifier));
}

Name::identifiers_iterator Name::identifiers_begin() const
{
    return mIdentifiers.begin();
}

Name::identifiers_iterator Name::identifiers_end() const
{
    return mIdentifiers.end();
}

std::string Name::first() const { return mIdentifiers.front(); }

std::string Name::last() const { return mIdentifiers.back(); }

size_t Name::countIdentifiers() const { return mIdentifiers.size(); }

bool Name::isSimple() const { return countIdentifiers() == 1; }

std::string Name::str() const
{
    std::stringstream s;
    s << *this;
    return s.str();
}

bool Name::operator==(const Name& other) const
{
    if (countIdentifiers() != other.countIdentifiers()) {
        return false;
    }

    Name::identifiers_iterator it1 = identifiers_begin();
    Name::identifiers_iterator it2 = other.identifiers_begin();

    do {
        if (*it1 != *it2) {
            return false;
        }

        it1++;
        it2++;
    } while (it1 != identifiers_end());

    return true;
}

bool Name::operator!=(const Name& other) const { return !(*this == other); }

Name Name::operator+(const Name& other) const
{
    Name ret(identifiers_begin(), identifiers_end());

    ret.mIdentifiers.insert(ret.mIdentifiers.end(), other.identifiers_begin(),
        other.identifiers_end());

    return ret;
}

std::ostream& operator<<(std::ostream& s, const Name& name)
{
    Name::identifiers_iterator it = name.identifiers_begin();
    s << *it;
    it++;

    for (; it != name.identifiers_end(); it++) {
        s << ".";
        s << *it;
    }

    return s;
}

} // namespace ast
} // namespace soyac
