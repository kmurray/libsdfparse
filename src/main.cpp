#include <memory>

#include "sdf_scanner.hpp"

int main(int argc, char** argv) {

    std::shared_ptr<sdfparse::Lexer> lexer = std::make_shared<sdfparse::Lexer>();

    while(lexer->next_token() != 0) {}

    return 0;
}
