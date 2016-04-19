/* C++ parsers require Bison 3 */
%require "3.0"
%language "C++"

/* Write-out tokens header file */
%defines

/* Use Bison's 'variant' to store values. 
 * This allows us to use non POD types (e.g.
 * with constructors/destrictors), which is
 * not possible with the default mode which
 * uses unions.
 */
%define api.value.type variant

/* 
 * Use the 'complete' symbol type (i.e. variant)
 * in the lexer
 */
%define api.token.constructor

/*
 * Add a prefix the make_* functions used to
 * create the symbols
 */
%define api.token.prefix {TOKEN_}

/* Wrap everything in our namespace */
%define api.namespace {sdfparse}

%define parser_class_name {Parser}

/* Extra checks for correct usage */
%define parse.assert

/* Enable debugging info */
%define parse.trace

/* Better error reporting */
%define parse.error verbose

/* 
 * Fixes inaccuracy in verbose error reporting.
 * May be slow for some grammars.
 */
/*%define parse.lac full*/

/* Generate a table of token names */
%token-table

/* The parser expects the lexer to be provided as a constructor argument */
%parse-param {sdfparse::Lexer& lexer}

/* Our yylex implementation expects the lexer to be passed as an argument */
%lex-param {sdfparse::Lexer& lexer}

%code requires {
    namespace sdfparse {
        class Lexer;
    }


    //This is not defined by default for some reason...
    #define YY_NULLPTR nullptr
}

%code top {

    #include "sdf_lexer.hpp"

    //Bison calls yylex() to get the next token.
    //Since we have re-defined the equivalent function in the lexer
    //we need to tell Bison how to get the next token.
    static sdfparse::Parser::symbol_type yylex(sdfparse::Lexer &lexer) {
        return lexer.next_token();
    }

    #include <iostream> //For cout in error reporting
}

%token LPAR "("
%token RPAR ")"
%token DELAYFILE "DELAYFILE"
%token SDFVERSION "SDFVERSION"
%token DIVIDER "DIVIDER"
%token TIMESCALE "TIMESCALE"
%token CELL "CELL"
%token CELLTYPE "CELLTYPE"
%token INSTANCE "INSTANCE"
%token DELAY "DELAY"
%token ABSOLUTE "ABSOLUTE"
%token IOPATH "IOPATH"
%token COLON ":"
%token <int> Integer "integer"
%token <double> Float "float"
%token <std::string> String "string"
%token <std::string> QuotedString "quoted-string"
%token EOF 0 "end-of-file"

%start sdf_file

%%
sdf_file : LPAR DELAYFILE sdf_data RPAR { }
         ;

sdf_data : /* Empty */ { }
         | sdf_data sdf_version { }
         | sdf_data divider { }
         | sdf_data timescale { }
         | sdf_data cell { }
         ;

sdf_version : LPAR SDFVERSION QuotedString RPAR { }
            ;

divider : LPAR DIVIDER String RPAR { }
        ;

timescale : LPAR TIMESCALE Integer String RPAR { }
          ;

cell : LPAR CELL cell_type instance delay RPAR { }
     ;

cell_type : LPAR CELLTYPE QuotedString RPAR { }
          ;

instance : LPAR INSTANCE String RPAR { }
         ;

delay : LPAR DELAY absolute RPAR { }
      ;

absolute : LPAR ABSOLUTE iopath_list RPAR { }
         ;

iopath_list : /* empty */ { }
            | iopath { }
            ;

iopath : LPAR IOPATH String String delay_triple delay_triple RPAR { }
       ;

delay_triple : LPAR delay_value COLON delay_value COLON delay_value RPAR { }
             ;

delay_value : Integer { }
            | Float { }
            ;

%%

//We need to provide an implementation for parser error handling
void sdfparse::Parser::error(const std::string& msg) {
    std::cout << "SDF Parsing Error: " << msg << "\n";
}
