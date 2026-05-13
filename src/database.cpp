#include "database.h"

void Database::set(const std::string& key , const std::string& value){
    std::lock_guard<std::mutex> lock(mutex);
    
    data[key] = value;
}

std::string Database::get(const std::string& key){
    std::lock_guard<std::mutex> lock(mutex);

    auto it = data.find(key);

    if (it == data.end()){
        return "(nil)";
    }

    return it->second;
}

bool Database::del(const std::string& key){
    std::lock_guard<std::mutex> lock(mutex);

    return data.erase(key) > 0;
}

bool Database::exists(const std::string& key){
    std::lock_guard<std::mutex> lock(mutex);

    return data.find(key) != data.end();
}

std::string Database::keys(){
    std::lock_guard<std::mutex> lock(mutex);

    std::string result;

    for (const auto& pair : data){
        result += pair.first + "\n";
    }

    if (result.empty()){
        return "(empty)";
    }
    return result;
}