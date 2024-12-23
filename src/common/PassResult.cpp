/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include "PassResult.hpp"

namespace soyac {

PassResult::PassResult()
    : mProblems(0)
{
}

PassResult::~PassResult()
{
    for (PassResult::problems_iterator it = problems_begin();
        it != problems_end(); it++) {
        delete *it;
    }
}

bool PassResult::foundProblems() const { return mProblems.size() > 0; }

bool PassResult::foundErrors() const
{
    if (mProblems.size() == 0) {
        return false;
    }
    for (PassResult::problems_iterator it = problems_begin();
        it != problems_end(); it++) {
        if ((*it)->type() == Problem::ERROR) {
            return true;
        }
    }

    return false;
}

PassResult::problems_iterator PassResult::problems_begin() const
{
    return mProblems.begin();
}

PassResult::problems_iterator PassResult::problems_end() const
{
    return mProblems.end();
}

} // namespace soyac
