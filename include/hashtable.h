#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <vector>
#include <string>

template<typename Value>
struct Bucket
{
    std::string key;
    Value value;

    bool occupied = false;
    bool deleted = false;
};

template<typename Value>
class HashTable
{
public:
    HashTable(size_t cap = 16) : currentSize(0), capacity(cap){
        table.resize(capacity);
    }

    bool insert(const std::string& key, const Value& value){
        if (static_cast<double>(currentSize + 1) / capacity > 0.7){
            rehash();
        }

        int index = findInsertIndex(key);
        if (index == -1){
            return false;          // Later: rehash() and try again
        }

        Bucket<Value>& bucket = table[index];

        if (!bucket.occupied){
            currentSize++;
        }

        bucket.key = key;
        bucket.value = value;
        bucket.occupied = true;
        bucket.deleted = false;

        return true;
    }

    Value* get(const std::string& key){
        int index = findIndex(key);
        if (index == -1) return nullptr;

        return &table[index].value;
    }

    bool contains(const std::string& key) const{
        return findIndex(key) != -1;
    }

    bool erase(const std::string& key){
       int index = findIndex(key);
       if(index == -1) return false;

       table[index].occupied = false;
       table[index].deleted = true;
       currentSize--;
       return true;
    }

    void clear(){
        table.clear();
        table.resize(capacity);
        currentSize = 0;
    }

    std::vector<std::pair<std::string, Value>> entries() const{
        std::vector<std::pair<std::string, Value>> result;

        for (const auto& bucket : table){
            if (bucket.occupied){
                result.push_back({bucket.key, bucket.value});
            }
        }

        return result;
    }

    size_t size() const{
        return currentSize;
    }

private:
    std::vector<Bucket<Value>> table;
    size_t currentSize;
    size_t capacity ;

    // FNV-1a hash fxn
   size_t hash(const std::string& key) const{
        return hash(key, capacity);
    }

    size_t hash(const std::string& key, size_t cap) const{
        const size_t OFFSET = 14695981039346656037ULL;
        const size_t PRIME = 1099511628211ULL;

        size_t hashValue = OFFSET;

        for (char c : key)
        {
            hashValue ^= static_cast<size_t>(c);
            hashValue *= PRIME;
        }

        return hashValue % cap;
    }

    int findIndex(const std::string& key) const{
        size_t start = hash(key);

        for (size_t i = 0; i < capacity; i++){
            size_t index = (start + i) % capacity;

            const Bucket<Value>& bucket = table[index];

            // Never used bucket → key doesn't exist
            if (!bucket.occupied && !bucket.deleted){
                return -1;
            }

            // Found the key
            if (bucket.occupied && bucket.key == key){
                return static_cast<int>(index);
            }
        }

        return -1;
    }

    int findInsertIndex(const std::string& key) const{
        size_t start = hash(key);
        int firstDeleted = -1;

        for (size_t i = 0; i < capacity; i++){
            size_t index = (start + i) % capacity;
            const Bucket<Value>& bucket = table[index];

            // 1. Key already exists
            if (bucket.occupied && bucket.key == key){
                return static_cast<int>(index);
            }

            // 2. Deleted bucket
            if (bucket.deleted && firstDeleted == -1){
                firstDeleted = static_cast<int>(index);
            }

            // 3. Never-used bucket
            if (!bucket.occupied && !bucket.deleted){
                if (firstDeleted != -1) return firstDeleted;
                
                return static_cast<int>(index);
            }
        }

        return firstDeleted;
    }

    void insertIntoTable(std::vector<Bucket<Value>>& targetTable,size_t targetCapacity,
                    const std::string& key, const Value& value){
        size_t start = hash(key, targetCapacity);

        for (size_t i = 0; i < targetCapacity; i++){
            size_t index = (start + i) % targetCapacity;

            Bucket<Value>& bucket = targetTable[index];

            if (!bucket.occupied){
                bucket.key = key;
                bucket.value = value;
                bucket.occupied = true;
                bucket.deleted = false;
                return;
            }
        }
    }

    void rehash(){
        size_t newCapacity = capacity * 2;
        std::vector<Bucket<Value>> newTable(newCapacity);

        for (const auto& bucket : table){
            if (bucket.occupied){
                insertIntoTable(newTable,newCapacity,bucket.key,bucket.value);
            }
        }

        table = std::move(newTable);
        capacity = newCapacity;
    }

    double loadFactor() const{
        return static_cast<double>(currentSize) / capacity;
    }


};



#endif