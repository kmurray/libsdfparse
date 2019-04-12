%{
    #include <iostream>
    #include <sstream>
    #include <cassert>
    #include "sdf_lexer.hpp"
    #include "sdf_parser.gen.hpp"
    #include "location.hh"

    using std::cout;
    using std::stringstream;

    //Called everytime a token matches
    //This advances the end of the location to the end
    //of the token.
    #define YY_USER_ACTION loc_.columns(YYLeng());

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
SYMBOL [-_~|*/\[\]\.\{\}^+$/\\]
DIGIT [0-9]
COLON [:]
WS [ \t]
ENDL (\n|\n\r)

%%

%{
    //Run everytime yylex is called
    loc_.step(); //Move begining of location to end
%}

{WS}+                                           { /* skip white space */ loc_.step(); }
{ENDL}                                          { /* skip end-of-line */ loc_.lines(1); loc_.step(); }

\(                                              { /* cout << "LPAR\n"; */ return sdfparse::Parser::make_LPAR(loc_); }
\)                                              { /* cout << "RPAR\n"; */ return sdfparse::Parser::make_RPAR(loc_); }

DELAYFILE                                       { /* cout << "DELAYFILE\n"; */ return sdfparse::Parser::make_DELAYFILE(loc_); }
SDFVERSION                                      { /* cout << "SDFVERSION\n"; */ return sdfparse::Parser::make_SDFVERSION(loc_); }
DESIGN                                          { /* cout << "DESIGN\n"; */ return sdfparse::Parser::make_DESIGN(loc_); }
VENDOR                                          { /* cout << "VENDOR\n"; */ return sdfparse::Parser::make_VENDOR(loc_); }
PROGRAM                                         { /* cout << "PROGRAM\n"; */ return sdfparse::Parser::make_PROGRAM(loc_); }
VERSION                                         { /* cout << "VERSION\n"; */ return sdfparse::Parser::make_VERSION(loc_); }
DIVIDER                                         { /* cout << "DIVIDER\n"; */ return sdfparse::Parser::make_DIVIDER(loc_); }
TIMESCALE                                       { /* cout << "TIMESCALE\n"; */ return sdfparse::Parser::make_TIMESCALE(loc_); }
CELL                                            { /* cout << "CELL\n"; */ return sdfparse::Parser::make_CELL(loc_); }
CELLTYPE                                        { /* cout << "CELLTYPE\n"; */ return sdfparse::Parser::make_CELLTYPE(loc_); }
INSTANCE                                        { /* cout << "INSTANCE\n"; */ return sdfparse::Parser::make_INSTANCE(loc_); }
DELAY                                           { /* cout << "DELAY\n"; */ return sdfparse::Parser::make_DELAY(loc_); }
ABSOLUTE                                        { /* cout << "ABSOLUTE\n"; */ return sdfparse::Parser::make_ABSOLUTE(loc_); }
IOPATH                                          { /* cout << "IOPATH\n"; */ return sdfparse::Parser::make_IOPATH(loc_); }
{COLON}                                         { /* cout << "COLON\n"; */ return sdfparse::Parser::make_COLON(loc_); }
posedge                                         { return sdfparse::Parser::make_POSEDGE(loc_); }
negedge                                         { return sdfparse::Parser::make_NEGEDGE(loc_); }
SETUP                                           { return sdfparse::Parser::make_SETUP(loc_); }
HOLD                                            { return sdfparse::Parser::make_HOLD(loc_); }
REMOVAL                                         { return sdfparse::Parser::make_REMOVAL(loc_); }
RECOVERY                                        { return sdfparse::Parser::make_RECOVERY(loc_); }
TIMINGCHECK                                     { return sdfparse::Parser::make_TIMINGCHECK(loc_); }
[-+]?({DIGIT}*\.?{DIGIT}+|{DIGIT}+\.)           { 
                                                    /*cout << "Float: " << YYText() << "\n";*/
                                                    stringstream ss;
                                                    ss << YYText();
                                                    double val;
                                                    ss >> val;

                                                    if(ss.fail() || !ss.eof()) {
                                                        stringstream msg_ss;
                                                        msg_ss << "Failed to parse float number '" << YYText() << "'";
                                                        throw sdfparse::ParseError(msg_ss.str(), loc_);
                                                    }

                                                    return sdfparse::Parser::make_Float(val, loc_); 
                                                }
\"({ALPHA}|{DIGIT}|{SYMBOL})+\"                 {   
                                                    //Trim off the beginning and ending quotes
                                                    const char* begin = YYText() + 1;
                                                    const char* end = begin + YYLeng() - 2;
                                                    std::string str(begin, end);
                                                    /*cout << "Qstring: " << str << "\n"; */
                                                    return sdfparse::Parser::make_Qstring(str, loc_); 
                                                }
({ALPHA}|{SYMBOL})({ALPHA}|{DIGIT}|{SYMBOL})*   { 
                                                    /*cout << "String: " << YYText() << "\n"; */
                                                    return sdfparse::Parser::make_String(YYText(), loc_); 
                                                }

.                                               { 
                                                    stringstream ss;
                                                    ss << "Unexpected character '" << YYText() << "'";
                                                    throw sdfparse::ParseError(ss.str(), loc_);
                                                }

<<EOF>>                                         { 
                                                    /*cout << "EOF\n";*/
                                                    return sdfparse::Parser::make_EOF(loc_);
                                                }

%%
