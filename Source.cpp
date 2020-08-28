#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "BVH.h"
#include<GL/glut.h>

//Просто парсер, который заполняет структуру, делал по документации вордовой

Tree BuildTree(const char* file) {
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
    BvhNodeTree* root = tree.createTree(NodeArr[0], nullptr, false);

    //std::cout <<std::endl<< std::endl << *root.child0 << *root.child1 << *root.child0->child0;

    tree.drawTree(root);
    std::cout << std::endl << std::endl << std::endl;
    for (size_t i = 0; i < tree.leafArr.size(); i++)
    {
        std::cout << *tree.leafArr[i] << std::endl;
    }
    
    

    

    return tree;
}

float* findNormal(BvhNodeTree* coords) {
    
}

void drawGeometry(Tree tree)
{
    for (size_t i = 0; i < tree.leafArr.size(); i++)
    {
        glBegin(GL_TRIANGLES);
        glNormal3fv(&findNormal(tree.leafArr[i])[0]);

        glVertex3f(tree.leafArr[i]->aabb0_min_or_v0.vec[0], tree.leafArr[i]->aabb0_min_or_v0.vec[1], tree.leafArr[i]->aabb0_min_or_v0.vec[2]);
        glVertex3f(tree.leafArr[i]->aabb0_max_or_v1.vec[0], tree.leafArr[i]->aabb0_min_or_v0.vec[1], tree.leafArr[i]->aabb0_min_or_v0.vec[2]);
        glVertex3f(tree.leafArr[i]->aabb1_min_or_v2.vec[0], tree.leafArr[i]->aabb0_min_or_v0.vec[1], tree.leafArr[i]->aabb0_min_or_v0.vec[2]);
        glEnd();
    }
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawGeometry(BuildTree("C:\\Users\\dimon\\Desktop\\task_description\\binary_dump\\cornell_box_bvh2.bin"));
    glutSwapBuffers();
}

void init()
{
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutCreateWindow("AMD test task");
    glutDisplayFunc(display);
    init();
    glutMainLoop();
    return 0;
}