#pragma once
#include <vector>
#include <memory>
#include <stdexcept>
#include <iostream>

template<typename KeyType, typename ValueType>
class LeafNode {
public:
    std::vector<KeyType> keys;
    std::vector<ValueType> values;
    std::shared_ptr<LeafNode> next;

    LeafNode() = default;
};

template<typename KeyType, typename ValueType>
class BPlusTree {
private:
    std::shared_ptr<LeafNode<KeyType, ValueType>> root;
    int order;

public:
    explicit BPlusTree(int order = 3) {
        root = std::make_shared<LeafNode<KeyType, ValueType>>();
        this->order = order;
    }

    void insert(KeyType key, ValueType value) {
        auto node = root;
        int pos = 0;
        while (pos < node->keys.size() && node->keys[pos] < key) pos++;

        node->keys.insert(node->keys.begin() + pos, key);
        node->values.insert(node->values.begin() + pos, value);

        if (node->keys.size() > order) {
            auto newLeaf = std::make_shared<LeafNode<KeyType, ValueType>>();
            int mid = node->keys.size() / 2;

            newLeaf->keys.assign(node->keys.begin() + mid, node->keys.end());
            newLeaf->values.assign(node->values.begin() + mid, node->values.end());

            node->keys.resize(mid);
            node->values.resize(mid);

            newLeaf->next = node->next;
            node->next = newLeaf;
        }
    }

    ValueType search(KeyType key) {
        auto node = root;
        while (node) {
            for (int i = 0; i < node->keys.size(); i++)
                if (node->keys[i] == key)
                    return node->values[i];
            node = node->next;
        }
        throw std::runtime_error("Key not found");
    }

    std::vector<ValueType> rangeSearch(KeyType start, KeyType end) {
        std::vector<ValueType> result;
        auto node = root;
        while (node) {
            for (int i = 0; i < node->keys.size(); i++) {
                if (node->keys[i] >= start && node->keys[i] <= end)
                    result.push_back(node->values[i]);
                else if (node->keys[i] > end)
                    return result;
            }
            node = node->next;
        }
        return result;
    }
};
