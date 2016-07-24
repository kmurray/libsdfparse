#pragma once
#include <string>

//Describes how to escape a string
enum class EscapeStyle {
    ALL_CHARS, //Escape all characters 
    EXCLUDE_LAST_INDEX //Escape all characters except for final indexing
};

std::string escape_sdf_identifier(const std::string identifier, EscapeStyle style=EscapeStyle::ALL_CHARS);
std::string unescape_sdf_identifier(const std::string str);
