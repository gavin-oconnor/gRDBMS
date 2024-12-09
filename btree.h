#ifndef B_TREE_H
#define B_TREE_H

#include <vector>
#include <algorithm>
#include <iostream>

class Node
{
public:
    bool is_leaf;
    std::vector<int> keys;
    std::vector<Node *> children;
    Node(bool is_leaf);
    ~Node();
};

Node::Node(bool is_leaf_)
{
    is_leaf = is_leaf_;
}

Node::~Node()
{
}

class BTree
{
public:
    int ORDER;
    Node *root;
    BTree(int ORDER_);
    ~BTree();
    void insert(int key);
    void insert_non_full(Node *node, int key);
    void split_child(Node *node, int index);
    void print_tree();
    void print_helper(Node *node);
    bool search(int key);
    bool search_helper(Node *node, int key);
};

BTree::BTree(int ORDER_)
{
    ORDER = ORDER_;
    root = new Node(true);
}

BTree::~BTree()
{
    delete root;
}

void BTree::insert(int key)
{
    if (root->keys.size() == ORDER - 1)
    {
        // root full, so split
        Node *new_root = new Node(false);
        new_root->children.push_back(root);
        split_child(new_root, 0);
        root = new_root;
    }
    insert_non_full(root, key);
}

void BTree::split_child(Node *node, int index)
{
    int medianIndex = ORDER / 2 - 1;
    int medianValue = node->children[index]->keys[medianIndex];
    Node *new_left = new Node(node->children[index]->is_leaf);
    Node *new_right = new Node(node->children[index]->is_leaf);
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
            new_left->children.push_back(node->children[index]->children[i]);
        }
        for (int i = medianIndex + 1; i < node->children[index]->children.size(); i++)
        {
            new_right->children.push_back(node->children[index]->children[i]);
        }
    }
    delete node->children[index];
    node->keys.insert(node->keys.begin() + index, medianValue);
    node->children.insert(node->children.begin() + index + 1, new_right);
    node->children[index] = new_left;
    // node->keys.push_back(-1);
    // node->children.push_back(nullptr);
    // for (int i = index; i < node->keys.size() - 1; i++)
    // {
    //     node->keys[i + 1] = node->keys[i];
    // }
    // for (int i = index + 1; i < node->children.size() - 1; i++)
    // {
    //     node->children[i + 1] = node->children[i];
    // }
    // node->keys[index] = medianValue;
    // node->children[index + 1] = new_right;
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
        insert_non_full(node->children[i], key);
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
            print_helper(node->children[i]);
        }
    }
}

void BTree::print_tree()
{
    print_helper(root);
}

bool BTree::search(int key)
{
    return search_helper(root, key);
}

bool BTree::search_helper(Node *node, int key)
{
    int left = 0;
    int right = node->keys.size() - 1;
    if (key < node->keys[left])
    {
        if (node->is_leaf || node->children.size() == 0)
        {
            return false;
        }
        return search_helper(node->children[0], key);
    }
    if (key > node->keys[right])
    {
        if (node->is_leaf || node->children.size() <= right)
        {
            return false;
        }
        return search_helper(node->children[right + 1], key);
    }
    while (left <= right)
    {
        int mid = (right + left) / 2;
        if (node->keys[mid] == key)
        {
            return true;
        }
        if (!node->is_leaf && mid + 1 <= right && node->keys[mid] < key && key < node->keys[mid + 1])
        {
            return search_helper(node->children[mid + 1], key);
        }
        if (!node->is_leaf && mid - 1 >= 0 && node->keys[mid] > key && key > node->keys[mid - 1])
        {
            return search_helper(node->children[mid - 1], key);
        }
        if (node->keys[mid] > key)
        {
            right = mid - 1;
        }
        else
        {
            left = mid + 1;
        }
    }
    return false;
}

#endif