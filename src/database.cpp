#include "database.h"
#include <fstream>

// set.....................
void Database::set(const std::string& key , const std::string& value){
    std::lock_guard<std::mutex> lock(mutex);

    Value entry;
    entry.data = value;
    data[key] = entry;

}

// get.........................
std::string Database::get(const std::string& key){
    std::lock_guard<std::mutex> lock(mutex);

    auto it = data.find(key);

    if (it == data.end()){
        return "(nil)";
    }

    if (it->second.hasExpiry && std::chrono::steady_clock::now() > it->second.expiry){
        data.erase(it);
        return "(nil)";
    }

    return it->second.data;
}

// del.........................
bool Database::del(const std::string& key){
    std::lock_guard<std::mutex> lock(mutex);

    return data.erase(key) > 0;
}

// exist........................
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

// expire.......................
bool Database::expire(const std::string& key, int seconds){
    std::lock_guard<std::mutex> lock(mutex);

    auto it = data.find(key);

    if (it == data.end()) return false;

    it->second.hasExpiry = true;

    it->second.expiry =
        std::chrono::steady_clock::now() +
        std::chrono::seconds(seconds);

    return true;
}

// save to file .............................
bool Database::save(const std::string& filename){
    std::lock_guard<std::mutex> lock(mutex);

    std::ofstream file(filename);

    if (!file.is_open()){
        return false;
    }

    for (const auto& pair : data){
        file << pair.first << " "
             << pair.second.data << "\n";
    }

    return true;
}

// load from file ...........................
bool Database::load(const std::string& filename){
    std::lock_guard<std::mutex> lock(mutex);

    std::ifstream file(filename);

    if (!file.is_open()){
        return false;
    }

    data.clear();

    std::string key;
    std::string value;

    while (file >> key >> value){
        Value entry;
        entry.data = value;

        data[key] = entry;
    }

    return true;
}