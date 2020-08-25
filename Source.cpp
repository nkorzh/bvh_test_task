#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "BVH.h"

//Просто парсер, который заполняет структуру, делал по документации вордовой

void BuildTree(const char* file) {
    FILE* Dumbs = fopen(file, "rb");
    if (Dumbs == nullptr)
    {
        fputs("Error", stderr);
        exit(1);
    }
    uint32_t treesizeBytes;
    fread(&treesizeBytes, sizeof(uint32_t), 1, Dumbs);

    std::cout << "------" << treesizeBytes << "--------"<< std::endl;

    size_t treesize = treesizeBytes/64;

    BvhNode* NodeArr = new BvhNode[treesize];
    for (size_t i = 0; i < treesize; i++)
    {
        fread(&NodeArr[i], sizeof(BvhNode), treesize, Dumbs);
    }

    for (size_t i = 0; i < treesize; i++)
    {
        std::cout<< "-------" << i << "--------" << std::endl << NodeArr[i];
    }
    Tree tree = Tree(NodeArr);
    BvhNodeTree* root = tree.createTree(NodeArr[0]);

    //std::cout <<std::endl<< std::endl << *root.child0 << *root.child1 << *root.child0->child0;

    tree.drawTree(root);
}
int main() {

    BuildTree("C:\\Users\\dimon\\Desktop\\task_description\\binary_dump\\cornell_box_bvh2.bin");

}