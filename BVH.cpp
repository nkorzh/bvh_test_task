#include "BVH.h"

void Tree::drawTree(const BvhNodeTree* root)
{
    if (root != nullptr) { 

        std::cout << *root << std::endl; 
        drawTree(root->child0); 
        drawTree(root->child1); 
    }
}

void Tree::destroy_tree()
{
    destroy_tree(root);
}

void Tree::destroy_tree(BvhNodeTree* leaf)
{
    if (leaf != nullptr)				
    {						
        destroy_tree(leaf->child0);	
        destroy_tree(leaf->child1);  	
        delete leaf;			
    }
}
