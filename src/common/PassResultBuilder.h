/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef SOYA_PASS_RESULT_BUILDER_H_
#define SOYA_PASS_RESULT_BUILDER_H_

#include "PassResult.h"
#include <ast/Node.h>
#include <boost/format.hpp>
#include <string>

namespace soyac {

/**
 * Used tp create PassResult instances.
 *
 * After constructing a PassResultBuilder, errors and warnings can be added
 * with the addError() and addWarning() methods. When finished, the built
 * PassResult instance can be obtained by calling result().
 */
class PassResultBuilder {
public:
    /**
     * Creates a PassResultBuilder.
     */
    PassResultBuilder();

    /**
     * Adds an error found at the specified location to the built PassResult.
     *
     * @param location     The source file location where the error was found.
     * @param description  The error's description.
     */
    void addError(
        const soyac::ast::Location& location, const std::string& description);

    /**
     * Adds an error found at the specified location to the built PassResult
     * (with a boost::format as description).
     *
     * @param location     The source file location where the error was found.
     * @param description  The error's description.
     */
    void addError(
        const soyac::ast::Location& location, const boost::format& description);

    /**
     * Adds an error to the built PassResult.
     *
     * @param source       The abstract syntax tree node which is the source
     *                     of the error.
     * @param description  The error's description.
     */
    void addError(
        const soyac::ast::Node* source, const std::string& description);

    /**
     * Adds an error to the built PassResult (with a boost::format as
     * description).
     *
     * @param source       The abstract syntax tree node which is the source
     *                     of the error.
     * @param description  The error's description.
     */
    void addError(
        const soyac::ast::Node* source, const boost::format& description);

    /**
     * Adds a warning to the built PassResult.
     *
     * @param source       The abstract syntax tree node which is the source
     *                     of the warning.
     * @param description  The warning's description.
     */
    void addWarning(
        const soyac::ast::Node* source, const std::string& description);

    /**
     * Adds a warning to the built PassResult (with a boost::format as
     * description).
     *
     * @param source       The abstract syntax tree node which is the source
     *                     of the warning.
     * @param description  The warning's description.
     */
    void addWarning(
        const soyac::ast::Node* source, const boost::format& description);

    /**
     * Returns the built PassResult instance. If no error or warning has
     * been added to the PassResultBuilder yet, null is returned instead.
     *
     * @return  The built PassResult, or null.
     */
    PassResult* result() const;

private:
    PassResult* mPassResult;
};

} // namespace soyac

#endif
