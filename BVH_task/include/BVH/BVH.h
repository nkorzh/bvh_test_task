#pragma once
#include <cstdint>
#include <iostream>
#include <stack>
#include <vector>
#include <Math/Vec.h>

using namespace std;


//
struct BvhNode
{
    uint32_t child0;
    uint32_t child1;
    uint32_t parent;
    uint32_t update;

    float3 aabb0_min_or_v0;
    float3 aabb0_max_or_v1;
    float3 aabb1_min_or_v2;
    float3 aabb1_max_or_v3;
    friend std::ostream& operator << (std::ostream& out, const BvhNode& node) {
        out << node.child0 << std::endl << node.child1 << std::endl << node.parent
            << std::endl << node.update << std::endl << node.aabb0_min_or_v0 << std::endl << node.aabb0_max_or_v1 << std::endl
            << node.aabb1_min_or_v2 << std::endl << node.aabb1_max_or_v3 << std::endl << std::endl;
        return out;
    }
};

//
struct BvhNodeTree
{
    BvhNodeTree();

    BvhNodeTree(uint32_t update, float3 aabb0_min_or_v0, float3 aabb0_max_or_v1,
        float3 aabb1_min_or_v2, float3 aabb1_max_or_v3, BvhNodeTree* child0, BvhNodeTree* child1, BvhNodeTree* parent, uint32_t index, bool leaf) {

        this->aabb0_min_or_v0 = aabb0_min_or_v0;
        this->aabb0_max_or_v1 = aabb0_max_or_v1;
        this->aabb1_min_or_v2 = aabb1_min_or_v2;
        this->aabb1_max_or_v3 = aabb1_max_or_v3;
        this->child0 = child0;
        this->child1 = child1;
        this->parent = parent;
        this->update = update;
        this->index = index;
        this->leafFlag = leaf;
    }
    BvhNodeTree* child0;  //
    BvhNodeTree* child1;  //
                          //
    BvhNodeTree* parent;
    uint32_t update;      //
    uint32_t index;       //


    float3 aabb0_min_or_v0;  //
    float3 aabb0_max_or_v1;  //
    float3 aabb1_min_or_v2;  //
    float3 aabb1_max_or_v3;

    bool leafFlag;

    //
    friend std::ostream& operator << (std::ostream& out, BvhNodeTree node) {
        out << "++++" << node.index << "++++" << std::endl << node.aabb0_min_or_v0 << std::endl << node.aabb0_max_or_v1 << std::endl
            << node.aabb1_min_or_v2 << std::endl << node.aabb1_max_or_v3 << std::endl << std::endl;
        return out;
    }
};

//
class Tree {
public:
    Tree(BvhNode* BvhArray)
    {
        //
        this->BvhArray = BvhArray;
    };

    ~Tree()
    {
        //Destroy_Tree();
    };

    //
    BvhNodeTree* createTree(BvhNode item, BvhNodeTree* last, bool isLeft);
    //
    void drawTree(const BvhNodeTree* root);

    void Destroy_Tree();

    //
    BvhNodeTree* getNodeByIndex(uint32_t item);

    std::vector<BvhNodeTree*> leafArr;
    BvhNodeTree* root = nullptr;

private:
    //BvhNodeTree* root = new BvhNodeTree(0u, float3(), float3(), float3(), float3(), nullptr, nullptr, nullptr, 0, false);
    void destroy_tree(BvhNodeTree* leaf);
    BvhNode* BvhArray;
    const long long leaf = 4294967295;
};