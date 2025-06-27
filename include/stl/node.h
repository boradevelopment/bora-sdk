#pragma once
// For maps
template<typename K, typename V>
struct MapNode {
    K key;
    V value;
    MapNode* left = nullptr;
    MapNode* right = nullptr;

    MapNode(const K& k, const V& v)
        : key(k), value(v), left(nullptr), right(nullptr) {}
};

// Dev Note: For vector and list, we use a doubly linked list structure.
template<typename T>
struct Node {
    T data;
    Node* next = nullptr;
    Node* prev = nullptr;
};
