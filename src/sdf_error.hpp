#pragma once
#include <stdexcept>
#include <string>

#include "location.hh"


namespace sdfparse {

class ParseError : public std::runtime_error {
    public:
        ParseError(const std::string& msg, location loc)
            : std::runtime_error(msg)
            , loc_(loc)
            {}

        location loc() { return loc_; }

    private:
        location loc_;
};

}
