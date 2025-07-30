#pragma once

#include "symbols/memory.h"
#include "stl/node.h"

namespace bora::stl {
/// @brief Basic Map implementation using a binary search tree.
/// @tparam K Key Type
/// @tparam V Value Type
template<typename K, typename V>
class map {
private:
    MapNode<K, V>* root = nullptr;

    // Allocate a node using bora::memory::allocate
    MapNode<K, V>* allocate_node(const K& key, const V& value) {
        void* mem = bora::memory::allocate(sizeof(MapNode<K, V>));
        if (!mem) return nullptr;
        return new MapNode<K, V>(key, value);
    }

    // Deallocate node using bora::memory::deallocate
    void deallocate_node(MapNode<K, V>* node) {
        if (node) {
            node->~MapNode<K, V>();
            bora::memory::deallocate(reinterpret_cast<u64>(node));
        }
    }

    // Recursive insert helper
    MapNode<K, V>* insert_recursive(MapNode<K, V>* node, const K& key, const V& value) {
        if (!node) return allocate_node(key, value);
        if (key < node->key)
            node->left = insert_recursive(node->left, key, value);
        else if (key > node->key)
            node->right = insert_recursive(node->right, key, value);
        else
            node->value = value; // overwrite existing key
        return node;
    }

    // Recursive find helper
    MapNode<K, V>* find_recursive(MapNode<K, V>* node, const K& key) const {
        if (!node) return nullptr;
        if (key == node->key) return node;
        if (key < node->key) return find_recursive(node->left, key);
        else return find_recursive(node->right, key);
    }

    // Recursive destroy helper
    void destroy_recursive(MapNode<K, V>* node) {
        if (!node) return;
        destroy_recursive(node->left);
        destroy_recursive(node->right);
        deallocate_node(node);
    }

public:
    map() = default;

    ~map() {
        destroy_recursive(root);
    }

    void insert(const K& key, const V& value) {
        root = insert_recursive(root, key, value);
    }

    V* find(const K& key) {
        MapNode<K, V>* node = find_recursive(root, key);
        return node ? &node->value : nullptr;
    }
};
} // namespace bora::stl
