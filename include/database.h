#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <mutex>
#include <unordered_map>
#include <chrono>

struct Value
{
    std::string data;

    std::chrono::steady_clock::time_point expiry;

    bool hasExpiry = false;
};

class Database
{
public:
    void set(const std::string& key, const std::string& value);
    std::string get(const std::string& key);
    bool del(const std::string& key);
    bool exists(const std::string& key);
    bool expire(const std::string& key, int seconds);
    std::string keys();

private:
    std::unordered_map<std::string, Value> data;
    std::mutex mutex;
};

#endif