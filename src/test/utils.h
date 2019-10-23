#pragma once

#include <string>
#include <regex>
#include <vector>

std::vector<std::string> split_string(const std::string& input, const std::string& regex = "\n") {
    // passing -1 as the submatch index parameter performs splitting
    std::regex re(regex);
    std::sregex_token_iterator
        first{std::begin(input), std::end(input), re, -1},
        last;
    return {first, last};
}