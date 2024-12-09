#include "btree.h"
#include <iostream>
#include <vector>

int main()
{
    BTree *tree = new BTree(4);
    for (int i = 1; i < 11; i++)
    {
        tree->insert(2 * i);
    }
    tree->print_tree();
    for (int i = 0; i < 21; i++)
    {
        std::cout << i << " is " << (tree->search(i) ? "in the tree" : "not in the tree") << std::endl;
    }
}