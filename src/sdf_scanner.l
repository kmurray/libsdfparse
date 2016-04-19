%{
    #include <iostream>
    #include <cassert>
    #include "sdf_scanner.hpp"

    using std::cout;

%}

/* Generate a C++ lexer */
%option c++

/* track line numbers*/
%option yylineno 

/* No lexing accross files */
%option noyywrap

/* unistd.h doesn't exist on windows */
%option nounistd

/* isatty() doesn't exist on windows */
%option never-interactive

/* no default rule to echo unrecongaized tokens to output */
%option nodefault

/* 
 * We want the yylex() member function to be implemented
 * in our class, rather than in the FlexLexer parent class.
 *
 * This allows us to acces member vairables of the lexer class
 * in side the flex rules below.
 */
%option yyclass="Lexer"

/*
 * Use a prefix to avoid name clashes with other
 * flex lexers
 */
%option prefix="SdfParse_" 

/* Character classes */
ALPHA [a-zA-Z]
SYMBOL [-_~|*/\[\]\.\{\}^+$/]
DIGIT [0-9]
COLON [:]
WS [ \t]
ENDL (\n|\n\r)

%%

{WS}                                            { /* skip white space */ }
{ENDL}                                          { /* skip end-of-line */ }

\(                                              { cout << "LPAR\n"; }
\)                                              { cout << "RPAR\n"; }

DELAYFILE                                       { cout << "DELAYFILE\n"; }
SDFVERSION                                      { cout << "SDFVERSION\n"; }
DIVIDER                                         { cout << "DIVIDER\n"; }
TIMESCALE                                       { cout << "TIMESCALE\n"; }
CELL                                            { cout << "CELL\n"; }
CELLTYPE                                        { cout << "CELLTYPE\n"; }
INSTANCE                                        { cout << "INSTANCE\n"; }
DELAY                                           { cout << "DELAY\n"; }
ABSOLUTE                                        { cout << "ABSOLUTE\n"; }
IOPATH                                          { cout << "IOPATH\n"; }
{COLON}                                         { cout << "COLON\n"; }
{DIGIT}+                                        { cout << "Integer\n"; }
\"({ALPHA}|{DIGIT}|{SYMBOL})+\"                 { cout << "Qstring: " << YYText() << "\n"; }
({ALPHA}|{SYMBOL})({ALPHA}|{DIGIT}|{SYMBOL})*   { cout << "String: " << YYText() << "\n"; }

.                                               { assert(false); }

<<EOF>>                                         { cout << "EOF\n"; }

%%

#include <memory>
#include "sdf_scanner.hpp"

int main(int argc, char** argv) {

    std::shared_ptr<sdfparse::Lexer> lexer = std::make_shared<sdfparse::Lexer>();

    while(lexer->next_token() != 0) {}

    return 0;
}
