#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H

#include <string>

#include "database.h"
#include "parser.h"

class CommandHandler
{
public:
    CommandHandler(Database& db);

    std::string execute(const std::string& command);

private:
    Database& database;
    Parser parser;
};

#endif