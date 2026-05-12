#include "database.h"

void Database::set(const std::string& key , const std::string& value){
    data[key] = value;
}

std::string Database::get(const std::string& key){
    auto it = data.find(key);

    if (it == data.end()){
        return "(nil)";
    }

    return it->second;
}