/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef SOYA_PROBLEM_REPORT_H_
#define SOYA_PROBLEM_REPORT_H_

#include <common/PassResult.h>

namespace soyac {
namespace driver {

/**
 * Provides static methods for collecting the errors and warnings found
 * by the compiler passes and displaying them on the commandline.
 */
class ProblemReport {
public:
    /**
     * Adds the problems stored in the passed PassResult to the
     * error report.
     *
     * @param result  The PassResult to be added.
     */
    static void addPassResult(PassResult* result);

    /**
     * Writes the error report to the standard error stream (thereby
     * removing all PassResults from the report). If the problem report
     * contained any error messages (warnings are not considered), @c true
     * is returned; otherwise, the return value is @c false.
     *
     * @return  @c true if errors were printed;
     *          @c false otherwise.
     */
    static bool show();

private:
    static std::list<PassResult*> sResults;
};

} // namespace driver
} // namespace soyac

#endif
