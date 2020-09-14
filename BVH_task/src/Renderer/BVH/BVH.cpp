//Author: Elagin Dmitrii

#include <BVH/BVH.h>

BvhNodeTree* Tree::createTree(BvhNode item, BvhNodeTree* last, bool isLeft) {
        if (item.child0 == 4294967295)
        {
            uint32_t ind = isLeft ? BvhArray[last->index].child0 : BvhArray[last->index].child1;
            BvhNodeTree* buff = new BvhNodeTree(item.update, item.aabb0_min_or_v0, item.aabb0_max_or_v1, item.aabb1_min_or_v2, item.aabb1_max_or_v3,
                nullptr, nullptr, last, ind, true);
            leafArr.push_back(buff);
            return buff;
        }
        BvhNode left = BvhArray[item.child0];
        BvhNode right = BvhArray[item.child1];
        BvhNodeTree* rootNode = new BvhNodeTree(item.update, item.aabb0_min_or_v0, item.aabb0_max_or_v1, item.aabb1_min_or_v2, item.aabb1_max_or_v3,
            nullptr, nullptr, last, BvhArray[item.child1].parent, false);
        rootNode->child0 = createTree(left, rootNode, true);
        rootNode->child1 = createTree(right, rootNode, false);
        if (BvhArray[item.child1].parent == 0)
        {
            root = rootNode;
        }
        return rootNode;
}

void Tree::drawTree(const BvhNodeTree* root)
{
    if (root != nullptr)
    {

        std::cout << *root << std::endl;
        drawTree(root->child0);
        drawTree(root->child1);
    }
}

BvhNodeTree* Tree::getNodeByIndex(uint32_t item)
{
    BvhNodeTree* node = root;

    std::stack<BvhNodeTree*> s;
    s.push(node);
    while (!s.empty())
    {
        node = s.top();
        s.pop();
        if (node->index == item)
        {
            break;
            return node;
        }
        if (node->child0 != nullptr)
            s.push(node->child0);
        if (node->child1 != nullptr)
            s.push(node->child1);
    }
}

void Tree::destroy_tree(BvhNodeTree* leaf)
{
    if ((leaf != nullptr) && (leaf->child0 != nullptr))
    {
        destroy_tree(leaf->child0);
        destroy_tree(leaf->child1);
        delete leaf;
    }
}

/* Binary file .bin parser function.
 * Builds the tree by bv-hierarchy
 * RETURNS: Tree
 */
Tree BuildTree(const char* file) 
{
    FILE* Dumbs = fopen(file, "rb");
    if (Dumbs == nullptr)
    {
        fputs("Error", stderr);
        exit(1);
    }
    uint32_t treesizeBytes;
    fread(&treesizeBytes, sizeof(uint32_t), 1, Dumbs);

    size_t treesize = treesizeBytes / 64;

    BvhNode* NodeArr = new BvhNode[treesize];
    for (size_t i = 0; i < treesize; i++)
    {
        fread(&NodeArr[i], sizeof(BvhNode), treesize, Dumbs);
    }

    Tree tree = Tree(NodeArr);
    BvhNodeTree* root = tree.createTree(NodeArr[0], nullptr, false);

    //tree.drawTree(root);
   
    fclose(Dumbs);
    return tree;
}
