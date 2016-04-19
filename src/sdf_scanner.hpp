#pragma once

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
 * in sdf_scanner.l
 */
#define yyFlexLexer SdfParse_Lexer
#include <FlexLexer.h>

#endif

/*
 * The YY_DECL is used by flex to specify the signature of the main
 * lexer function.
 *
 * We re-define it to something reasonable
 */
#undef YY_DECL
#define YY_DECL int sdfparse::Lexer::next_token()



namespace sdfparse {

class Lexer : private yyFlexLexer {
    //We use private inheritance to hide the flex
    //implementation details from anyone using Lexer
    public:
        int next_token();
};

} //sdfparse
