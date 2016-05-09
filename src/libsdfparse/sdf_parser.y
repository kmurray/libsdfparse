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

/* Track locations */
%locations

/* Generate a table of token names */
%token-table

/* The parser expects the lexer to be provided as a constructor argument */
%parse-param {sdfparse::Lexer& lexer}
%parse-param {sdfparse::Loader& driver}

/* Our yylex implementation expects the lexer to be passed as an argument */
%lex-param {sdfparse::Lexer& lexer}

%code requires {
    namespace sdfparse {
        class Lexer;
        class Loader;
    }

    #include "sdf_error.hpp"
    #include "sdf_data.hpp"

    //This is not defined by default for some reason...
    #define YY_NULLPTR nullptr
}

%code top {

    #include "sdf_lexer.hpp"
    #include "sdf_loader.hpp"

    //Bison calls yylex() to get the next token.
    //Since we have re-defined the equivalent function in the lexer
    //we need to tell Bison how to get the next token.
    static sdfparse::Parser::symbol_type yylex(sdfparse::Lexer& lexer) {
        return lexer.next_token();
    }

    #include <iostream> //For cout in error reporting
}

%token LPAR "("
%token RPAR ")"
%token DELAYFILE "DELAYFILE"
%token SDFVERSION "SDFVERSION"
%token DESIGN "DESIGN"
%token VENDOR "VENDOR"
%token PROGRAM "PROGRAM"
%token VERSION "VERSION"
%token DIVIDER "DIVIDER"
%token TIMESCALE "TIMESCALE"
%token CELL "CELL"
%token CELLTYPE "CELLTYPE"
%token INSTANCE "INSTANCE"
%token DELAY "DELAY"
%token ABSOLUTE "ABSOLUTE"
%token IOPATH "IOPATH"
%token COLON ":"
%token <double> Float "float"
%token <std::string> String "string"
%token <std::string> Qstring "quoted-string"
%token EOF 0 "end-of-file"

%type <RealTriple> real_triple
%type <Iopath> iopath
%type <std::vector<Iopath>> iopath_list
%type <std::vector<Iopath>> absolute
%type <Delay> delay
%type <std::string> instance
%type <std::string> celltype
%type <Cell> cell
%type <Timescale> timescale
%type <std::string> hierarchy_divider
%type <std::string> version
%type <std::string> program
%type <std::string> vendor
%type <std::string> design
%type <std::string> sdf_version
%type <Header> sdf_header
%type <std::vector<Cell>> cell_list
%type <DelayFile> sdf_file

%start sdf_file

%%
sdf_file : LPAR DELAYFILE sdf_header RPAR { driver.delayfile_ = DelayFile($3); }
         | LPAR DELAYFILE sdf_header cell_list RPAR { driver.delayfile_ = DelayFile($3, $4); }
         ;

sdf_header : sdf_version                    { $$ = Header($1); }
           | sdf_header design              { $1.set_design($2); $$ = $1; }
           | sdf_header vendor              { $1.set_vendor($2); $$ = $1; }
           | sdf_header program             { $1.set_program($2); $$ = $1; }
           | sdf_header version             { $1.set_version($2); $$ = $1; }
           | sdf_header hierarchy_divider   { $1.set_divider($2); $$ = $1; }
           | sdf_header timescale           { $1.set_timescale($2); $$ = $1; }
           ;

cell_list : cell { $$ = std::vector<Cell>(); $$.push_back($1); }
          | cell_list cell  { $1.push_back($2); $$ = $1; }
          ;

sdf_version : LPAR SDFVERSION Qstring RPAR { $$ = $3; }
            ;

design : LPAR DESIGN Qstring RPAR { $$ = $3; }
       ;

vendor : LPAR VENDOR Qstring RPAR { $$ = $3; }
       ;

program : LPAR PROGRAM Qstring RPAR { $$ = $3; }
        ;

version : LPAR VERSION Qstring RPAR { $$ = $3; }
        ;

hierarchy_divider : LPAR DIVIDER String RPAR { $$ = $3; }
                  ;

timescale : LPAR TIMESCALE Float String RPAR { $$ = Timescale($3, $4); }
          ;

cell : LPAR CELL celltype instance delay RPAR { $$ = Cell($3, $4, $5); }
     | LPAR CELL celltype instance RPAR { $$ = Cell($3, $4, Delay()); }
     ;

celltype : LPAR CELLTYPE Qstring RPAR { $$ = $3; }
         ;

instance : LPAR INSTANCE String RPAR { $$ = $3; }
         ;

delay : LPAR DELAY absolute RPAR { $$ = Delay(Delay::Type::ABSOLUTE, $3); }
      ;

absolute : LPAR ABSOLUTE RPAR { $$ = std::vector<Iopath>(); } 
         | LPAR ABSOLUTE iopath_list RPAR { $$ = $3; }
         ;

iopath_list : iopath             { $$ = std::vector<Iopath>(); $$.push_back($1); }
            | iopath_list iopath { $1.push_back($2); $$ = $1; }
            ;

iopath : LPAR IOPATH String String real_triple real_triple RPAR { $$ = Iopath($3, $4, $5, $6); }
       ;

real_triple : LPAR Float COLON Float COLON Float RPAR { $$ = RealTriple($2, $4, $6); }
             ;

%%

//We need to provide an implementation for parser error handling
void sdfparse::Parser::error(const sdfparse::location& loc, const std::string& msg) {
    throw sdfparse::ParseError(msg, loc);
}
