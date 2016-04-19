#include <memory>
#include <fstream>

#include "sdf_lexer.hpp"
#include "sdf_parser.gen.hpp"

int main(int argc, char** argv) {

    if(argc != 2) {
        std::cout << "Usage: " << argv[0] << "sdf_file" << "\n";
        return 1;
    }

    std::ifstream is(argv[1]);
    assert(is.good());

    std::shared_ptr<sdfparse::Lexer> lexer = std::make_shared<sdfparse::Lexer>();
    lexer->switch_streams(&is, nullptr);

    //while(lexer->next_token() != 0) {}
    //while(true) {
        //lexer->next_token(); 
    //}

    sdfparse::Parser parser(*lexer);
    auto result = parser.parse();
    assert(result == 0);

    return 0;
}
