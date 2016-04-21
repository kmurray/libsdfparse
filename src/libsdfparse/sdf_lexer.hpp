#pragma once

#include "sdf_parser.gen.hpp"

/*
 * Flex requires that yyFlexLexer is equivalent to
 * the lexer class we are using, before we include
 * FlexLexer.h.  See 'Generating C++ Scanners' in 
 * the Flex manual.
 */
#if ! defined(yyFlexLexerOnce)

#undef yyFlexLexer
/*
 * Flex doesn't support C++ namespaces, so
 * it is faked using the '%option prefix' defined
 * in sdf_scanner.l, for some reason Flex also prefixes
 * the class name with Flex.
 *
 * So:
 * %option prefix=SdfParse_
 * %option yyclass=Lexer
 * becomes 'SdfParse_FlexLexer'
 */
#define yyFlexLexer SdfParse_FlexLexer
#include <FlexLexer.h>

#endif

/*
 * The YY_DECL is used by flex to specify the signature of the main
 * lexer function.
 *
 * We re-define it to something reasonable
 */
#undef YY_DECL
#define YY_DECL sdfparse::Parser::symbol_type sdfparse::Lexer::next_token()

/*
 * Bison generated location tracking
 */
#include "location.hh"

namespace sdfparse {

class Lexer : private yyFlexLexer {
    //We use private inheritance to hide the flex
    //implementation details from anyone using Lexer
    public:
        sdfparse::Parser::symbol_type next_token();

        using yyFlexLexer::switch_streams;

        location get_loc() { return loc_; }
        void set_loc(location& loc) { loc_ = loc; }
    private:
        location loc_; 
};

} //sdfparse
