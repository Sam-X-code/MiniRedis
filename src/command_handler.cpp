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

    if (tokens[0] == "DEL"){
        if (tokens.size() != 2){
            return "Usage: DEL <key>";
        }

        if (database.del(tokens[1])){
            return "OK";
        }

        return "(nil)";
    }

    if (tokens[0] == "EXISTS"){
        if (tokens.size() != 2){
            return "Usage: EXISTS <key>";
        }

        return database.exists(tokens[1]) ? "1" : "0";
    }

    if (tokens[0] == "KEYS"){
        return database.keys();
    }

    if (tokens[0] == "EXPIRE"){
        if (tokens.size() != 3){
            return "Usage: EXPIRE <key> <seconds>";
        }

        int seconds = std::stoi(tokens[2]);

        return database.expire(tokens[1], seconds)
            ? "OK"
            : "(nil)";
    }



    return "Unknown command";
}