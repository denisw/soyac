/*
 * soyac - The Soya programming language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _UNIT_TEST_UTIL_HPP
#define _UNIT_TEST_UTIL_HPP

#include "../../../src/ast/Visitor.hpp"

/**
 * Declares and defines a Visitor implementation which can
 * be used to check if the visit() method of a specific
 * soyac::ast::Node subclass is correctly implemented.
 * For that class, the declared Visitor subclass implements
 * a visit method which returns @c true. All other visit
 * methods return 0.
 *
 * The name of the Node subclass must be specified as unqualified
 * name, that is, @i X instead of @i soyac::ast::X. The generated
 * Visitor subclass has a name of the form CTestVisitor, where
 * C is the Node subclass' name.
 *
 * This macro must be used with an appended semicolon.
 */
#define TEST_VISITOR(NCLASS) \
    class NCLASS##TestVisitor : public soyac::ast::Visitor \
    { \
    protected: \
        virtual void* visit##NCLASS(soyac::ast::NCLASS* node) \
        { \
            return (void*) true; \
        } \
    }

/**
 * Declares and defines a class derived from a specific abtsract Node subclass
 * which overrides that class' visit() method.
 *
 * This macro must be used with an appended semicolon.
 */
#define DUMMY_NODE(NCLASS) \
    class Dummy##NCLASS : public soyac::ast::NCLASS \
    { \
    public: \
        virtual void* visit(soyac::ast::Visitor* v) \
        { \
            return 0; \
        } \
    }

#endif
