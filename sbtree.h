#ifndef B_TREE_H
#define B_TREE_H

#include <vector>
#include <algorithm>
#include <iostream>
#include <memory>

class Node
{
public:
    bool is_leaf;
    std::vector<int> keys;
    std::vector<std::unique_ptr<Node>> children;
    Node(bool is_leaf);
    ~Node();
};

Node::Node(bool is_leaf_)
{
    is_leaf = is_leaf_;
}

Node::~Node() = default;

class BTree
{
public:
    int ORDER;
    std::unique_ptr<Node> root;
    BTree(int ORDER_);
    ~BTree();
    void insert(int key);
    void insert_non_full(Node *node, int key);
    void split_child(Node *node, int index);
    void print_tree();
    void print_helper(Node *node);
};

BTree::BTree(int ORDER_)
{
    ORDER = ORDER_;
    root = std::make_unique<Node>(true);
}

BTree::~BTree() = default;

void BTree::insert(int key)
{
    if (root->keys.size() == ORDER - 1)
    {
        // root full, so split
        std::unique_ptr<Node> new_root = std::make_unique<Node>(false);
        new_root->children.push_back(std::move(root));
        split_child(new_root.get(), 0);
        root = std::move(new_root);
    }
    insert_non_full(root.get(), key);
}

void BTree::split_child(Node *node, int index)
{
    int medianIndex = ORDER / 2 - 1;
    int medianValue = node->children[index]->keys[medianIndex];
    std::unique_ptr<Node> new_left = std::make_unique<Node>(node->children[index]->is_leaf);
    std::unique_ptr<Node> new_right = std::make_unique<Node>(node->children[index]->is_leaf);
    for (int i = 0; i < medianIndex; i++)
    {
        new_left->keys.push_back(node->children[index]->keys[i]);
    }
    for (int i = medianIndex + 1; i < node->children[index]->keys.size(); i++)
    {
        new_right->keys.push_back(node->children[index]->keys[i]);
    }
    if (!node->children[index]->is_leaf)
    {
        for (int i = 0; i <= medianIndex; i++)
        {
            new_left->children.push_back(std::move(node->children[index]->children[i]));
        }
        for (int i = medianIndex + 1; i < node->children[index]->children.size(); i++)
        {
            new_right->children.push_back(std::move(node->children[index]->children[i]));
        }
    }
    node->keys.insert(node->keys.begin() + index, medianValue);
    node->children.insert(node->children.begin() + index + 1, std::move(new_right));
    node->children[index] = std::move(new_left);
}

void BTree::insert_non_full(Node *node, int key)
{
    if (node->is_leaf)
    {
        node->keys.push_back(key);
        std::sort(node->keys.begin(), node->keys.end());
    }
    else
    {
        int i = node->keys.size() - 1;
        while (i >= 0 && key < node->keys[i])
        {
            i--;
        }
        i++;
        if (node->children[i]->keys.size() == ORDER - 1)
        {
            split_child(node, i);
            if (node->keys[i] < key)
            {
                i++;
            }
        }
        insert_non_full(node->children[i].get(), key);
    }
}

void BTree::print_helper(Node *node)
{
    for (int i = 0; i < node->keys.size(); i++)
    {
        std::cout << node->keys.at(i) << " ";
    }
    std::cout << std::endl;
    for (int i = 0; i < node->children.size(); i++)
    {
        if (node->children[i] != nullptr)
        {
            print_helper(node->children[i].get());
        }
    }
}

void BTree::print_tree()
{
    print_helper(root.get());
}

#endif