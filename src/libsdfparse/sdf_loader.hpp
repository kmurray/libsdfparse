#pragma once

#include <iosfwd>
#include <memory>

#include "sdf_data.hpp"

namespace sdfparse {

//Forward delcarations
class Lexer;
class Parser;
class ParseError;

//Class for loading an SDF file.
//
//The sdf file can be parsed using load(), which returns true
//if successful - after which the loaded data can be accessed via 
//get_delayfile().
//
//The virtual method on_error() can be overriding to control
//error handling. The default simply prints out an error message,
//but it could also be defined to (re-)throw an exception.
class Loader {

    public:
        Loader();
        ~Loader();

        bool load(std::string filename);
        bool load(std::istream& is, std::string filename="<inputstream>");

        const DelayFile& get_delayfile() { return delayfile_; };

    protected:
        virtual void on_error(ParseError& error);

    private:
        friend Parser;
        std::string filename_;
        std::unique_ptr<Lexer> lexer_;
        std::unique_ptr<Parser> parser_;

        DelayFile delayfile_;
};

} //sdfparse
