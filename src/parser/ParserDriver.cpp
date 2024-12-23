/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include "ParserDriver.h"
#include <cerrno>
#include <cstring>
#include <iostream>

namespace soyac {
namespace parser {

ParserDriver::ParserDriver(const std::string& fileName)
    : mFileName(fileName)
    , mLexer(nullptr)
{
}

soyac::ast::Module* ParserDriver::parse(PassResult*& result)
{
    std::ifstream stream;
    stream.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    stream.open(mFileName.c_str(), std::ios_base::binary);

    mResultBuilder = new PassResultBuilder;
    mLexer = new Lexer(&stream, mFileName, mResultBuilder);

    yy::SoyaParser parser(this);

    /*
     * A return value of 1 indicates failure.
     */
    try {
        if (parser.parse() == 1) {
            mSyntaxTree = nullptr;
        }
    } catch (const std::ifstream::failure& exc) {
        delete mLexer;
        mLexer = nullptr;
        delete mResultBuilder;

        throw exc;
    }

    delete mLexer;
    mLexer = nullptr;

    result = mResultBuilder->result();
    delete mResultBuilder;

    return mSyntaxTree;
}

Lexer* ParserDriver::lexer() const { return mLexer; }

PassResultBuilder* ParserDriver::resultBuilder() const
{
    return mResultBuilder;
}

void ParserDriver::setSyntaxTree(soyac::ast::Module* syntaxTree)
{
    mSyntaxTree = syntaxTree;
}

} // namespace parser
} // namespace soyac
