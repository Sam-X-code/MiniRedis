#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <mutex>
#include <unordered_map>

class Database
{
public:
    void set(const std::string& key, const std::string& value);
    std::string get(const std::string& key);
    bool del(const std::string& key);
    bool exists(const std::string& key);
    std::string keys();

private:
    std::unordered_map<std::string, std::string> data;
    std::mutex mutex;
};

#endif