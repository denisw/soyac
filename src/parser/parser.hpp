
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison LALR(1) parsers in C++
   
      Copyright (C) 2002, 2003, 2004, 2005, 2006, 2007, 2008 Free Software
   Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C++ LALR(1) parser skeleton written by Akim Demaille.  */

#ifndef PARSER_HEADER_H
# define PARSER_HEADER_H

/* "%code requires" blocks.  */

/* Line 35 of lalr1.cc  */
#line 19 "/home/denis/src/soyac-svn/trunk/src/parser/soya.y"

#include <iostream>
#include <sstream>
#include <stdint.h>

#include <ast/ast.hpp>

using namespace soyac::ast;

namespace soyac {
namespace parser
{
class ParserDriver;
}}



/* Line 35 of lalr1.cc  */
#line 63 "/home/denis/src/soyac-svn/trunk/src/parser/parser.hpp"


#include <string>
#include <iostream>
#include "stack.hh"


/* Line 35 of lalr1.cc  */
#line 1 "[Bison:b4_percent_define_default]"

namespace yy {

/* Line 35 of lalr1.cc  */
#line 77 "/home/denis/src/soyac-svn/trunk/src/parser/parser.hpp"
  class position;
  class location;

/* Line 35 of lalr1.cc  */
#line 1 "[Bison:b4_percent_define_default]"

} // yy

/* Line 35 of lalr1.cc  */
#line 87 "/home/denis/src/soyac-svn/trunk/src/parser/parser.hpp"

#include "location.hh"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)		\
do {							\
  if (N)						\
    {							\
      (Current).begin = (Rhs)[1].begin;			\
      (Current).end   = (Rhs)[N].end;			\
    }							\
  else							\
    {							\
      (Current).begin = (Current).end = (Rhs)[0].end;	\
    }							\
} while (false)
#endif


/* Line 35 of lalr1.cc  */
#line 1 "[Bison:b4_percent_define_default]"

namespace yy {

/* Line 35 of lalr1.cc  */
#line 135 "/home/denis/src/soyac-svn/trunk/src/parser/parser.hpp"

  /// A Bison parser.
  class SoyaParser
  {
  public:
    /// Symbol semantic values.
#ifndef YYSTYPE
    union semantic_type
    {

/* Line 35 of lalr1.cc  */
#line 39 "/home/denis/src/soyac-svn/trunk/src/parser/soya.y"

    Node* node;

    Name* name;
    std::string* identifier;
    std::stringstream* modulename;
    std::list<std::string>* identifierlist;

    Block* block;
    DeclarationBlock* dblock;
    DeclarationStatement* dstmt;
    DeclaredEntity* decl;
    DeclaredEntity::Modifier mod;
    ConstructorInitializer* cinit;
    Expression* expr;
    Function* acc;
    Function** accs;
    FunctionParameter* param;
    Import* imp;
    Statement* stmt;
    UnresolvedBinaryExpression::Kind binop;
    Type* type;

    std::list<DeclarationStatement*>* dstmtlist;
    std::list<DeclaredEntity::Modifier>* modlist;
    std::list<EnumConstant*>* econstlist;
    std::list<Expression*>* exprlist;
    std::list<FunctionParameter*>* paramlist;
    std::list<Import*>* implist;
    std::list<Statement*>* stmtlist;
    std::list<Type*>* typelist;

    IntegerValue* intLiteral;
    FloatingPointValue* fpLiteral;
    uint32_t charLiteral;

    bool boolVal;



/* Line 35 of lalr1.cc  */
#line 189 "/home/denis/src/soyac-svn/trunk/src/parser/parser.hpp"
    };
#else
    typedef YYSTYPE semantic_type;
#endif
    /// Symbol locations.
    typedef location location_type;
    /// Tokens.
    struct token
    {
      /* Tokens.  */
   enum yytokentype {
     IDENTIFIER = 258,
     ALIAS = 259,
     AS = 260,
     BOOL = 261,
     BREAK = 262,
     CASE = 263,
     CHAR = 264,
     CLASS = 265,
     CONST = 266,
     CONSTRUCTOR = 267,
     CONTINUE = 268,
     DEFAULT = 269,
     DO = 270,
     DOUBLE = 271,
     ELSE = 272,
     ENUM = 273,
     EXTENDS = 274,
     EXTERN = 275,
     FALSE_ = 276,
     FLOAT = 277,
     FOR = 278,
     FROM = 279,
     FUNCTION = 280,
     GET = 281,
     IF = 282,
     IS = 283,
     IMPLEMENTS = 284,
     IMPORT = 285,
     INT = 286,
     LONG = 287,
     MODULE = 288,
     NEW = 289,
     NULL_ = 290,
     OVERRIDE = 291,
     PRIVATE = 292,
     PROPERTY = 293,
     PROTECTED = 294,
     PUBLIC = 295,
     READONLY = 296,
     RETURN = 297,
     SET = 298,
     STATIC = 299,
     STRUCT = 300,
     SUPER = 301,
     SWITCH = 302,
     TRUE_ = 303,
     THIS = 304,
     UINT = 305,
     ULONG = 306,
     VAR = 307,
     VIRTUAL = 308,
     VOID = 309,
     WHILE = 310,
     XOR = 311,
     INT_LITERAL = 312,
     FP_LITERAL = 313,
     CHAR_LITERAL = 314,
     LPAREN = 315,
     RPAREN = 316,
     LBRACK = 317,
     RBRACK = 318,
     LCURL = 319,
     RCURL = 320,
     DOT = 321,
     DOTDOT = 322,
     COMMA = 323,
     COLON = 324,
     DCOLON = 325,
     SCOLON = 326,
     ARROW = 327,
     PLUS = 328,
     MINUS = 329,
     MUL = 330,
     DIV = 331,
     MOD = 332,
     BAND = 333,
     BOR = 334,
     BXOR = 335,
     BNOT = 336,
     LSHIFT = 337,
     RSHIFT = 338,
     ASSIGN = 339,
     PLUS_ASSIGN = 340,
     MINUS_ASSIGN = 341,
     MUL_ASSIGN = 342,
     DIV_ASSIGN = 343,
     MOD_ASSIGN = 344,
     BAND_ASSIGN = 345,
     BOR_ASSIGN = 346,
     BXOR_ASSIGN = 347,
     BNOT_ASSIGN = 348,
     LSHIFT_ASSIGN = 349,
     RSHIFT_ASSIGN = 350,
     EQ = 351,
     NE = 352,
     LT = 353,
     GT = 354,
     LE = 355,
     GE = 356,
     NOT = 357,
     AND = 358,
     OR = 359,
     INT_TYPE = 360
   };

    };
    /// Token type.
    typedef token::yytokentype token_type;

    /// Build a parser object.
    SoyaParser (soyac::parser::ParserDriver* driver_yyarg);
    virtual ~SoyaParser ();

    /// Parse.
    /// \returns  0 iff parsing succeeded.
    virtual int parse ();

#if YYDEBUG
    /// The current debugging stream.
    std::ostream& debug_stream () const;
    /// Set the current debugging stream.
    void set_debug_stream (std::ostream &);

    /// Type for debugging levels.
    typedef int debug_level_type;
    /// The current debugging level.
    debug_level_type debug_level () const;
    /// Set the current debugging level.
    void set_debug_level (debug_level_type l);
#endif

  private:
    /// Report a syntax error.
    /// \param loc    where the syntax error is found.
    /// \param msg    a description of the syntax error.
    virtual void error (const location_type& loc, const std::string& msg);

    /// Generate an error message.
    /// \param state   the state where the error occurred.
    /// \param tok     the lookahead token.
    virtual std::string yysyntax_error_ (int yystate);

#if YYDEBUG
    /// \brief Report a symbol value on the debug stream.
    /// \param yytype       The token type.
    /// \param yyvaluep     Its semantic value.
    /// \param yylocationp  Its location.
    virtual void yy_symbol_value_print_ (int yytype,
					 const semantic_type* yyvaluep,
					 const location_type* yylocationp);
    /// \brief Report a symbol on the debug stream.
    /// \param yytype       The token type.
    /// \param yyvaluep     Its semantic value.
    /// \param yylocationp  Its location.
    virtual void yy_symbol_print_ (int yytype,
				   const semantic_type* yyvaluep,
				   const location_type* yylocationp);
#endif


    /// State numbers.
    typedef int state_type;
    /// State stack type.
    typedef stack<state_type>    state_stack_type;
    /// Semantic value stack type.
    typedef stack<semantic_type> semantic_stack_type;
    /// location stack type.
    typedef stack<location_type> location_stack_type;

    /// The state stack.
    state_stack_type yystate_stack_;
    /// The semantic value stack.
    semantic_stack_type yysemantic_stack_;
    /// The location stack.
    location_stack_type yylocation_stack_;

    /// Internal symbol numbers.
    typedef unsigned char token_number_type;
    /* Tables.  */
    /// For a state, the index in \a yytable_ of its portion.
    static const short int yypact_[];
    static const short int yypact_ninf_;

    /// For a state, default rule to reduce.
    /// Unless\a  yytable_ specifies something else to do.
    /// Zero means the default is an error.
    static const unsigned char yydefact_[];

    static const short int yypgoto_[];
    static const short int yydefgoto_[];

    /// What to do in a state.
    /// \a yytable_[yypact_[s]]: what to do in state \a s.
    /// - if positive, shift that token.
    /// - if negative, reduce the rule which number is the opposite.
    /// - if zero, do what YYDEFACT says.
    static const short int yytable_[];
    static const signed char yytable_ninf_;

    static const short int yycheck_[];

    /// For a state, its accessing symbol.
    static const unsigned char yystos_[];

    /// For a rule, its LHS.
    static const unsigned char yyr1_[];
    /// For a rule, its RHS length.
    static const unsigned char yyr2_[];

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
    /// For a symbol, its name in clear.
    static const char* const yytname_[];
#endif

#if YYERROR_VERBOSE
    /// Convert the symbol name \a n to a form suitable for a diagnostic.
    virtual std::string yytnamerr_ (const char *n);
#endif

#if YYDEBUG
    /// A type to store symbol numbers and -1.
    typedef short int rhs_number_type;
    /// A `-1'-separated list of the rules' RHS.
    static const rhs_number_type yyrhs_[];
    /// For each rule, the index of the first RHS symbol in \a yyrhs_.
    static const unsigned short int yyprhs_[];
    /// For each rule, its source line number.
    static const unsigned short int yyrline_[];
    /// For each scanner token number, its symbol number.
    static const unsigned short int yytoken_number_[];
    /// Report on the debug stream that the rule \a r is going to be reduced.
    virtual void yy_reduce_print_ (int r);
    /// Print the state stack on the debug stream.
    virtual void yystack_print_ ();

    /* Debugging.  */
    int yydebug_;
    std::ostream* yycdebug_;
#endif

    /// Convert a scanner token number \a t to a symbol number.
    token_number_type yytranslate_ (int t);

    /// \brief Reclaim the memory associated to a symbol.
    /// \param yymsg        Why this token is reclaimed.
    /// \param yytype       The symbol type.
    /// \param yyvaluep     Its semantic value.
    /// \param yylocationp  Its location.
    inline void yydestruct_ (const char* yymsg,
			     int yytype,
			     semantic_type* yyvaluep,
			     location_type* yylocationp);

    /// Pop \a n symbols the three stacks.
    inline void yypop_ (unsigned int n = 1);

    /* Constants.  */
    static const int yyeof_;
    /* LAST_ -- Last index in TABLE_.  */
    static const int yylast_;
    static const int yynnts_;
    static const int yyempty_;
    static const int yyfinal_;
    static const int yyterror_;
    static const int yyerrcode_;
    static const int yyntokens_;
    static const unsigned int yyuser_token_number_max_;
    static const token_number_type yyundef_token_;

    /* User arguments.  */
    soyac::parser::ParserDriver* driver;
  };

/* Line 35 of lalr1.cc  */
#line 1 "[Bison:b4_percent_define_default]"

} // yy

/* Line 35 of lalr1.cc  */
#line 480 "/home/denis/src/soyac-svn/trunk/src/parser/parser.hpp"



#endif /* ! defined PARSER_HEADER_H */
