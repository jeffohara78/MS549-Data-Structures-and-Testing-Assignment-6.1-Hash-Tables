#pragma once

#ifndef MYHASHTABLE_H
#define MYHASHTABLE_H

#include <vector>
#include <list>
#include <functional>
#include <iostream>

using namespace std;

template<typename K, typename V>
class MyHashTable {
private:
    struct KeyValue {
        K key;
        V value;
        KeyValue(const K& k, const V& v) : key(k), value(v) {}
    };

    // Hash table is a vector of lists for separate chaining
    vector<list<KeyValue>> table;
    size_t numElements;

    // Compute which bucket to use by hashing the key and taking modulus
    size_t getBucketIndex(const K& key) const {
        return std::hash<K>()(key) % table.size();
    }

public:
    MyHashTable(size_t size = 101) : table(size), numElements(0) {}

    // Insert a new key-value pair or update if key already exists
    void insert(const K& key, const V& value) {
        size_t index = getBucketIndex(key);
        // Check if key already in this bucket; if so, update
        for (auto& kv : table[index]) {
            if (kv.key == key) {
                kv.value = value;
                return;
            }
        }
        // If not found, append new KeyValue to the chain
        table[index].push_back(KeyValue(key, value));
        numElements++;
    }

    // Retrieve a pointer to the value associated with the given key
    // Returns nullptr if the key isn't found
    V* retrieve(const K& key) {
        size_t index = getBucketIndex(key);
        // Search the chain at the computed bucket
        for (auto& kv : table[index]) {
            if (kv.key == key) {
                return &kv.value;
            }
        }
        return nullptr; // Not found
    }

    // Remove the key-value pair with the given key
    // Return true if removed, false if not found
    bool remove(const K& key) {
        size_t index = getBucketIndex(key);
        for (auto it = table[index].begin(); it != table[index].end(); ++it) {
            if (it->key == key) {
                // Erase the key-value from the chain
                table[index].erase(it);
                numElements--;
                return true;
            }
        }
        return false; // Key not in table
    }

    // How many key-value pairs are in the table
    size_t size() const {
        return numElements;
    }

    // How many buckets/chains does the table have
    size_t bucketCount() const {
        return table.size();
    }
};

#endif
