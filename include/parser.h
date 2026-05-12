#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>

class Parser{
public:
    std::vector<std::string> parse(const std::string& command);
};

#endif