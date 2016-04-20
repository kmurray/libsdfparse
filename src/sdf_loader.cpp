#include <iostream>
#include "sdf_loader.hpp"

#include "sdf_lexer.hpp"
#include "sdf_parser.gen.hpp"

namespace sdfparse {

Loader::Loader()
    : lexer_(new Lexer)
    , parser_(new Parser(*lexer_))
    {}


//We need to declare the destructor in the .cpp file,
//since to destruct the unique_ptr's we also need the
//full definitions of Lexer and Parser
Loader::~Loader()
    {}

void Loader::load(std::istream& is) {
    assert(is.good());

    lexer_->switch_streams(&is);

    auto result = parser_->parse();
    assert(result == 0);
}

} //sdfparse
