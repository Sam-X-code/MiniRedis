#include "database.h"
#include <fstream>

void Database::addToFront(Node* node){
    node->prev = nullptr;
    node->next = head;

    if (head != nullptr){
        head->prev = node;
    }

    head = node;

    if (tail == nullptr){
        tail = node;
    }
}

void Database::removeNode(Node* node){
    if (node->prev != nullptr){
        node->prev->next = node->next;
    }
    else{
        head = node->next;
    }

    if (node->next != nullptr){
        node->next->prev = node->prev;
    }
    else{
        tail = node->prev;
    }

    node->prev = nullptr;
    node->next = nullptr;
}

void Database::moveToFront(Node* node){
    if (node == head) return;

    removeNode(node);
    addToFront(node);
}

void Database::removeTail(){
    if (tail == nullptr) return;

    Node* node = tail;
    removeNode(node);
    data.erase(node->key);
    delete node;
}

// set.....................
void Database::set(const std::string& key, const std::string& value){
    std::lock_guard<std::mutex> lock(mutex);

    auto it = data.find(key);

    // Key already exists
    if (it != data.end()){
        Node* node = it->second;
        node->value.data = value;
        moveToFront(node);
        return;
    }

    // Cache full?
    if (data.size() >= capacity){
        removeTail();
    }

    Value entry;
    entry.data = value;

    Node* node = new Node(key, entry);
    addToFront(node);
    data[key] = node;
}

// get.........................
std::string Database::get(const std::string& key){
    std::lock_guard<std::mutex> lock(mutex);

    auto it = data.find(key);

    if (it == data.end()){
        return "(nil)";
    }

    Node* node = it->second;

    if (node->value.hasExpiry &&
        std::chrono::steady_clock::now() > node->value.expiry){
        removeNode(node);
        data.erase(key);
        delete node;
        return "(nil)";
    }

    moveToFront(node);
    
    return node->value.data;
}

// del.........................
bool Database::del(const std::string& key){
    std::lock_guard<std::mutex> lock(mutex);

    auto it = data.find(key);

    if (it == data.end()){
        return false;
    }

    Node* node = it->second;
    removeNode(node);
    data.erase(it);
    delete node;
    return true;
}

// exist........................
bool Database::exists(const std::string& key)
{
    std::lock_guard<std::mutex> lock(mutex);

    auto it = data.find(key);

    if (it == data.end()){
        return false;
    }

    Node* node = it->second;

    if (node->value.hasExpiry &&
        std::chrono::steady_clock::now() > node->value.expiry){
        removeNode(node);
        data.erase(it);
        delete node;
        return false;
    }

    return true;
}

std::string Database::keys(){
    std::lock_guard<std::mutex> lock(mutex);

    std::string result;

    auto now = std::chrono::steady_clock::now();

    for (auto it = data.begin(); it != data.end(); ){
        Node* node = it->second;

        if (node->value.hasExpiry &&
            now > node->value.expiry){
            removeNode(node);
            delete node;
            it = data.erase(it);
        }
        else{
            result += it->first + "\n";
            ++it;
        }
    }

    return result.empty() ? "(empty)" : result;
}

// expire.......................
bool Database::expire(const std::string& key, int seconds){
    std::lock_guard<std::mutex> lock(mutex);

    auto it = data.find(key);

    if (it == data.end()){
        return false;
    }

    Node* node = it->second;
    node->value.hasExpiry = true;

    node->value.expiry =
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
        Node* node = pair.second;

        file << node->key << " " << node->value.data << "\n";
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

    // Clear existing data
    while (head != nullptr){
        Node* temp = head;
        head = head->next;
        delete temp;
    }

    tail = nullptr;
    data.clear();

    std::string key;
    std::string value;

    while (file >> key >> value){
        Value entry;
        entry.data = value;

        Node* node = new Node(key, entry);
        addToFront(node);

        data[key] = node;
    }

    return true;
}

Database::~Database(){
    while (head != nullptr){
        Node* temp = head;
        head = head->next;
        delete temp;
    }
}