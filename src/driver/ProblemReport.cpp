/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include "ProblemReport.hpp"
#include <iostream>

namespace soyac {
namespace driver {

std::list<PassResult*> ProblemReport::sResults;

void ProblemReport::addPassResult(PassResult* result)
{
    sResults.push_back(result);
}

bool ProblemReport::show()
{
    int numErrors = 0;
    int numWarnings = 0;

    for (std::list<PassResult*>::iterator it = sResults.begin();
        it != sResults.end(); it++) {
        PassResult* result = *it;

        for (PassResult::problems_iterator it2 = result->problems_begin();
            it2 != result->problems_end(); it2++) {
            Problem* prob = *it2;

            std::cerr << std::endl;

            if (prob->type() == Problem::ERROR) {
                std::cerr << "error";
                numErrors++;
            } else {
                std::cerr << "warning";
                numWarnings++;
            }

            std::cerr << " (" << prob->location().fileName() << ", "
                      << prob->location().startLine() << "):" << std::endl
                      << prob->description() << std::endl;
        }

        delete result;
    }

    if (numErrors > 0 || numWarnings > 0) {
        std::cerr << std::endl;

        if (numErrors == 0) {
            std::cerr << "SUCCESS (";
        } else {
            std::cerr << "FAILURE (";
        }

        std::cerr << numErrors << " ";

        if (numErrors == 1) {
            std::cerr << "error";
        } else {
            std::cerr << "errors";
        }

        std::cerr << ", " << numWarnings << " ";

        if (numWarnings == 1) {
            std::cerr << "warning";
        } else {
            std::cerr << "warnings";
        }

        std::cerr << ")" << std::endl;
        return (numErrors > 0);
    } else {
        return false;
    }
}

} // namespace driver
} // namespace soyac
