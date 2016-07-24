#include "sdf_escape.hpp"
#include <locale>

bool is_special_sdf_char(char c);


//Returns true if c is categorized as a special character in SDF
bool is_special_sdf_char(char c) {
    //From section 3.2.5 of IEEE1497 Part 3 (i.e. the SDF spec)
    //Special characters run from:
    //    ! to # (ASCII decimal 33-35)
    //    % to / (ASCII decimal 37-47)
    //    : to @ (ASCII decimal 58-64)
    //    [ to ^ (ASCII decimal 91-94)
    //    ` to ` (ASCII decimal 96)
    //    { to ~ (ASCII decimal 123-126)
    //
    //Not that the spec defines _ (decimal code 95) and $ (decimal code 36) 
    //as non-special alphanumeric characters. 
    //
    //However it inconsistently also lists $ in the list of special characters.
    //Since the spec allows for non-special characters to be escaped (they are treated
    //normally), we treat $ as a special character to be safe.
    //
    //Note that the spec appears to have rendering errors in the PDF availble
    //on IEEE Xplore, listing the 'LEFT-POINTING DOUBLE ANGLE QUOTATION MARK' 
    //character (decimal code 171) in place of the APOSTROPHE character ' 
    //with decimal code 39 in the special character list. We assume code 39
    if((c >= 33 && c <= 35) ||
       (c == 36) || // $
       (c >= 37 && c <= 47) ||
       (c >= 58 && c <= 64) ||
       (c >= 91 && c <= 94) ||
       (c == 96) ||
       (c >= 123 && c <= 126)) {
        return true;
    }

    return false;
}

//Escapes the given identifier to be safe for sdf
std::string escape_sdf_identifier(const std::string identifier, EscapeStyle style) {
    //SDF allows escaped characters
    //
    //We look at each character in the string and escape it if it is
    //a special character
    std::string escaped_name;
    std::string postfix = "";

    auto end = identifier.size();

    if(style == EscapeStyle::EXCLUDE_LAST_INDEX) {
        //Determine if we have a valid index at the end
        auto last_open_bracket = identifier.find_last_of("[");
        auto last_end_bracket = identifier.find_last_of("]");

        bool ends_with_close_bracket = (last_end_bracket == identifier.size() - 1);
        bool has_open_bracket = (last_open_bracket < last_end_bracket);

        if(has_open_bracket && ends_with_close_bracket) {
            bool only_integers_between_brackets = true;
            for(size_t i = last_open_bracket + 1; i < last_end_bracket; i++) {
                if(!std::isdigit(identifier[i])) {
                    only_integers_between_brackets = false;
                }
            }

            if(only_integers_between_brackets) {
                //Copy the unescaped last indexing
                postfix = std::string(identifier, last_open_bracket, std::string::npos);

                //Only escape up to the last index
                end = last_open_bracket;
            }
        }
    }

    for(size_t i = 0; i < end; ++i) {
        char c = identifier[i];
        if(is_special_sdf_char(c)) {
            //Escape the special character
            escaped_name += '\\';
        }
        escaped_name += c;
    }

    //Used to append unescaped index if needed
    escaped_name += postfix;

    return escaped_name;
}

//Unsecapes and SDF identifier by removeing all back-slashes
std::string unescape_sdf_identifier(const std::string str) {
    std::string unescaped;
    for(char c : str) {
        if(c != '\\') {
            unescaped += c;
        }
    }
    return unescaped;
}
