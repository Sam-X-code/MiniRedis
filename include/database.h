#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <mutex>
#include "hashtable.h"
#include <chrono>

struct Value
{
    std::string data;

    std::chrono::steady_clock::time_point expiry;

    bool hasExpiry = false;
};

struct Node
{
    std::string key;
    Value value;

    Node* prev;
    Node* next;

    Node(const std::string& k,const Value& v): key(k), value(v), prev(nullptr),next(nullptr) {}
};

class Database
{
public:
    ~Database();
    void set(const std::string& key, const std::string& value);
    std::string get(const std::string& key);
    bool del(const std::string& key);
    bool exists(const std::string& key);
    bool expire(const std::string& key, int seconds);
    std::string keys();

    bool save(const std::string& filename);
    bool load(const std::string& filename);

    Node* head = nullptr;
    Node* tail = nullptr;
    size_t capacity = 3;

private:
    HashTable<Node*> data;
    std::mutex mutex;

    void addToFront(Node* node);
    void removeNode(Node* node);
    void moveToFront(Node* node);
    void removeTail();
};

#endif