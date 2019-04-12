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
    #include "sdf_escape.hpp"

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
%token POSEDGE "posedge"
%token NEGEDGE "negedge"
%token SETUP "SETUP"
%token HOLD "HOLD"
%token REMOVAL "REMOVAL"
%token RECOVERY "RECOVERY"
%token TIMINGCHECK "TIMINGCHECK"
%token <double> Float "float"
%token <std::string> String "string"
%token <std::string> Qstring "quoted-string"
%token EOF 0 "end-of-file"

%type <std::string> Id "identifier"
%type <std::string> Qid "quoted-identifier"
%type <RealTriple> real_triple
%type <PortSpec> port_spec
%type <Iopath> iopath
%type <std::vector<Iopath>> iopath_list
%type <std::vector<Iopath>> absolute
%type <Delay> delay
%type <std::string> instance
%type <std::string> celltype
%type <PortCondition> port_condition
%type <TimingCheck> timing_check
%type <std::vector<Timing>> timing_check_list
%type <Timing> t_check
%type <Timing> setup_check
%type <Timing> hold_check
%type <Timing> removal_check
%type <Timing> recovery_check
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

sdf_version : LPAR SDFVERSION Qid RPAR { $$ = $3; }
            ;

design : LPAR DESIGN Qid RPAR { $$ = $3; }
       ;

vendor : LPAR VENDOR Qid RPAR { $$ = $3; }
       ;

program : LPAR PROGRAM Qid RPAR { $$ = $3; }
        ;

version : LPAR VERSION Qid RPAR { $$ = $3; }
        ;

hierarchy_divider : LPAR DIVIDER Id RPAR { $$ = $3; }
                  ;

timescale : LPAR TIMESCALE Float Id RPAR { $$ = Timescale($3, $4); }
          ;

cell : LPAR CELL celltype instance timing_check RPAR { $$ = Cell($3, $4, Delay(), $5); }
     | LPAR CELL celltype instance delay RPAR { $$ = Cell($3, $4, $5, TimingCheck()); }
     | LPAR CELL celltype instance RPAR { $$ = Cell($3, $4, Delay(), TimingCheck()); }
     | LPAR CELL celltype instance delay timing_check RPAR { $$ = Cell($3, $4, Delay(), TimingCheck()); }
     ;

celltype : LPAR CELLTYPE Qid RPAR { $$ = $3; }
         ;

instance : LPAR INSTANCE Id RPAR { $$ = $3; }
         ;

timing_check : LPAR TIMINGCHECK timing_check_list RPAR { $$ = TimingCheck($3); }
             ;

timing_check_list : t_check { $$ = std::vector<Timing>(); $$.push_back($1); }
                | timing_check_list t_check { $$ = $1; $$.push_back($2); }
                ;

t_check: removal_check
       | recovery_check
       | hold_check
       | setup_check
       ;

removal_check : LPAR REMOVAL port_spec port_spec real_triple RPAR { $$ = Timing($4, $3, $5, "REMOVAL"); }

recovery_check : LPAR RECOVERY port_spec port_spec real_triple RPAR { $$ = Timing($4, $3, $5, "RECOVERY"); }

hold_check : LPAR HOLD port_spec port_spec real_triple RPAR { $$ = Timing($4, $3, $5, "HOLD"); }

setup_check : LPAR SETUP port_spec port_spec real_triple RPAR { $$ = Timing($4, $3, $5, "SETUP"); }


delay : LPAR DELAY absolute RPAR { $$ = Delay(Delay::Type::ABSOLUTE, $3); }
      ;

absolute : LPAR ABSOLUTE RPAR { $$ = std::vector<Iopath>(); } 
         | LPAR ABSOLUTE iopath_list RPAR { $$ = $3; }
         ;

iopath_list : iopath             { $$ = std::vector<Iopath>(); $$.push_back($1); }
            | iopath_list iopath { $1.push_back($2); $$ = $1; }
            ;

iopath : LPAR IOPATH port_spec port_spec real_triple real_triple RPAR { $$ = Iopath($3, $4, $5, $6); }
       ;

port_spec : Id { $$ = PortSpec($1, PortCondition::NONE); }
          | LPAR port_condition Id RPAR { $$ = PortSpec($3, $2); }
          | Float { $$ = PortSpec(std::to_string((int)$1), PortCondition::NONE); }
          ;

port_condition: POSEDGE { $$ = PortCondition::POSEDGE; }
              | NEGEDGE { $$ = PortCondition::NEGEDGE; }
              ;

real_triple : LPAR Float COLON Float COLON Float RPAR { $$ = RealTriple($2, $4, $6); }
            | LPAR RPAR { $$ = RealTriple(); }
            | LPAR Float COLON COLON Float RPAR { $$ = RealTriple($2, $5); }
            ;


Id : String { $$ = unescape_sdf_identifier($1); }
Qid : Qstring { $$ = unescape_sdf_identifier($1); }

%%

//We need to provide an implementation for parser error handling
void sdfparse::Parser::error(const sdfparse::location& loc, const std::string& msg) {
    throw sdfparse::ParseError(msg, loc);
}
