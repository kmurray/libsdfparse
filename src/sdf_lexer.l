%{
    #include <iostream>
    #include <sstream>
    #include <cassert>
    #include "sdf_lexer.hpp"
    #include "sdf_parser.gen.hpp"
    #include "location.hh"

    using std::cout;
    using std::stringstream;

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

\(                                              { cout << "LPAR\n"; return sdfparse::Parser::make_LPAR(sdfparse::location()); }
\)                                              { cout << "RPAR\n"; return sdfparse::Parser::make_RPAR(sdfparse::location()); }

DELAYFILE                                       { cout << "DELAYFILE\n"; return sdfparse::Parser::make_DELAYFILE(sdfparse::location()); }
SDFVERSION                                      { cout << "SDFVERSION\n"; return sdfparse::Parser::make_SDFVERSION(sdfparse::location()); }
DIVIDER                                         { cout << "DIVIDER\n"; return sdfparse::Parser::make_DIVIDER(sdfparse::location()); }
TIMESCALE                                       { cout << "TIMESCALE\n"; return sdfparse::Parser::make_TIMESCALE(sdfparse::location()); }
CELL                                            { cout << "CELL\n"; return sdfparse::Parser::make_CELL(sdfparse::location()); }
CELLTYPE                                        { cout << "CELLTYPE\n"; return sdfparse::Parser::make_CELLTYPE(sdfparse::location()); }
INSTANCE                                        { cout << "INSTANCE\n"; return sdfparse::Parser::make_INSTANCE(sdfparse::location()); }
DELAY                                           { cout << "DELAY\n"; return sdfparse::Parser::make_DELAY(sdfparse::location()); }
ABSOLUTE                                        { cout << "ABSOLUTE\n"; return sdfparse::Parser::make_ABSOLUTE(sdfparse::location()); }
IOPATH                                          { cout << "IOPATH\n"; return sdfparse::Parser::make_IOPATH(sdfparse::location()); }
{COLON}                                         { cout << "COLON\n"; return sdfparse::Parser::make_COLON(sdfparse::location()); }
[-+]?({DIGIT}*\.?{DIGIT}+|{DIGIT}+\.)           { 
                                                    cout << "Float: " << YYText() << "\n";
                                                    stringstream ss;
                                                    ss << YYText();
                                                    double val;
                                                    ss >> val;
                                                    assert(!ss.fail() && ss.eof());
                                                    return sdfparse::Parser::make_Float(val, sdfparse::location()); 
                                                }
\"({ALPHA}|{DIGIT}|{SYMBOL})+\"                 {   
                                                    cout << "Qstring: " << YYText() << "\n"; 
                                                    return sdfparse::Parser::make_Qstring(YYText(), sdfparse::location()); 
                                                }
({ALPHA}|{SYMBOL})({ALPHA}|{DIGIT}|{SYMBOL})*   { 
                                                    cout << "String: " << YYText() << "\n"; 
                                                    return sdfparse::Parser::make_String(YYText(), sdfparse::location()); 
                                                }

.                                               { assert(false); }

<<EOF>>                                         { cout << "EOF\n"; return sdfparse::Parser::make_EOF(sdfparse::location()); }

%%
