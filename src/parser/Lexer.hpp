/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _LEXER_HPP
#define _LEXER_HPP

#include <deque>
#include <iostream>
#include <sstream>
#include <string>
#include <boost/regex/icu.hpp>

#include <ast/FloatingPointLiteral.hpp>
#include <ast/IntegerLiteral.hpp>
#include <common/PassResultBuilder.hpp>
#include "parser.hpp"

using namespace yy;
using soyac::ast::IntegerValue;

namespace soyac {
namespace parser
{

/**
 * Soyac's lexical analyzer.
 *
 * (See the Soya Language Reference, Chapter 3.)
 */
class Lexer
{
public:
    /**
     * Creates a Lexer.
     *
     * @param stream         The stream to read tokens from.
     * @param filename       The read source file's name (used as location
     *                       information).
     * @param resultBuilder  A PassResultBuilder to which the Lexer adds
     *                       found syntax errors.
     */
    Lexer(std::istream* stream,
          const std::string& fileName,
          PassResultBuilder* resultBuilder);

    /**
     * Lexer destructor.
     */
    ~Lexer();

    /**
     * Returns the name of the read source file.
     *
     * @return  The read source file's name.
     */
    const std::string& fileName() const;

    /**
     * Reads the next token and returns an integer indicating the token's
     * type. Additionally, nextToken() stores the token's location and
     * semantic value in the @c yylval and @c yylloc structures.
     *
     * @param yylval  Storage for the read token's semantic value.
     * @param yylloc  Storage for the read token's location.
     * @return        The read token's ID.
     */
    int nextToken(SoyaParser::semantic_type* yylval,
                  SoyaParser::location_type* yylloc);

private:
    /*
     * NOTE
     * ====
     * The Lexer class is fully UTF-8 aware. To represent UTF-8 characters,
     * it uses null-terminated byte strings. Thus, all functions that return
     * characters are declared with "const char*" as return type. Likewise,
     * variables that hold characters are also declared as "const char*"
     * (e.g. mLastChar).
     */

    std::istream* mStream;
    std::string mFileName;
    PassResultBuilder* mResultBuilder;
    int mCurrentLine;
    int mCurrentColumn;
    std::deque<const char*> mCharBuffer;
    const char* mLastChar;

    /*
     * This variable determines whether the static regular expression objects
     * have already been initialized.
     */
    static bool sInitRegexDone;

    /*
     * Regular expressions.
     */
    static boost::u32regex sLineTerminator;
    static boost::u32regex sWhiteSpace;
    static boost::u32regex sIdentifierStartChar;
    static boost::u32regex sIdentifierChar;
    static boost::u32regex sIntegerType;

    /*
     * Maps Soya's keywords as strings to the IDs of their respective tokens.
     */
    static std::map<std::string, int> sKeywords;

    /**
     * Initializes all regular expression objects. Called when the first
     * Lexer is created.
     */
    static void initRegex();

    /**
     * Calls the PassResultBuilder::addError() method of the PassResultBuilder
     * instance passed to the Lexer constructor. As error location, the Lexer's
     * current stream position is passed.
     *
     * @see PassResultBuilder::addError()
     */
    void addError(const std::string& description);

    /**
     * Reads the next character from the input stream and proceeds to the next
     * character position in the stream. (This also includes incrementing
     * mCurrentColumn by one; however, nextChar() does not respect line
     * terminators and thus does not manage mCurrentLine.)
     *
     * If the end of the input stream was already reached before the call to
     * nextChar(), the null character (@c '\0') is returned; otherwise, the
     * return value is the read character.
     *
     * @return  The next character from the input stream, or the null character
     *          if the input stream's end is reached.
     */
    const char* nextChar();

    /**
     * Returns the character which is @c numChars characters ahead from the
     * current position in the input stream. If the end of the stream is
     * encountered, the null character (@c '\0') is returned.
     *
     * Other than nextChar(), lookAhead() does not change the Lexer's position
     * in the stream; that is, multiple calls to lookAhead() with the same
     * argument always return the same character as long as nextChar() is not
     * called.
     *
     * The read characters are buffered using bufferChars().
     *
     * @param numChars  The number of characters to look ahead from the
     *                  current position in the stream.
     * @return          The requested character, or the null character if the
     *                  input stream's end is reached.
     */
    const char* lookAhead(unsigned int numChars = 1);

    /**
     * Reads the next @c n UTF-8 characters (relative to the current position
     * in the input stream) from the input stream and stores them in the
     * Lexer's character buffer as null-terminated byte strings. If the end
     * of the input stream is reached, the null character (@c '\0') is put
     * into the buffer.
     *
     * @param n  The number of characters to buffer.
     */
    void bufferChars(int n);

    /**
     * Skips a line terminator.
     */
    void skipLineTerminator();

    /**
     * Skips a delimited comment.
     */
    void skipDelimitedComment();

    /**
     * Skips a single-line comment.
     */
    void skipSingleLineComment();

    /**
     * Reads an identifier or keyword and returns the read token's ID.
     *
     * @param yylval  The @c yylval argument passed to nextToken().
     * @return        The read token's ID.
     */
    int readIdentifierOrKeyword(SoyaParser::semantic_type* yylval);

    /**
     * Reads an integer or floating-point literal and returns the read
     * token's ID.
     *
     * @param yylval  The @c yylval argument passed to nextToken().
     * @return        The read token's ID.
     */
    int readIntegerOrFloatingPointLiteral(SoyaParser::semantic_type* yylval);

    /**
     * Reads a decimal integer literal and returns the read token's ID.
     * Called from readIntegerOrFloatingPointLiteral().
     *
     * @param yylval    The @c yylval argument passed to nextToken().
     * @param negative  Whether the read value should be considered negative.
     * @return          The read token's ID.
     */
    int readDecimalIntegerLiteral(SoyaParser::semantic_type* yylval,
                                  bool negative);

    /**
     * Reads a binary integer literal and returns the read token's ID.
     * Called from readIntegerOrFloatingPointLiteral().
     *
     * @param yylval    The @c yylval argument passed to nextToken().
     * @param negative  Whether the read value should be considered negative.
     * @return          The read token's ID.
     */
    int readBinaryIntegerLiteral(SoyaParser::semantic_type* yylval,
                                 bool negative);

    /**
     * Reads a octal integer literal and returns the read token's ID.
     * Called from readIntegerOrFloatingPointLiteral().
     *
     * @param yylval    The @c yylval argument passed to nextToken().
     * @param negative  Whether the read value should be considered negative.
     * @return          The read token's ID.
     */
    int readOctalIntegerLiteral(SoyaParser::semantic_type* yylval,
                                bool negative);

    /**
     * Reads a hexadecimal integer literal and returns the read token's ID.
     * Called from readIntegerOrFloatingPointLiteral().
     *
     * @param yylval    The @c yylval argument passed to nextToken().
     * @param negative  Whether the read value should be considered negative.
     * @return          The read token's ID.
     */
    int readHexadecimalIntegerLiteral(SoyaParser::semantic_type* yylval,
                                      bool negative);

    /**
     * Converts an integer literal (without number format prefixes like '0x')
     * to an IntegerValue and stores it in @c yylval. If the value is not in
     * the range of any of Soya's integer types, an error is reported and the
     * semantic value is set to null.
     *
     * @param yylval   The @c yylval argument passed to nextToken().
     * @param literal  The literal whose value should be stored in @c yylval.
     * @param base     The number base of the literal (e.g. 10 for decimal
     *                 integer literals).
     */
    void setSemanticValueFromIntegerLiteral(SoyaParser::semantic_type* yylval,
                                            const std::string& literal,
                                            int base);

    /**
     * Reads a floating-point literal and returns the read token's ID.
     * Called from readIntegerOrFloatingPointLiteral().
     *
     * @param yylval    The @c yylval argument passed to nextToken().
     * @param negative  Whether the read value should be considered negative.
     * @return          The read token's ID.
     */
    int readFloatingPointLiteral(SoyaParser::semantic_type* yylval,
                                 bool negative);

    /**
     * Reads a character literal and returns the read token's ID.
     *
     * @param yylval    The @c yylval argument passed to nextToken().
     * @return          The read token's ID.
     */
    int readCharacterLiteral(SoyaParser::semantic_type* yylval);

    /**
     * Reads an escape character from the input stream and returns the
     * corresponding Unicode character.
     *
     * @return  The character represented by the read escape character.
     */
    const char* readEscapeChar();

    /**
     * Reads a punctator or operator and returns the read token's ID. If the
     * next characters in the input stream do not match any punctator or
     * operator of Soya, -1 is returned instead.
     *
     * @return  The read token's ID, or -1 if no punctator or operator
     *          was found.
     */
    int readPunctatorOrOperator();
};

/**
 * The yylex() function required by the parser.
 *
 * @param yylval  Storage for the read token's semantic value.
 * @param yylloc  Storage for the read token's location.
 * @param lexer   The Lexer from which the token should be read.
 * @return        The read token's ID.
 *
 * @see Lexer::nextToken()
 */
int yylex (SoyaParser::semantic_type* yylval,
           SoyaParser::location_type* yylloc,
           ParserDriver* driver);

}}

#endif
