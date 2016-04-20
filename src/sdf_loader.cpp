#include <fstream>
#include "sdf_loader.hpp"

#include "sdf_lexer.hpp"
#include "sdf_parser.gen.hpp"
#include "location.hh"

namespace sdfparse {

Loader::Loader()
    : filename_("") //Initialize the filename
    , lexer_(new Lexer())
    , parser_(new Parser(*lexer_, *this)) {
}


//We need to declare the destructor in the .cpp file,
//since to destruct the unique_ptr's we also need the
//full definitions of Lexer and Parser
Loader::~Loader()
    {}

void Loader::load(std::string filename) {
    std::ifstream is(filename);
    load(is, filename);
}

void Loader::load(std::istream& is, std::string filename) {
    assert(is.good());

    //Update the filename for location references
    filename_ = filename;

    //Point the lexer at the new input
    lexer_->switch_streams(&is);

    //Initialize locations with filename
    auto pos = position(&filename_);
    auto loc = location(pos, pos);
    lexer_->set_loc(loc);

    //Do the parsing
    auto result = parser_->parse();
    assert(result == 0);
}

} //sdfparse
