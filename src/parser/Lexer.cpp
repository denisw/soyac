/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include <cassert>
#include <cerrno>
#include <cctype>
#include <cstdlib>
#include <stdint.h>
#include <regex>
#include <vector>

#include <boost/format.hpp>
#include "utf8.h"

#include <ast/IntegerType.hpp>
#include <ast/Location.hpp>
#include "Lexer.hpp"
#include "ParserDriver.hpp"

using namespace yy;

namespace soyac {
namespace parser
{

bool Lexer::sInitRegexDone;
std::regex Lexer::sLineTerminator;
std::regex Lexer::sWhiteSpace;
std::regex Lexer::sIdentifierStartChar;
std::regex Lexer::sIdentifierChar;
std::regex Lexer::sIntegerType;
std::map<std::string, int> Lexer::sKeywords;


Lexer::Lexer(std::istream* stream,
             const std::string& filename,
             PassResultBuilder* resultBuilder)
    : mStream(stream),
      mFileName(filename),
      mResultBuilder(resultBuilder),
      mCurrentLine(1),
      mCurrentColumn(1),
      mLastChar(NULL)
{
    assert (stream != NULL);

    if (!sInitRegexDone)
    {
        initRegex();
        sInitRegexDone = true;
    }
}


Lexer::~Lexer()
{
    if (mLastChar != NULL)
        delete [] mLastChar;
}


void
Lexer::initRegex()
{

    sWhiteSpace = std::regex("[ \\r\\n\\t]");

    sIdentifierStartChar = std::regex("[A-Za-z_]");
    sIdentifierChar      = std::regex("[A-Za-z0-9_]");

    sIntegerType = std::regex("(u)?int[1-9]?[0-9]*");

    sKeywords = std::map<std::string, int>();
    sKeywords["alias"]       = SoyaParser::token::ALIAS;
    sKeywords["as"]          = SoyaParser::token::AS;
    sKeywords["bool"]        = SoyaParser::token::BOOL;
    sKeywords["break"]       = SoyaParser::token::BREAK;
    sKeywords["case"]        = SoyaParser::token::CASE;
    sKeywords["char"]        = SoyaParser::token::CHAR;
    sKeywords["class"]       = SoyaParser::token::CLASS;
    sKeywords["const"]       = SoyaParser::token::CONST;
    sKeywords["constructor"] = SoyaParser::token::CONSTRUCTOR;
    sKeywords["continue"]    = SoyaParser::token::CONTINUE;
    sKeywords["default"]     = SoyaParser::token::DEFAULT;
    sKeywords["do"]          = SoyaParser::token::DO;
    sKeywords["double"]      = SoyaParser::token::DOUBLE;
    sKeywords["else"]        = SoyaParser::token::ELSE;
    sKeywords["enum"]        = SoyaParser::token::ENUM;
    sKeywords["extends"]     = SoyaParser::token::EXTENDS;
    sKeywords["extern"]      = SoyaParser::token::EXTERN;
    sKeywords["false"]       = SoyaParser::token::FALSE_;
    sKeywords["float"]       = SoyaParser::token::FLOAT;
    sKeywords["for"]         = SoyaParser::token::FOR;
    sKeywords["from"]        = SoyaParser::token::FROM;
    sKeywords["function"]    = SoyaParser::token::FUNCTION;
    sKeywords["get"]         = SoyaParser::token::GET;
    sKeywords["if"]          = SoyaParser::token::IF;
    sKeywords["is"]          = SoyaParser::token::IS;
    sKeywords["implements"]  = SoyaParser::token::IMPLEMENTS;
    sKeywords["import"]      = SoyaParser::token::IMPORT;
    sKeywords["int"]         = SoyaParser::token::INT;
    sKeywords["long"]        = SoyaParser::token::LONG;
    sKeywords["module"]      = SoyaParser::token::MODULE;
    sKeywords["new"]         = SoyaParser::token::NEW;
    sKeywords["override"]    = SoyaParser::token::OVERRIDE;
    sKeywords["private"]     = SoyaParser::token::PRIVATE;
    sKeywords["property"]    = SoyaParser::token::PROPERTY;
    sKeywords["protected"]   = SoyaParser::token::PROTECTED;
    sKeywords["public"]      = SoyaParser::token::PUBLIC;
    sKeywords["readonly"]    = SoyaParser::token::READONLY;
    sKeywords["return"]      = SoyaParser::token::RETURN;
    sKeywords["set"]         = SoyaParser::token::SET;
    sKeywords["static"]      = SoyaParser::token::STATIC;
    sKeywords["struct"]      = SoyaParser::token::STRUCT;
    sKeywords["super"]       = SoyaParser::token::SUPER;
    sKeywords["switch"]      = SoyaParser::token::SWITCH;
    sKeywords["this"]        = SoyaParser::token::THIS;
    sKeywords["true"]        = SoyaParser::token::TRUE_;
    sKeywords["uint"]        = SoyaParser::token::UINT;
    sKeywords["ulong"]       = SoyaParser::token::ULONG;
    sKeywords["var"]         = SoyaParser::token::VAR;
    sKeywords["virtual"]     = SoyaParser::token::VIRTUAL;
    sKeywords["void"]        = SoyaParser::token::VOID;
    sKeywords["while"]       = SoyaParser::token::WHILE;
    sKeywords["xor"]         = SoyaParser::token::XOR;
}


const std::string&
Lexer::fileName() const
{
    return mFileName;
}


int
Lexer::nextToken(SoyaParser::semantic_type* yylval,
                 SoyaParser::location_type* yylloc)
{
    const char* utf8_char = lookAhead();

    /*
     * End of file
     */
    if (utf8_char[0] == '\0')
        return -1;

    /*
     * Line Terminators
     */
    if (std::regex_match(utf8_char, sLineTerminator))
    {
        skipLineTerminator();
        return nextToken(yylval, yylloc);
    }

    /*
     * White Space
     */
    else if (std::regex_match(utf8_char, sWhiteSpace))
    {
        nextChar();
        return nextToken(yylval, yylloc);
    }

    /*
     * Delimited Comments
     */
    else if (utf8_char[0] == '/' && lookAhead(2)[0] == '*')
    {
        skipDelimitedComment();
        return nextToken(yylval, yylloc);
    }

    /*
     * Single-Line Comments
     */
    else if (utf8_char[0] == '/' && lookAhead(2)[0] == '/')
    {
        skipSingleLineComment();
        return nextToken(yylval, yylloc);
    }

    /*
     * Tokens
     */
    else
    {
        yylloc->begin.line = mCurrentLine;
        yylloc->begin.column = mCurrentColumn;

        int ret;

        /*
         * Identifiers / Keywords
         */
        if (std::regex_match(utf8_char, sIdentifierStartChar))
            ret = readIdentifierOrKeyword(yylval);

        /*
         * Integer / Floating-Point Literals
         */
        else if (std::isdigit(utf8_char[0]) ||
                 (utf8_char[0] == '+' && std::isdigit(lookAhead(2)[0])) ||
                 (utf8_char[0] == '-' && std::isdigit(lookAhead(2)[0])))
        {
            ret = readIntegerOrFloatingPointLiteral(yylval);
        }

        /*
         * Character Literals
         */
        else if (utf8_char[0] == '\'')
            ret = readCharacterLiteral(yylval);

        /*
         * Operators
         */
        else
        {
            ret = readPunctatorOrOperator();

            /*
             * (Unexpected Characters)
             */
            if (ret == -1)
            {
                nextChar();

                addError(
                  str(boost::format("Unexpected character '%1%'.") % mLastChar));

                ret = nextToken(yylval, yylloc);
            }
        }

        yylloc->end.line = mCurrentLine;
        yylloc->end.column = mCurrentColumn;

        return ret;
    }
}


void
Lexer::addError(const std::string& description)
{
    soyac::ast::Location location(fileName(),
                                  mCurrentLine, mCurrentColumn,
                                  mCurrentLine, mCurrentColumn);

    return mResultBuilder->addError(location, description);
}


const char*
Lexer::nextChar()
{
    /*
     * If the buffer is empty, read the next character into it so that
     * we can return it.
     */
    if (mCharBuffer.empty())
        bufferChars(1);

    /*
     * Delete the previously last consumed character, if any.
     */
    if (mLastChar)
        delete [] mLastChar;

    /*
     * The character we will return (the first character in the buffer) is
     * considered the last consumed character after this method; thus, we
     * adjust mLastChar to point to this character.
     */
    mLastChar = mCharBuffer.front();

    /*
     * Remove the consumed character from the buffer and return it.
     */
    mCharBuffer.pop_front();
    return mLastChar;
}


const char*
Lexer::lookAhead(unsigned int numChars)
{
    assert (numChars > 0);

    bufferChars(numChars);
    return mCharBuffer[numChars - 1];
}


void
Lexer::bufferChars(int numChars)
{
    for (int i = mCharBuffer.size(); i < numChars; i++)
    {
        /*
         * First, we peek the next byte. If it is a valid ASCII character
         * (that is, the most significant bit is 0), this byte is our next
         * character. Otherwise, the number of 1's in the first byte tell us
         * how long the UTF-8 character is.
         *
         * See http://en.wikipedia.org/wiki/UTF-8
         */

        unsigned char firstByte = (unsigned char) mStream->peek();

        if (mStream->eof())
        {
            mCharBuffer.push_front("");
            continue;
        }

        char* c;

        if (firstByte >> 7 == 0)
        {
            mStream->get();

            c = new char[2];
            c[0] = firstByte;
            c[1] = '\0';
        }
        else
        {
            int charLength = 2;

            for (int i = 2; i < 4; i++)
            {
                if (firstByte & (0x80 >> i))
                    charLength++;
                else
                    break;
            }

            c = new char[charLength + 1];

            mStream->read(c, charLength);
            c[charLength] = '\0';
        }

        mCharBuffer.push_back(c);
    }
}


void
Lexer::skipLineTerminator()
{
    nextChar();

    /*
     * Treat \r\n as a single line terminator.
     */
    if (mLastChar[0] == '\r' && lookAhead()[0] == '\n')
        nextChar();

    /*
     * After skipping a line terminator, we are at a new line, so
     * increment mCurrentLine and set mCurrentColumn back to 1.
     */
    mCurrentLine++;
    mCurrentColumn = 1;

}


void
Lexer::skipDelimitedComment()
{
    /*
     * Skip "/" and "*" characters.
     */
    nextChar();
    nextChar();

    /*
     * Skip each character in the stream until we find the ending '*' and
     * '/' (which we skip too before returning).
     */
    while (true)
    {
        const char* lookahead = lookAhead();

        if (std::regex_match(lookahead, sLineTerminator))
        {
            skipLineTerminator();
        }
        else if (lookahead[0] == '*' && lookAhead(2)[0] == '/')
        {
            nextChar();
            nextChar();
            break;
        }
        else if (lookahead[0] == '\0')
        {
            addError("Undelimited comment.");
            return;
        }
        else
            nextChar();
    }
}


void
Lexer::skipSingleLineComment()
{
    /*
     * Skip "//".
     */
    nextChar();
    nextChar();

    /*
     * Skip all characters until we encounter a line terminator
     * or end-of-file.
     */
    while (!std::regex_match(lookAhead(), sLineTerminator) &&
           lookAhead()[0] != '\0')
    {
        nextChar();
    }
}

int
Lexer::readIdentifierOrKeyword(SoyaParser::semantic_type* yylval)
{
    std::string* identifier = new std::string;

    while (std::regex_match(lookAhead(), sIdentifierChar))
        *identifier += nextChar();

    /*
     * If the read string is an integer type keyword, determine the
     * matching IntegerType instance and return it as semantic value.
     */
    if (std::regex_match(*identifier, sIntegerType))
    {

        if (*identifier == "int")
            yylval->type = TYPE_INT;
        else if (*identifier == "uint")
            yylval->type = TYPE_UINT;
        else
        {
            bool isSigned = ((*identifier)[0] != 'u');
            int size = atoi(identifier->c_str() + (isSigned ? 3 : 4));

            yylval->type = soyac::ast::IntegerType::get(size, isSigned);
        }

        delete identifier;
        return SoyaParser::token::INT_TYPE;
    }

    /*
     * Otherwise, look up in the keyword map if the identifier-like string we
     * read is a keyword, and return the arppropiate token ID in this case.
     * Otherwise, it is *really* an identifier, and we store it as
     * semantic value in yylval.
     */

    std::map<std::string, int>::iterator it = sKeywords.find(*identifier);

    if (it != sKeywords.end())
    {
        delete identifier;
        return it->second;
    }
    else
    {
        yylval->identifier = identifier;
        return SoyaParser::token::IDENTIFIER;
    }
}


int
Lexer::readIntegerOrFloatingPointLiteral(SoyaParser::semantic_type* yylval)
{
    bool negative = false;

    if (lookAhead()[0] == '-')
    {
        negative = true;
        nextChar();
    }
    else if (lookAhead()[0] == '+')
        nextChar();

    /*
     * If the literal starts with '0', it might be a binary or
     * hexadecimal integer literal.
     */
    if (lookAhead()[0] == '0')
    {
        if (lookAhead(2)[0] == 'b')
        {
            nextChar();
            nextChar();
            return readBinaryIntegerLiteral(yylval, negative);
        }
        else if (lookAhead(2)[0] == 'x')
        {
            nextChar();
            nextChar();
            return readHexadecimalIntegerLiteral(yylval, negative);
        }
    }

    /*
     * If the literal is neither a binary or hexadecimal integer, the
     * remaining possibilities are decimal integer, octal integer (if
     * the literal starts with '0'), and floating-point. First, we
     * check if it is a floating-point literal by looking for a '.'
     * or 'E'/'e'.
     */

    int numLookahead = 1;

    while (true)
    {
        const char* c = lookAhead(numLookahead);

        if ((c[0] == '.' || c[0] == 'E' || c[0] == 'e') &&
            isdigit(lookAhead(numLookahead + 1)[0]))
        {
            return readFloatingPointLiteral(yylval, negative);
        }
        else if (!isdigit(c[0]))
        {
            break;
        }

        numLookahead++;
    }

    /*
     * If the literal is no floating-point literal, it is interpreted as
     * an octal integer if it starts with '0'. Otherwise, it must be a
     * decimal integer.
     */
    if (lookAhead()[0] == '0')
        return readOctalIntegerLiteral(yylval, negative);
    else
        return readDecimalIntegerLiteral(yylval, negative);
}


int
Lexer::readDecimalIntegerLiteral(SoyaParser::semantic_type* yylval,
                                 bool negative)
{
    std::stringstream literal;

    if (negative)
        literal << '-';

    while (true)
    {
        const char* c = lookAhead();

        if (isdigit(c[0]))
        {
            literal << c;
            nextChar();
        }
        else
            break;
    }


    setSemanticValueFromIntegerLiteral(yylval, literal.str(), 10);
    return SoyaParser::token::INT_LITERAL;
}


int
Lexer::readBinaryIntegerLiteral(SoyaParser::semantic_type* yylval,
                                bool negative)
{
    std::stringstream literal;

    if (negative)
        literal << '-';

    while (true)
    {
        const char* c = lookAhead();

        if (c[0] == '0' || c[0] == '1')
        {
            literal << c;
            nextChar();
        }
        else
            break;
    }


    setSemanticValueFromIntegerLiteral(yylval, literal.str(), 2);
    return SoyaParser::token::INT_LITERAL;
}


int
Lexer::readOctalIntegerLiteral(SoyaParser::semantic_type* yylval,
                               bool negative)
{
    std::stringstream literal;

    if (negative)
        literal << '-';

    while (true)
    {
        const char* c = lookAhead();

        if (c[0] >= '0' && c[0] <= '7')
        {
            literal << c;
            nextChar();
        }
        else
            break;
    }

    setSemanticValueFromIntegerLiteral(yylval, literal.str(), 8);
    return SoyaParser::token::INT_LITERAL;
}


int
Lexer::readHexadecimalIntegerLiteral(SoyaParser::semantic_type* yylval,
                                     bool negative)
{
    std::stringstream literal;

    if (negative)
        literal << '-';

    while (true)
    {
        const char* c = lookAhead();

        if (isxdigit(c[0]))
        {
            literal << c;
            nextChar();
        }
        else
            break;
    }

    setSemanticValueFromIntegerLiteral(yylval, literal.str(), 16);
    return SoyaParser::token::INT_LITERAL;
}

void
Lexer::setSemanticValueFromIntegerLiteral(SoyaParser::semantic_type* yylval,
                                          const std::string& literal,
                                          int base)
{
    int64_t data;
    bool isSigned;

    data = std::strtoll(literal.c_str(), NULL, base);

    if (errno == ERANGE)
    {
        errno = 0;
        data = (int64_t) std::strtoull(literal.c_str(), NULL, base);

        if (errno == ERANGE)
        {
            addError(
              str(boost::format(
                "The integer literal '%1%' is too big for any of "
                "the supported integer types.")
                % literal));

            yylval->intLiteral = NULL;
        }
        else
            isSigned = false;
    }
    else
        isSigned = true;

    yylval->intLiteral = new IntegerValue(data, isSigned);
}


int
Lexer::readFloatingPointLiteral(SoyaParser::semantic_type* yylval,
                                bool negative)
{
    std::stringstream literal;
    bool dotRead = false;
    bool eRead = false;

    if (negative)
        literal << '-';

    while (true)
    {
        const char* c = lookAhead();

        if (isdigit(c[0]) ||
            (c[0] == '.' && (!dotRead ? (dotRead = true) : false)) ||
            ((c[0] == 'E' || c[0] == 'e') && (!eRead ? (eRead = true) : false)))
        {
            literal << c;
            nextChar();

            if ((c[0] == 'E' || c[0] == 'e') && lookAhead()[0] == '-')
            {
                literal << '-';
                nextChar();
            }
        }
        else
            break;
    }

    bool singlePrecision = false;

    if (lookAhead()[0] == 'f')
    {
        singlePrecision = true;
        nextChar();
    }

    yylval->fpLiteral =
      new FloatingPointValue(std::strtod(literal.str().c_str(), 0),
                             singlePrecision);

    if (errno == ERANGE)
    {
        addError(
          str(boost::format(
            "The floating-point literal '%1%' is too big for any "
            "of the supported integer types.")
            % literal.str()));
    }

    return SoyaParser::token::FP_LITERAL;
}


int
Lexer::readCharacterLiteral(SoyaParser::semantic_type* yylval)
{
    /*
     * Discard the laeding "'" character.
     */
    nextChar();

    const char* utf8_char;

    /*
     * If the character literal begins with a '\', it must be a character
     * escape sequence.
     */
    if (lookAhead()[0] == '\\')
        utf8_char = readEscapeChar();
    else
        utf8_char = nextChar();

    /*
     * As we store character values as 32-bit Unicode codepoint integers,
     * we need to convert the read character to an uint32_t value. Many
     * thanks to Nemanja Trifunovic, who wrote the UTF-8 library used for
     * the conversion:
     *
     * http://utfcpp.sourceforge.net/
     */
    std::vector<uint32_t> codepoints;
    utf8::utf8to16(utf8_char, utf8_char + std::strlen(utf8_char),
                   std::back_inserter(codepoints));

    if (codepoints.size() == 0)
        yylval->charLiteral = 0;
    else
        yylval->charLiteral = codepoints.front();

    /*
     * Discard the trailing "'" character.
     */
    if (lookAhead()[0] != '\'')
        addError("Undelimited character literal.");
    else
        nextChar();

    return SoyaParser::token::CHAR_LITERAL;
}


const char*
Lexer::readEscapeChar()
{
    static char ret[2] = {'\0', '\0'};

    /*
     * Discard leading '\'.
     */
    nextChar();

    const char* c = lookAhead();

    switch (c[0])
    {
        case '\'':
            ret[0] = '\'';
            break;

        case '\"':
            ret[0] = '\"';
            break;

        case '\\':
            ret[0] = '\\';
            break;

        case '0':
            ret[0] = '\0';
            break;

        case 'a':
            ret[0] = '\a';
            break;

        case 'b':
            ret[0] = '\b';
            break;

        case 'f':
            ret[0] = '\f';
            break;

        case 'n':
            ret[0] = '\n';
            break;

        case 'r':
            ret[0] = '\r';
            break;

        case 't':
            ret[0] = '\t';
            break;

        case 'v':
            ret[0] = '\v';
            break;

        default:
            ret[0] = '\0';
            addError(
              str(boost::format(
                "Invalid escape sequence '\\%1%'.")
                % c));
    }

    nextChar();
    return ret;
}



int
Lexer::readPunctatorOrOperator()
{
    const char* c = lookAhead();

    switch (c[0])
    {
        case '(':
            nextChar();
            return SoyaParser::token::LPAREN;

        case ')':
            nextChar();
            return SoyaParser::token::RPAREN;

        case '[':
            nextChar();
            return SoyaParser::token::LBRACK;

        case ']':
            nextChar();
            return SoyaParser::token::RBRACK;

        case '{':
            nextChar();
            return SoyaParser::token::LCURL;

        case '}':
            nextChar();
            return SoyaParser::token::RCURL;

        case '.':
            nextChar();

            if (lookAhead()[0] == '.')
            {
                nextChar();
                return SoyaParser::token::DOTDOT;
            }
            else
                return SoyaParser::token::DOT;

        case ',':
            nextChar();
            return SoyaParser::token::COMMA;

        case ':':
            nextChar();

            if (lookAhead()[0] == ':')
            {
                nextChar();
                return SoyaParser::token::DCOLON;
            }
            else
                return SoyaParser::token::COLON;

        case ';':
            nextChar();
            return SoyaParser::token::SCOLON;

        case '=':
            nextChar();
            c = lookAhead();

            if (c[0] == '>')
            {
                nextChar();
                return SoyaParser::token::ARROW;
            }
            else if (c[0] == '=')
            {
                nextChar();
                return SoyaParser::token::EQ;
            }
            else
                return SoyaParser::token::ASSIGN;

        case '+':
            nextChar();

            if (lookAhead()[0] == '=')
            {
                nextChar();
                return SoyaParser::token::PLUS_ASSIGN;
            }
            else
                return SoyaParser::token::PLUS;

        case '-':
            nextChar();

            if (lookAhead()[0] == '=')
            {
                nextChar();
                return SoyaParser::token::MINUS_ASSIGN;
            }
            else
                return SoyaParser::token::MINUS;

        case '*':
            nextChar();

            if (lookAhead()[0] == '=')
            {
                nextChar();
                return SoyaParser::token::MUL_ASSIGN;
            }
            else
                return SoyaParser::token::MUL;

        case '/':
            nextChar();

            if (lookAhead()[0] == '=')
            {
                nextChar();
                return SoyaParser::token::DIV_ASSIGN;
            }
            else
                return SoyaParser::token::DIV;

        case '%':
            nextChar();

            if (lookAhead()[0] == '=')
            {
                nextChar();
                return SoyaParser::token::MOD_ASSIGN;
            }
            else
                return SoyaParser::token::MOD;

        case '&':
            nextChar();

            if (lookAhead()[0] == '&')
            {
                nextChar();
                return SoyaParser::token::AND;
            }
            else
                return SoyaParser::token::BAND;

        case '|':
            nextChar();

            if (lookAhead()[0] == '|')
            {
                nextChar();
                return SoyaParser::token::OR;
            }
            else
                return SoyaParser::token::BOR;

        case '^':
            nextChar();
            return SoyaParser::token::BXOR;

        case '<':
            nextChar();
            c = lookAhead();

            if (c[0] == '<')
            {
                nextChar();
                c = lookAhead();

                if (c[0] == '=')
                {
                    nextChar();
                    return SoyaParser::token::LSHIFT_ASSIGN;
                }
                else
                    return SoyaParser::token::LSHIFT;
            }
            else if (c[0] == '=')
            {
                nextChar();
                return SoyaParser::token::LE;
            }
            else
                return SoyaParser::token::LT;

        case '>':
            nextChar();
            c = lookAhead();

            if (c[0] == '>')
            {
                nextChar();
                c = lookAhead();

                if (c[0] == '=')
                {
                    nextChar();
                    return SoyaParser::token::RSHIFT_ASSIGN;
                }
                else
                    return SoyaParser::token::RSHIFT;
            }
            else if (c[0] == '=')
            {
                nextChar();
                return SoyaParser::token::GE;
            }
            else
                return SoyaParser::token::GT;

        case '!':
            nextChar();

            if (lookAhead()[0] == '=')
            {
                nextChar();
                return SoyaParser::token::NE;
            }
            else
                return SoyaParser::token::NOT;

        default:
            return -1;
    }
}


int
yylex(SoyaParser::semantic_type* yylval, SoyaParser::location_type* yylloc,
      ParserDriver* driver)
{
    return driver->lexer()->nextToken(yylval, yylloc);
}


}}
