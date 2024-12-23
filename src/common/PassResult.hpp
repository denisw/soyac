/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _PASS_RESULT_HPP
#define _PASS_RESULT_HPP

#include "Problem.hpp"
#include <list>

namespace soyac {

/**
 * Holds information about the result of a compiler pass.
 *
 * PassResult instances are created using the PassResultBuilder class.
 */
class PassResult {
public:
    /**
     * Iterates over the problems found in the pass.
     */
    typedef std::list<Problem*>::const_iterator problems_iterator;

    /**
     * PassResult destructor.
     */
    ~PassResult();

    /**
     * Returns @c true if the compiler pass that returned the PassResult
     * found any problems (errors or warnings) in the processed abstract
     * syntax tree.
     *
     * @return  @c true if the pass found problems;
     *          @c false otherwise.
     */
    bool foundProblems() const;

    /**
     * Returns @c true if the compiler pass that returned the PassResult
     * found any errors in the processed abstract syntax tree.
     *
     * @return  @c true if the pass found errors;
     *          @c false otherwise.
     */
    bool foundErrors() const;

    /**
     * Returns a constant iterator pointing to the first problem found by
     * the compiler pass that returned the PassResult.
     *
     * @return  The start iterator for the found problems.
     */
    problems_iterator problems_begin() const;

    /**
     * Returns a constant iterator pointing past the last problem found by
     * the compiler pass that returned the PassResult.
     *
     * @return  The end iterator for the found problems.
     */
    problems_iterator problems_end() const;

private:
    std::list<Problem*> mProblems;
    bool mFoundErrors;

    /*
     * PassResultBuilder is made a friend class of PassResult so that it can
     * directly add problems to a PassResult's internal problem list.
     */
    friend class PassResultBuilder;

    /*
     * PassResult constructor.
     */
    PassResult();
};

} // namespace soyac

#endif
