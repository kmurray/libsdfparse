#pragma once

#include <iosfwd>
#include <memory>

//#include "sdf_lexer.hpp"
//#include "sdf_parser.gen.hpp"

namespace sdfparse {

//Forward delcarations
class Lexer;
class Parser;
class ParseError;

class Loader {

    public:
        Loader();
        ~Loader();

        bool load(std::string filename);
        bool load(std::istream& is, std::string filename="<inputstream>");

        virtual void on_error(ParseError& error);

    private:
        std::string filename_;
        std::unique_ptr<Lexer> lexer_;
        std::unique_ptr<Parser> parser_;
};

} //sdfparse
