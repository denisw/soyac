/*
 * soyac - The Soya programming language compiler
 * Copyright (c) 2008 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

//// Options ///////////////////////////////////////////////////////////////////

%skeleton "lalr1.cc"
%require "2.3"
%defines
%define api.parser.class {SoyaParser}

%locations
%debug

%code requires {
#include <iostream>
#include <sstream>
#include <stdint.h>

#include <ast/ast.h>

using namespace soyac::ast;

namespace soyac {
namespace parser
{
class ParserDriver;
}}
}

%parse-param { soyac::parser::ParserDriver* driver }
%lex-param   { soyac::parser::ParserDriver* driver }

%union
{
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
};

%{
#include <cassert>
#include <list>
#include <boost/format.hpp>
#include <parser/ParserDriver.h>

using namespace soyac::parser;

std::string modifier_to_string(DeclaredEntity::Modifier mod);

void set_location(Node* node,
                  const yy::SoyaParser::location_type& begin,
                  const yy::SoyaParser::location_type& end);
%}

%initial-action
{
  @$.begin.filename = @$.end.filename =
    const_cast<std::string*>(&driver->lexer()->fileName());
};

//// Tokens ////////////////////////////////////////////////////////////////////

// Identifiers
%token <identifier> IDENTIFIER
%destructor { delete $$; } IDENTIFIER

// Keywords
%token ALIAS        "alias"
%token AS           "as"
%token BOOL         "bool"
%token BREAK        "break"
%token CASE         "case"
%token CHAR         "char"
%token CLASS        "class"
%token CONST        "const"
%token CONSTRUCTOR  "constructor"
%token CONTINUE     "continue"
%token DEFAULT      "default"
%token DO           "do"
%token DOUBLE       "double"
%token ELSE         "else"
%token ENUM         "enum"
%token EXTENDS      "extends"
%token EXTERN       "extern"
%token FALSE_       "false"
%token FLOAT        "float"
%token FOR          "for"
%token FROM         "from"
%token FUNCTION     "function"
%token GET          "get"
%token IF           "if"
%token IS           "is"
%token IMPLEMENTS   "implements"
%token IMPORT       "import"
%token INT          "int"
%token LONG         "long"
%token MODULE       "module"
%token NEW          "new"
%token nullptr_        "null"
%token OVERRIDE     "override"
%token PRIVATE      "private"
%token PROPERTY     "property"
%token PROTECTED    "protected"
%token PUBLIC       "public"
%token READONLY     "readonly"
%token RETURN       "return"
%token SET          "set"
%token STATIC       "static"
%token STRUCT       "struct"
%token SUPER        "super"
%token SWITCH       "switch"
%token TRUE_        "true"
%token THIS         "this"
%token UINT         "uint"
%token ULONG        "ulong"
%token VAR          "var"
%token VIRTUAL      "virtual"
%token VOID         "void"
%token WHILE        "while"
%token XOR          "xor"

// Literals
%token <intLiteral>   INT_LITERAL
%token <fpLiteral>    FP_LITERAL
%token <charLiteral>  CHAR_LITERAL

// Punctators
%token LPAREN  "("
%token RPAREN  ")"
%token LBRACK  "["
%token RBRACK  "]"
%token LCURL   "{"
%token RCURL   "}"
%token DOT     "."
%token DOTDOT  ".."
%token COMMA   ","
%token COLON   ":"
%token DCOLON  "::"
%token SCOLON  ";"
%token ARROW   "=>"

// Operators
%token PLUS           "+"
%token MINUS          "-"
%token MUL            "*"
%token DIV            "/"
%token MOD            "%"
%token BAND           "&"
%token BOR            "|"
%token BXOR           "^"
%token BNOT           "~"
%token LSHIFT         "<<"
%token RSHIFT         ">>"
%token ASSIGN         "="
%token PLUS_ASSIGN    "+="
%token MINUS_ASSIGN   "-="
%token MUL_ASSIGN     "*="
%token DIV_ASSIGN     "/="
%token MOD_ASSIGN     "%="
%token BAND_ASSIGN    "&="
%token BOR_ASSIGN     "|="
%token BXOR_ASSIGN    "^="
%token BNOT_ASSIGN    "~="
%token LSHIFT_ASSIGN  "<<="
%token RSHIFT_ASSIGN  ">>="
%token EQ             "=="
%token NE             "!="
%token LT             "<"
%token GT             ">"
%token LE             "<="
%token GE             ">="
%token NOT            "!"
%token AND            "&&"
%token OR             "||"

// Integer Types
%token <type> INT_TYPE

///// Nonterminal Types ////////////////////////////////////////////////////////

%type <acc>
    get_accessor
    get_accessor_opt
    set_accessor
    set_accessor_opt
    ;

%type <accs>
    property_accessors
    ;

%type <binop>
    additive_operator
    bit_shift_operator
    compound_assignment_operator
    equality_operator
    relational_operator
    multiplicative_operator
    ;

%type <block>
    constructor_body_opt
    block
    function_body_opt
    ;

%type <boolVal>
    static_opt
    ;

%type <cinit>
    constructor_initializer_opt
    ;

%type <dblock>
    declaration_block
    ;

%type <decl>
    class_declaration
    constructor_declaration
    declaration
    enum_declaration
    function_declaration
    property_declaration
    variable_declaration
    variable_or_constant_declarator
    ;

%type <dstmt>
    declaration_statement
    declaration_block_statement
    ;

%type <dstmtlist>
    declaration_block_statements
    declaration_block_statements_opt
    ;

%type <econstlist>
    enum_constants
    ;

%type <expr>
    additive_expression
    assignment_expression
    bit_shift_expression
    cast_expression
    equality_expression
    expression
    for_condition_opt
    relational_expression
    initializer_opt
    logical_and_expression
    logical_or_expression
    multiplicative_expression
    primary_expression
    return_value_opt
    unary_expression
    ;

%type <exprlist>
    array_elements_opt
    function_arguments
    function_arguments_opt
    ;

%type <identifierlist>
    qualified_name_identifiers
    ;

%type <imp>
    import_statement
    ;

%type <implist>
    import_statements
    import_statements_opt
    ;

%type <intLiteral>
    enum_value_opt
    ;

%type <mod>
    modifier
    ;

%type <modlist>
    modifiers_opt
    modifiers
    ;

%type <modulename>
    package_module_name
    ;

%type <name>
    module_declaration_opt
    module_name
    name
    qualified_name
    simple_name
    ;

%type <param>
    function_parameter
    ;

%type <paramlist>
    function_parameters
    function_parameters_opt
    ;

%type <stmt>
    do_statement
    else_statement_opt
    expression_statement
    for_statement
    if_statement
    return_statement
    statement
    while_statement
    ;

%type <stmtlist>
    expression_statement_list
    for_initializer_opt
    for_iterator_opt
    statements
    statements_opt
    variable_declaration_list
    ;

%type <type>
    array_type
    extends_clause_opt
    function_type
    function_type_return_type_opt
    return_type_opt
    simple_type
    struct_declaration
    type
    underlying_type_opt
    variable_type_opt
    ;

%type <typelist>
    parameter_types
    parameter_types_opt
    ;

%%
///// Rules ////////////////////////////////////////////////////////////////////

%start module;

/**** Names () ****************************************************************/

name
    : simple_name
    {
        $$ = $1;
    }
    | package_module_name
    {
        $$ = new Name($1->str());
        delete $1;
    }
    | qualified_name
    {
        $$ = $1;
    }
    ;

simple_name
    : IDENTIFIER
    {
        $$ = new Name(*$1);
        @$ = @1;
        delete $1;
    }
    ;

qualified_name
    : qualified_name_identifiers
    {
        $$ = new Name($1->begin(), $1->end());
        delete $1;
    }
    ;

qualified_name_identifiers
    : module_name "." IDENTIFIER
    {
        $$ = new std::list<std::string>;
        $$->push_back($1->str());
        $$->push_back(*$3);
        delete $1;
        delete $3;
    }
    | qualified_name_identifiers "." IDENTIFIER
    {
        $1->push_back(*$3);
        delete $3;
        $$ = $1;
    }
    ;

module_name
    : simple_name
    {
        $$ = $1;
    }
    | package_module_name
    {
        $$ = new Name($1->str());
        delete $1;
    }
    ;

package_module_name
    : IDENTIFIER "::" IDENTIFIER
    {
        $$ = new std::stringstream;
        *$$ << *$1 << "::" << *$3;
        delete $1;
        delete $3;
    }
    | package_module_name "::" IDENTIFIER
    {
        *$1 << "::" << *$3;
        delete $1;
        $$ = $1;
    }
    ;

/**** Types () ****************************************************************/

type
    : simple_type    { $$ = $1; }
    | array_type     { $$ = $1; }
    | function_type  { $$ = $1; }
    | "(" type ")"   { $$ = $2; }
    | name
    {
        $$ = new UnknownType(*$1);
        set_location($$, @1, @1);
        delete $1;
    }
    ;

array_type
    : type "[" "]"
    {
        if (dynamic_cast<UnknownType*>($1) != nullptr)
            $$ = new UnknownArrayType((UnknownType*) $1);
        else
            $$ = ArrayType::get($1);

        set_location($$, @1, @3);
    }
    ;

function_type
    : "function" "(" parameter_types_opt ")" function_type_return_type_opt
    {
        $$ = new UnknownFunctionType($5, $3->begin(), $3->end());
        set_location($$, @1, @5);
        delete $3;
    }
    ;

parameter_types_opt
    :                  { $$ = new std::list<Type*>; }
    | parameter_types  { $$ = $1; }
    ;

parameter_types
    : type
    {
        $$ = new std::list<Type*>;
        $$->push_back($1);
    }
    | parameter_types "," type
    {
        $1->push_back($3);
        $$ = $1;
    }
    ;

function_type_return_type_opt
    :            { $$ = nullptr; }
    | "=>" type  { $$ = $2; }
    ;

simple_type
    : INT_TYPE      { $$ = $1; }
    | "bool"        { $$ = TYPE_BOOL; }
    | "char"        { $$ = TYPE_CHAR; }
    | "double"      { $$ = TYPE_DOUBLE; }
    | "float"       { $$ = TYPE_FLOAT; }
    | "long"        { $$ = TYPE_LONG; }
    | "ulong"       { $$ = TYPE_ULONG; }
    | "void"        { $$ = TYPE_VOID; }
    ;

/**** Modules () **************************************************************/

module
    : module_declaration_opt import_statements_opt statements_opt
    {
        Module* m;

        if ($1 != nullptr)
        {
            m = Module::get(*$1);

            /*
             * If the Module to parse already exists, set the returned Module
             * to that module.
             */
            if (m != nullptr)
            {
                driver->setSyntaxTree(m);
                m = nullptr;
            }
            else
                m = Module::get(*$1, true);

            delete $1;
        }
        else
        {
            m = Module::getProgram();

            if (m != nullptr)
            {
                driver->setSyntaxTree(m);
                m = nullptr;
            }
            else
                m = Module::getProgram(true);
        }

        if (m != nullptr)
        {
            if ($2 != nullptr)
            {
                for (std::list<Import*>::iterator it = $2->begin();
                     it != $2->end(); it++)
                {
                    m->addImport(*it);
                }

                delete $2;
            }

            if ($3 != nullptr)
            {
                for (std::list<Statement*>::iterator it = $3->begin();
                     it != $3->end(); it++)
                {
                    m->body()->addStatement(*it);
                }

                delete $3;
            }

            driver->setSyntaxTree(m);
        }
    }
    ;

module_declaration_opt
    :                           { $$ = nullptr; }
    | "module" module_name ";"  { $$ = $2; }
    ;

/***** Import Statements () ***************************************************/

import_statements_opt
    :                    { $$ = nullptr; }
    | import_statements  { $$ = $1; }
    ;

import_statements
    : import_statement
    {
        $$ = new std::list<Import*>;
        $$->push_back($1);
    }
    | import_statements import_statement
    {
        $1->push_back($2);
        $$ = $1;
    }
    ;

import_statement
    : static_opt "import" name ";"
    {
        if ($3->isSimple())
            $$ = new UnresolvedModuleImport(*$3, $1);
        else
            $$ = new UnresolvedEntityImport(*$3);

        set_location($$, @1, @4);
        delete $3;
    }
    ;

static_opt
    :           { $$ = true; }
    | "static"  { $$ = false; }
    ;

/***** Statements () **********************************************************/

statement
    : block                  { $$ = $1; }
    | declaration_statement  { $$ = $1; }
    | do_statement           { $$ = $1; }
    | expression_statement   { $$ = $1; }
    | for_statement          { $$ = $1; }
    | if_statement           { $$ = $1; }
    | return_statement       { $$ = $1; }
    | while_statement        { $$ = $1; }
    ;

block
    : "{" statements_opt "}"
    {
        if ($2 != nullptr)
        {
            $$ = new Block($2->begin(), $2->end());
            delete $2;
        }
        else
            $$ = new Block;

        set_location($$, @1, @3);
    }
    ;

statements_opt
    : statements  { $$ = $1; }
    |             { $$ = nullptr; }
    ;

statements
    : statement
    {
        $$ = new std::list<Statement*>;
        $$->push_back($1);
    }
    | statements statement
    {
        $1->push_back($2);
        $$ = $1;
    }
    ;

declaration_statement
    : modifiers_opt declaration
    {
        $$ = new DeclarationStatement($2);
        set_location($$, @1, @2);

        if ($1 != nullptr)
        {
            for (std::list<DeclaredEntity::Modifier>::iterator it = $1->begin();
                 it != $1->end(); it++)
            {
                if ($2->addModifier(*it) == false)
                {
                    driver->resultBuilder()->addError(
                      $$,
                      boost::format(
                        "'%1%' has more than one \"%2%\" modifier.")
                         % $2
                         % modifier_to_string(*it));
                }
            }

            delete $1;
        }
    }
    ;

modifiers_opt
    : modifiers  { $$ = $1; }
    |            { $$ = nullptr; }
    ;

modifiers
    : modifier
    {
        $$ = new std::list<DeclaredEntity::Modifier>;
        $$->push_back($1);
    }
    | modifiers modifier
    {
        $1->push_back($2);
        $$ = $1;
    }
    ;

modifier
    : "extern"   { $$ = DeclaredEntity::EXTERN; }
    | "private"  { $$ = DeclaredEntity::PRIVATE; }
    | "public"   { $$ = DeclaredEntity::PUBLIC; }
    ;

do_statement
    : "do" statement "while" "(" expression ")" ";"
    {
        $$ = new DoStatement($5, $2);
        set_location($$, @1, @6);
    }
    ;

expression_statement
    : expression ";"
    {
        $$ = new ExpressionStatement($1);
        set_location($$, @1, @2);
    }
    ;

if_statement
    : "if" "(" expression ")" statement else_statement_opt
    {
        $$ = new IfStatement($3, $5, $6);
        set_location($$, @1, @6);
    }
    ;

else_statement_opt
    :                   { $$ = nullptr; }
    | "else" statement  { $$ = $2; }
    ;

for_statement
    : "for" "(" for_initializer_opt for_condition_opt for_iterator_opt ")"
      statement
    {
        $$ = new ForStatement(
          $4,
          $3->begin(), $3->end(),
          $5->begin(), $5->end(),
          $7);

        set_location($$, @1, @7);
        delete $3;
        delete $5;
    }
    ;

for_initializer_opt
    : ";"                                  { $$ = new std::list<Statement*>;}
    | "var" variable_declaration_list ";"  { $$ = $2; }
    | expression_statement_list ";"        { $$ = $1; }
    ;

variable_declaration_list
    : variable_or_constant_declarator
    {
        $$ = new std::list<Statement*>;

        Statement* stmt = new DeclarationStatement($1);
        set_location(stmt, @1, @1);
        $$->push_back(stmt);
    }
    | variable_declaration_list "," variable_or_constant_declarator
    {
        Statement* stmt = new DeclarationStatement($3);
        set_location(stmt, @2, @2);
        $$->push_back(stmt);
    }
    ;

expression_statement_list
    : expression
    {
        $$ = new std::list<Statement*>;

        Statement* stmt = new ExpressionStatement($1);
        set_location(stmt, @1, @1);
        $$->push_back(stmt);
    }
    | expression_statement_list "," expression
    {
        Statement* stmt = new ExpressionStatement($3);
        set_location(stmt, @2, @2);
        $$->push_back(stmt);
    }
    ;

for_condition_opt
    : ";"             { $$ = nullptr; }
    | expression ";"  { $$ = $1; }
    ;

for_iterator_opt
    :                            { $$ = new std::list<Statement*>; }
    | expression_statement_list  { $$ = $1; }
    ;

return_statement
    : "return" return_value_opt ";"
    {
        $$ = new ReturnStatement($2);
        set_location($$, @1, @3);
    }
    ;

return_value_opt
    :             { $$ = nullptr; }
    | expression  { $$ = $1; }
    ;

while_statement
    : "while" "(" expression ")" statement
    {
        $$ = new WhileStatement($3, $5);
        set_location($$, @1, @5);
    }
    ;

/**** Declared Entities () ****************************************************/

declaration
    : variable_declaration  { $$ = $1; }
    | function_declaration  { $$ = $1; }
    | struct_declaration    { $$ = $1; }
    | class_declaration    { $$ = $1; }
    | enum_declaration      { $$ = $1; }
    ;

variable_declaration
    : "var" variable_or_constant_declarator ";"
    {
        $$ = $2;
        set_location($$, @1, @3);
    }
    ;

variable_or_constant_declarator
    : simple_name variable_type_opt initializer_opt
    {
        $$ = new Variable(*$1, $2, $3);
        set_location($$, @1, @3);
        delete $1;
    }
    ;

variable_type_opt
    : ":" type  { $$ = $2; }
    |           { $$ = TYPE_UNKNOWN; }
    ;

initializer_opt
    : "=" expression  { $$ = $2; }
    |                 { $$ = nullptr; }
    ;

function_declaration
    : "function" simple_name "(" function_parameters_opt ")" return_type_opt
      function_body_opt
    {
        $$ = new Function(*$2, $6, $4->begin(), $4->end(), $7);
        set_location($$, @1, @7);

        delete $2;
        delete $4;
    }
    ;

function_parameters_opt
    : function_parameters  { $$ = $1; }
    |                      { $$ = new std::list<FunctionParameter*>; }
    ;

function_parameters
    : function_parameter
    {
        $$ = new std::list<FunctionParameter*>;
        $$->push_back($1);
    }
    | function_parameters "," function_parameter
    {
        $1->push_back($3);
        $$ = $1;
    }
    ;

function_parameter
    : simple_name ":" type
    {
        $$ = new FunctionParameter(*$1, $3);
        set_location($$, @1, @3);
        delete $1;
    }
    ;

return_type_opt
    : ":" type  { $$ = $2; }
    |           { $$ = TYPE_VOID; }
    ;

function_body_opt
    : block  { $$ = $1; }
    | ";"    { $$ = nullptr; }
    ;

struct_declaration
    : "struct" simple_name declaration_block
    {
        $$ = new StructType(*$2, $3);
        set_location($$, @1, @3);
        delete $2;
    }
    ;

declaration_block
    : "{" declaration_block_statements_opt "}"
    {
        if ($2 != nullptr)
        {
            $$ = new DeclarationBlock($2->begin(), $2->end());
            delete $2;
        }
        else
            $$ = new DeclarationBlock;

        set_location($$, @1, @3);
    }
    ;

declaration_block_statements_opt
    : declaration_block_statements  { $$ = $1; }
    |                               { $$ = nullptr; }
    ;

declaration_block_statements
    : declaration_block_statement
    {
        $$ = new std::list<DeclarationStatement*>;
        $$->push_back($1);
    }
    | declaration_block_statements declaration_block_statement
    {
        $1->push_back($2);
        $$ = $1;
    }
    ;

declaration_block_statement
    : declaration_statement  { $$ = $1; }
    /*
     * As constructor declarations are only valid inside of declaration
     * bodies (that is, the bodies of structs and classes), we refer
     * to them only here, not in the general `declaration_statement'
     * production.
     */
    | modifiers_opt constructor_declaration
    {
        $$ = new DeclarationStatement($2);
        set_location($$, @1, @2);

        if ($1 != nullptr)
        {
            for (std::list<DeclaredEntity::Modifier>::iterator it = $1->begin();
                it != $1->end();
                it++)
            {
                $2->addModifier(*it);
            }

            delete $1;
        }
    }
    /*
     * Same goes for property declarations.
     */
    | modifiers_opt property_declaration
    {
        $$ = new DeclarationStatement($2);
        set_location($$, @1, @2);

        if ($1 != nullptr)
        {
            for (std::list<DeclaredEntity::Modifier>::iterator it = $1->begin();
                it != $1->end();
                it++)
            {
                $2->addModifier(*it);
            }

            delete $1;
        }
    }
    ;

constructor_declaration
    : "constructor" "(" function_parameters_opt ")" constructor_initializer_opt
       constructor_body_opt
    {
        $$ = new Constructor($5, $3->begin(), $3->end(), $6);
        delete $3;
    }
    ;

constructor_initializer_opt
    :
    {
        $$ = new UnresolvedDefaultConstructorInitializer;
    }
    | ":" "this" "(" function_arguments_opt ")"
    {
        $$ = new UnresolvedConstructorInitializer($4->begin(), $4->end());
        set_location($$, @1, @5);
        delete $4;
    }
    | ":" "super" "(" function_arguments_opt ")"
    {
        $$ = new UnresolvedBaseConstructorInitializer($4->begin(), $4->end());
        set_location($$, @1, @5);
        delete $4;
    }
    ;

constructor_body_opt
    : ";"    { $$ = new Block; }
    | block  { $$ = $1; }
    ;

property_declaration
    : "property" name ":" type "{" property_accessors "}"
    {
        $$ = new Property(*$2, $4,
                          (PropertyGetAccessor*) $6[0],
                          (PropertySetAccessor*) $6[1]);
        set_location($$, @1, @7);
        delete $2;
        delete $6;
    }
    ;

property_accessors
    : get_accessor set_accessor_opt
    {
        $$ = new Function*[2];
        $$[0] = $1;
        $$[1] = $2;
    }
    | set_accessor get_accessor_opt
    {
        $$ = new Function*[2];
        $$[0] = $2;
        $$[1] = $1;
    }
    ;

get_accessor_opt
    :               { $$ = nullptr; }
    | get_accessor  { $$ = $1; }
    ;

get_accessor
    : "get" function_body_opt
    {
        $$ = new PropertyGetAccessor($2);
        set_location($$, @1, @2);
    }
    ;

set_accessor_opt
    :               { $$ = nullptr; }
    | set_accessor  { $$ = $1; }
    ;

set_accessor
    : "set" function_body_opt
    {
        $$ = new PropertySetAccessor($2);
        set_location($$, @1, @2);
    }
    ;

class_declaration
    : "class" simple_name extends_clause_opt declaration_block
    {
        $$ = new ClassType(*$2, $3, $4);
        set_location($$, @1, @3);
        delete $2;
    }
    ;

extends_clause_opt
    :
    {
        $$ = new UnknownType(Name("__core__") + Name("Object"));
        set_location($$, @$, @$);
    }
    | "extends" name
    {
        $$ = new UnknownType(*$2);
        set_location($$, @1, @2);
        delete $2;
    }
    ;

enum_declaration
    : "enum" simple_name underlying_type_opt "{" enum_constants "}"
    {
        $$ = new EnumType(*$2, (IntegerType*) $3, $5->begin(), $5->end());
        set_location($$, @1, @6);
        delete $2;
        delete $5;
    }
    ;

underlying_type_opt
    :               { $$ = TYPE_INT; }
    | ":" INT_TYPE  { $$ = $2; }
    | ":" "long"    { $$ = TYPE_LONG; }
    | ":" "ulong"   { $$ = TYPE_ULONG; }
    ;

enum_constants
    : simple_name enum_value_opt
    {
        EnumConstant* c;

        if ($2 != nullptr)
        {
            c = new EnumConstant(*$1, *$2);
            delete $2;
        }
        else
            c = new EnumConstant(*$1, IntegerValue(0, false));

        set_location(c, @1, @2);
        delete $1;

        $$ = new std::list<EnumConstant*>;
        $$->push_back(c);
    }
    | enum_constants "," simple_name enum_value_opt
    {
        EnumConstant* c;

        if ($4 != nullptr)
        {
            c = new EnumConstant(*$3, *$4);
            delete $4;
        }
        else
        {
            const IntegerValue& lastVal = $1->back()->value();
            c = new EnumConstant(
              *$3, IntegerValue(lastVal.data() + 1, lastVal.isSigned()));
        }

        set_location(c, @1, @4);
        delete $3;

        $1->push_back(c);
        $$ = $1;
    }
    ;

enum_value_opt
    :                  { $$ = nullptr; }
    | "=" INT_LITERAL  { $$ = $2; }
    ;

/**** Expressions () **********************************************************/

expression
    : assignment_expression
    {
        $$ = $1;
    }
    ;

assignment_expression
    : logical_or_expression
    {
        $$ = $1;
    }
    | logical_or_expression "=" assignment_expression
    {
        $$ = new AssignmentExpression($1, $3);
        set_location($$, @1, @3);
    }
    | logical_or_expression compound_assignment_operator assignment_expression
    {
        $$ = new UnresolvedBinaryExpression($2, $1, $3);
        set_location($$, @1, @3);
    }
    ;

compound_assignment_operator
    : "+="   { $$ = UnresolvedBinaryExpression::PLUS_ASSIGN; }
    | "-="   { $$ = UnresolvedBinaryExpression::MINUS_ASSIGN; }
    | "*="   { $$ = UnresolvedBinaryExpression::MUL_ASSIGN; }
    | "/="   { $$ = UnresolvedBinaryExpression::DIV_ASSIGN; }
    | "%="   { $$ = UnresolvedBinaryExpression::MOD_ASSIGN; }
    | "<<="  { $$ = UnresolvedBinaryExpression::LSHIFT_ASSIGN; }
    | ">>="  { $$ = UnresolvedBinaryExpression::RSHIFT_ASSIGN; }
    ;

logical_or_expression
    : logical_and_expression
    {
        $$ = $1;
    }
    | logical_or_expression "||" logical_and_expression
    {
        $$ = new LogicalExpression(LogicalExpression::OR, $1, $3);
        set_location($$, @1, @3);
    }
    ;

logical_and_expression
    : equality_expression
    {
        $$ = $1;
    }
    | logical_and_expression "&&" equality_expression
    {
        $$ = new LogicalExpression(LogicalExpression::AND, $1, $3);
        set_location($$, @1, @3);
    }
    ;

equality_expression
    : relational_expression
    {
        $$ = $1;
    }
    | relational_expression equality_operator equality_expression
    {
        $$ = new UnresolvedBinaryExpression($2, $1, $3);
        set_location($$, @1, @3);
    }
    ;

equality_operator
    : "=="  { $$ = UnresolvedBinaryExpression::EQ; }
    | "!="  { $$ = UnresolvedBinaryExpression::NE; }
    ;

relational_expression
    : cast_expression
    {
        $$ = $1;
    }
    | cast_expression relational_operator bit_shift_expression
    {
        $$ = new UnresolvedBinaryExpression($2, $1, $3);
        set_location($$, @1, @3);
    }
    ;

relational_operator
    : "<"   { $$ = UnresolvedBinaryExpression::LT; }
    | ">"   { $$ = UnresolvedBinaryExpression::GT; }
    | "<="  { $$ = UnresolvedBinaryExpression::LE; }
    | ">="  { $$ = UnresolvedBinaryExpression::GE; }
    ;

cast_expression
    : bit_shift_expression
    {
        $$ = $1;
    }
    | bit_shift_expression "as" type
    {
        $$ = new CastExpression($1, $3);
        set_location($$, @1, @3);
    }
    ;

bit_shift_expression
    : additive_expression
    {
        $$ = $1;
    }
    | bit_shift_expression bit_shift_operator additive_expression
    {
        $$ = new UnresolvedBinaryExpression($2, $1, $3);
        set_location($$, @1, @3);
    }
    ;

bit_shift_operator
    : "<<"  { $$ = UnresolvedBinaryExpression::LSHIFT; }
    | ">>"  { $$ = UnresolvedBinaryExpression::RSHIFT; }
    ;

additive_expression
    : multiplicative_expression
    {
        $$ = $1;
    }
    | additive_expression additive_operator multiplicative_expression
    {
        $$ = new UnresolvedBinaryExpression($2, $1, $3);
        set_location($$, @1, @3);
    }
    ;

additive_operator
    : "+"  { $$ = UnresolvedBinaryExpression::PLUS; }
    | "-"  { $$ = UnresolvedBinaryExpression::MINUS; }
    ;

multiplicative_expression
    : unary_expression
    {
        $$ = $1;
    }
    | multiplicative_expression multiplicative_operator unary_expression
    {
        $$ = new UnresolvedBinaryExpression($2, $1, $3);
        set_location($$, @1, @3);
    }
    ;

multiplicative_operator
    : "*"  { $$ = UnresolvedBinaryExpression::MUL; }
    | "/"  { $$ = UnresolvedBinaryExpression::DIV; }
    | "%"  { $$ = UnresolvedBinaryExpression::MOD; }
    ;

unary_expression
    : primary_expression
    {
        $$ = $1;
    }
    | "!" unary_expression
    {
        $$ = new LogicalNotExpression($2);
        set_location($$, @1, @2);
    }
    ;

primary_expression
    : simple_name
    {
        $$ = new UnresolvedSimpleNameExpression(*$1);
        set_location($$, @1, @1);
        delete $1;
    }
    | "true"
    {
        $$ = new BooleanLiteral(true);
        set_location($$, @1, @1);
    }
    | "false"
    {
        $$ = new BooleanLiteral(false);
        set_location($$, @1, @1);
    }
    | "this"
    {
        $$ = new UnresolvedThisExpression;
        set_location($$, @1, @1);
    }
    | INT_LITERAL
    {
        $$ = new IntegerLiteral(*$1);
        set_location($$, @1, @1);
        delete $1;
    }
    | FP_LITERAL
    {
        $$ = new FloatingPointLiteral(*$1);
        set_location($$, @1, @1);
        delete $1;
    }
    | CHAR_LITERAL
    {
        $$ = new CharacterLiteral($1);
        set_location($$, @1, @1);
    }
    | primary_expression "." simple_name
    {
        $$ = new UnresolvedMemberAccessExpression($1, *$3);
        set_location($$, @1, @3);
        delete $3;
    }
    | package_module_name "." simple_name
    {
        Name n($1->str());
        delete $1;

        Expression* instance = new UnresolvedSimpleNameExpression(n);

        $$ = new UnresolvedMemberAccessExpression(instance, *$3);
        set_location($$, @1, @3);
        delete $3;
    }
    | primary_expression "(" function_arguments_opt ")"
    {
        $$ = new CallExpression($1, $3->begin(), $3->end());
        set_location($$, @1, @4);
        delete $3;
    }
    | primary_expression "[" expression "]"
    {
        $$ = new UnresolvedElementAccessExpression($1, $3);
        set_location($$, @1, @4);
    }
    | "new" type "(" function_arguments_opt ")"
    {
        if (dynamic_cast<ArrayType*>($2) != nullptr ||
            dynamic_cast<UnknownArrayType*>($2) != nullptr)
        {
            $$ = new ArrayCreationExpression($2, nullptr, $4->begin(), $4->end());
        }
        else
        {
            $$ = new UnresolvedObjectCreationExpression((UnknownType*) $2,
                                                        $4->begin(),
                                                        $4->end());
        }

        set_location($$, @1, @5);
        delete $4;
    }
    | "new" type "[" expression "]" array_elements_opt
    {
        Type* t;

        if (dynamic_cast<UnknownType*>($2) != nullptr)
            t = new UnknownArrayType((UnknownType*) $2);
        else
            t = ArrayType::get($2);

        $$ = new ArrayCreationExpression(t, $4, $6->begin(), $6->end());
        set_location($$, @1, @6);
        delete $6;
    }
    | "(" expression ")"
    {
        $$ = $2;
        set_location($$, @1, @2);
    }
    ;

function_arguments_opt
    : function_arguments  { $$ = $1; }
    |                     { $$ = new std::list<Expression*>; }
    ;

function_arguments
    : expression
    {
        $$ = new std::list<Expression*>;
        $$->push_back($1);
    }
    | function_arguments "," expression
    {
        $1->push_back($3);
        $$ = $1;
    }
    ;

array_elements_opt
    :                             { $$ = new std::list<Expression*>; }
    | "(" function_arguments ")"  { $$ = $2; }
    ;

%%
//// Error Reporting ///////////////////////////////////////////////////////////


void
yy::SoyaParser::error(const yy::SoyaParser::location_type& l,
                      const std::string& m)
{
    Location source(*l.begin.filename,
                    l.begin.line, l.begin.column,
                    l.end.line, l.end.column);

    driver->resultBuilder()->addError(source, "Syntax error.");
}


void
set_location(Node* node,
             const yy::SoyaParser::location_type& begin,
             const yy::SoyaParser::location_type& end)
{
    Location l(*begin.begin.filename,
               begin.begin.line, begin.begin.column,
               end.end.line, end.end.column);

    node->setLocation(l);
}


std::string
modifier_to_string(DeclaredEntity::Modifier mod)
{
    switch (mod)
    {
        case DeclaredEntity::EXTERN:
            return "extern";
        case DeclaredEntity::PRIVATE:
            return "private";
        case DeclaredEntity::PUBLIC:
            return "public";
        case DeclaredEntity::STATIC:
            return "static";
        default:
            assert (false);
    }
}
