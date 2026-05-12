#include "command_handler.h"

CommandHandler::CommandHandler(Database& db)
    : database(db){}

std::string CommandHandler::execute(const std::string& command){
    auto tokens = parser.parse(command);

    if (tokens.empty()){
        return "Empty command";
    }

    if(tokens[0] == "SET"){
        if(tokens.size() != 3){
            return "Usage: SET <key> <value>";
        }

        database.set(tokens[1], tokens[2]);
        return "OK";
    }
    
    if (tokens[0] == "GET"){
        return database.get(tokens[1]);
    }

    return "Unknown command";
}