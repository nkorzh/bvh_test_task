#pragma once
#include <cstdint>
#include <iostream>

struct float3
{
    float3() {
        vec[0] = 0;
        vec[1] = 0;
        vec[2] = 0;
    }

    float vec[3];
    friend std::ostream& operator<< (std::ostream& out, const float3& point) {
       out << point.vec[0] << ", " << point.vec[1] << ", " << point.vec[2] << ", ";
       return out;
    }
};

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

struct BvhNodeTree
{   
    BvhNodeTree(uint32_t update, float3 aabb0_min_or_v0, float3 aabb0_max_or_v1,
        float3 aabb1_min_or_v2, float3 aabb1_max_or_v3, BvhNodeTree* child0, BvhNodeTree*child1, BvhNodeTree* parent, uint32_t index) {
        this->aabb0_min_or_v0 = aabb0_min_or_v0;
        this->aabb0_max_or_v1 = aabb0_max_or_v1;
        this->aabb1_min_or_v2 = aabb1_min_or_v2;
        this->aabb1_max_or_v3 = aabb1_max_or_v3;
        this->child0 = child0;
        this->child1 = child1;
        this->parent = parent;
        this->update = update;
        this->index = index;
    }
    BvhNodeTree* child0;
    BvhNodeTree* child1;
    BvhNodeTree* parent;
    uint32_t update;
    uint32_t index;


    float3 aabb0_min_or_v0;
    float3 aabb0_max_or_v1;
    float3 aabb1_min_or_v2;
    float3 aabb1_max_or_v3;

    friend std::ostream& operator << (std::ostream& out, BvhNodeTree node) {
        out << "++++" << node.index << "++++" << std::endl << node.aabb0_min_or_v0 << std::endl << node.aabb0_max_or_v1 << std::endl
            << node.aabb1_min_or_v2 << std::endl << node.aabb1_max_or_v3 << std::endl << std::endl;
        return out;
    }
};

class Tree {
public: 
    Tree(BvhNode* BvhArray)
    {
        //root = NULL;
        this->BvhArray = BvhArray;
    };
    ~Tree()
    {
        //destroy_tree();
    };

    BvhNodeTree* createTree(BvhNode item) {
        if (item.child0 == 4294967295) {
            return new BvhNodeTree(item.update, item.aabb0_min_or_v0, item.aabb0_max_or_v1, item.aabb1_min_or_v2, item.aabb1_max_or_v3, nullptr, nullptr, nullptr, BvhArray[item.child1].parent);
        }
        BvhNode left = BvhArray[item.child0];
        BvhNode right = BvhArray[item.child1];
        BvhNodeTree* rootNode = new BvhNodeTree(item.update, item.aabb0_min_or_v0, item.aabb0_max_or_v1, item.aabb1_min_or_v2, item.aabb1_max_or_v3, createTree(left), createTree(right), nullptr, BvhArray[item.child1].parent);
        return rootNode;
    }
    void drawTree(const BvhNodeTree* root);
    void destroy_tree();
private:
    BvhNodeTree* root = new BvhNodeTree(0u, float3(), float3(), float3(), float3(), nullptr, nullptr, nullptr, 0);
    void destroy_tree(BvhNodeTree* leaf);
    BvhNode* BvhArray;
};

