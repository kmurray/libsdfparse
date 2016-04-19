#include <memory>

#include "sdf_lexer.hpp"
#include "sdf_parser.gen.hpp"

int main(int argc, char** argv) {

    std::shared_ptr<sdfparse::Lexer> lexer = std::make_shared<sdfparse::Lexer>();

    //while(lexer->next_token() != 0) {}
    //while(true) {
        //lexer->next_token(); 
    //}

    sdfparse::Parser parser(*lexer);
    parser.parse();

    return 0;
}
