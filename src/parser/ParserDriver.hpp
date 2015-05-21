/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _PARSER_DRIVER_HPP
#define _PARSER_DRIVER_HPP

#include <fstream>
#include <ast/Module.hpp>
#include <common/PassResultBuilder.hpp>
#include "Lexer.hpp"
#include "parser.hpp"

namespace soyac {
namespace parser
{

/*
 * Forward-declare this helper function of the parser to declare it as
 * friend. (See the comment in front of the friend declaration.)
 */
class ParserDriver;

/**
 * Encapsulates the process of parsing a Soya source file.
 */
class ParserDriver
{
public:
    /**
     * Creates a ParserDriver.
     *
     * @param fileName  The path to the source file to parse.
     */
    ParserDriver(const std::string& fileName);

    /**
     * Parses the source file passed to the ParserDriver and returns the
     * resulting abstract syntax tree. If any syntax errors are found, a
     * PassResult with the error information is created, to which a pointer
     * is stored at the passed location.
     *
     * If the parser encounters an unrecoverable error state and thus cannot
     * complete the parsing process, null is returned.
     *
     * If any I/O errors occur while reading from the input file, a
     * std::ifstream::failure exception is thrown.
     *
     * @throw std::ifstream::failure  If an I/O error occurs during parsing.
     *
     * @param result  The location at which to store the PassResult pointer.
     * @return        The resulting abstract syntax tree, or null.
     */
    soyac::ast::Module* parse(PassResult*& result)
        throw (std::ifstream::failure);

    /**
     * Returns the lexical analyzer.
     *
     * @return  The lexical analyzer.
     */
    Lexer* lexer() const;

private:
    std::string mFileName;
    Lexer* mLexer;
    PassResultBuilder* mResultBuilder;
    soyac::ast::Module* mSyntaxTree;

    /*
     * yy::SoyaParser is made a friend of ParserDriver because it needs
     * to pass its built abstract syntax tree to its ParserDriver by
     * calling setSyntaxTree(). In addition, yy::SoyaParser uses the
     * ParserDriver's PassResultBuilder.
     */
    friend class yy::SoyaParser;

    /**
     * Returns a PassResultBuilder instance that the parser can use to
     * report syntax errors.
     *
     * @return  A PassResultBuilder.
     */
    PassResultBuilder* resultBuilder() const;

    /**
     * Allows the parser to set the abstract syntax tree returned by
     * parse().
     *
     * @param ast  The abstract syntax tree.
     */
    void setSyntaxTree(soyac::ast::Module* syntaxTree);
};

}}

#endif
