#pragma once

#include <iosfwd>
#include <memory>

#include "sdf_data.hpp"

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

        const DelayFile& get_delayfile() { return delayfile_; };

        virtual void on_error(ParseError& error);

    private:
        friend Parser;
        std::string filename_;
        std::unique_ptr<Lexer> lexer_;
        std::unique_ptr<Parser> parser_;

        DelayFile delayfile_;
};

} //sdfparse
