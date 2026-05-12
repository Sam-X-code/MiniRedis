#include "parser.h"

#include <sstream>
#include <vector>

std::vector<std::string> Parser::parse(const std::string& command){
    std::stringstream ss(command);

    std::vector<std::string> tokens;
    std::string word;

    while (ss >> word){
        tokens.push_back(word);
    }

    return tokens;
}