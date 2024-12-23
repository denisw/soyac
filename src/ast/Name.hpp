/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _NAME_HPP
#define _NAME_HPP

#include "Node.hpp"
#include <cassert>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace soyac {
namespace ast {

/**
 * Represents a name.
 *
 * (See the Soya Language Reference, Chapter 7.)
 */
class Name {
public:
    typedef std::vector<std::string>::const_iterator identifiers_iterator;

    /**
     * Creates a simple Name consisting of the passed single identifier.
     *
     * @param identifier  The identifier.
     */
    Name(const std::string& identifier);

    /**
     * Creates a simple Name consisting of the passed single identifier
     * (as null-terminated character array).
     *
     * @param identifier  The identifier.
     */
    Name(const char* identifier);

    /**
     * Creates a Name consisting of the identifiers pointed to by the passed
     * iterator range. The resulting Name must consist of at least one
     * identifier.
     *
     * @param first  The start iterator.
     * @param last   The end iterator.
     */
    template <class InputIterator> Name(InputIterator start, InputIterator end)
    {
        for (InputIterator it = start; it != end; it++) {
            mIdentifiers.push_back(*it);
        }

        assert(countIdentifiers() > 0);
    }

    /**
     * Returns a constant iterator pointing to the name's first identifier.
     *
     * @return  The start iterator for the name's identifiers.
     */
    identifiers_iterator identifiers_begin() const;

    /**
     * Returns an iterator pointing past the name's last identifier.
     *
     * @return  The end iterator for the name's identifiers.
     */
    identifiers_iterator identifiers_end() const;

    /**
     * Returns the first identifier of the name.
     *
     * @return  The name's first identifier.
     */
    std::string first() const;

    /**
     * Returns the last identifier of the name.
     *
     * @return  The name's last identifier.
     */
    std::string last() const;

    /**
     * Returns the number of identifiers of which the name consists.
     *
     * @return  The name's number of identifiers.
     */
    size_t countIdentifiers() const;

    /**
     * Identifies whether the Name is a simple name.
     * A simple name is a name which consists of exactly one identifier.
     *
     * @return  @c true if the name is a simple name,
     *          @c false otherwise.
     */
    bool isSimple() const;

    /**
     * Returns the name's string representation.
     *
     * @return  The name as string.
     */
    std::string str() const;

    /**
     * Returns @c true if the name and the passed other Name consist of
     * the same identifiers in the same order. Otherwise, @c false is
     * returned.
     *
     * @param other  The name to compare with.
     * @return       @c true if both names are equal;
     *               @c false otherwise.
     */
    bool operator==(const Name& other) const;

    /**
     * Returns the opposite of operator==().
     *
     * @param other  The name to compare with.
     * @return       @c true if both names are unequal;
     *               @c false otherwise.
     */
    bool operator!=(const Name& other) const;

    /**
     * Creates and returns a Name which consists of the name's identifiers
     * and, additionally, the names of the passed other Name instance. For
     * instance, adding the name "baz" to "foo.bar" would result in the name
     * "foo.bar.baz".
     *
     * @param other  The name to be added.
     * @return       The resulting name.
     */
    Name operator+(const Name& other) const;

private:
    std::vector<std::string> mIdentifiers;
};

/**
 * Writes the passed name's string representation into an input stream.
 *
 * @param s     The stream to write to.
 * @param name  The Name whose string representation should be written.
 */
std::ostream& operator<<(std::ostream& s, const Name& name);

} // namespace ast
} // namespace soyac

#endif
