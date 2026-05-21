#include <iostream>
#include "../include/hashtable.h"

int main(){
 HashTable<int> ht;

if (ht.get("xyz") == nullptr)
{
    std::cout << "Not Found\n";
}
}