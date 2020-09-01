#define _CRT_SECURE_NO_WARNINGS
#define _x1_ coords->aabb0_min_or_v0.vec[0]
#define _y1_ coords->aabb0_min_or_v0.vec[1]
#define _z1_ coords->aabb0_min_or_v0.vec[2]
#define _x2_ coords->aabb0_max_or_v1.vec[0]
#define _y2_ coords->aabb0_max_or_v1.vec[1]
#define _z2_ coords->aabb0_max_or_v1.vec[2]
#define _x3_ coords->aabb1_min_or_v2.vec[0]
#define _y3_ coords->aabb1_min_or_v2.vec[1]
#define _z3_ coords->aabb1_min_or_v2.vec[2]


#include <iostream>
#include "BVH.h"
#include<GL/glut.h>


GLfloat n[6][3] = {  /* Normals for the 6 faces of a cube. */
  {-1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {1.0, 0.0, 0.0},
  {0.0, -1.0, 0.0}, {0.0, 0.0, 1.0}, {0.0, 0.0, -1.0} };
GLint faces[6][4] = {  /* Vertex indices for the 6 faces of a cube. */
  {0, 1, 2, 3}, {3, 2, 6, 7}, {7, 6, 5, 4},
  {4, 5, 1, 0}, {5, 6, 2, 1}, {7, 4, 0, 3} };
GLfloat v[8][3];  /* Will be filled in with X,Y,Z vertexes. */


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

    /*std::cout << "------" << treesizeBytes << "--------"<< std::endl;*/

    size_t treesize = treesizeBytes/64;

    BvhNode* NodeArr = new BvhNode[treesize];
    for (size_t i = 0; i < treesize; i++)
    {
        fread(&NodeArr[i], sizeof(BvhNode), treesize, Dumbs);
    }

    /*for (size_t i = 0; i < treesize; i++)
    {
        std::cout<< "-------" << i << "--------" << std::endl << NodeArr[i];
    }*/
    Tree tree = Tree(NodeArr);
    BvhNodeTree* root = tree.createTree(NodeArr[0], nullptr, false);

    //std::cout <<std::endl<< std::endl << *root.child0 << *root.child1 << *root.child0->child0;

    tree.drawTree(root);
    /*std::cout << std::endl << std::endl << std::endl;
    for (size_t i = 0; i < tree.leafArr.size(); i++)
    {
        std::cout << *tree.leafArr[i] << std::endl;
    }*/
    return tree;
}

GLfloat* findNormal(BvhNodeTree* coords) {
    
    float3 u = coords->aabb0_max_or_v1 - coords->aabb0_min_or_v0;
    float3 v = coords->aabb1_min_or_v2 - coords->aabb0_min_or_v0;
    GLfloat normal[3];
    normal[0] = (u.vec[1] * v.vec[2]) - (u.vec[2] * v.vec[1]);
    normal[1] = (u.vec[2] * v.vec[0]) - (u.vec[0] * v.vec[2]);
    normal[2] = (u.vec[0] * v.vec[1]) - (u.vec[1] * v.vec[0]);
    return normal;
    /*
    GLfloat normal[3];

    normal[0] = _y1_ * _z3_ - _y2_ * _z3_ - _y1_ * _z2_ - _z1_ * _y3_ + _z2_ * _y3_ + _z1_ * _y2_;
    normal[1] = _z1_ * _x3_ - _z2_ * _x3_ - _z1_ * _x2_ - _x1_ * _z3_ + _x2_ * _z3_ + _x1_ * _z2_;
    normal[2] = _x1_ * _y3_ - _x2_ * _y3_ - _x1_ * _y2_ - _y1_ * _x3_ + _y2_ * _x3_ + _y1_ * _x2_;

    return normal;*/
}

void drawGeometry(Tree tree)
{
    glClearColor(0.7f, 1.0f, 0.7f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    for (size_t i = 0; i < tree.leafArr.size(); i++)
    {
        glBegin(GL_TRIANGLES);
        glNormal3fv(&findNormal(tree.leafArr[i])[0]);
        glVertex3f(tree.leafArr[i]->aabb0_min_or_v0.vec[0], tree.leafArr[i]->aabb0_min_or_v0.vec[1], tree.leafArr[i]->aabb0_min_or_v0.vec[2]);
        glVertex3f(tree.leafArr[i]->aabb0_max_or_v1.vec[0], tree.leafArr[i]->aabb0_max_or_v1.vec[1], tree.leafArr[i]->aabb0_max_or_v1.vec[2]);
        glVertex3f(tree.leafArr[i]->aabb1_min_or_v2.vec[0], tree.leafArr[i]->aabb1_min_or_v2.vec[1], tree.leafArr[i]->aabb1_min_or_v2.vec[2]);
        glEnd();
    }

   /* for (int i = 0; i < 6; i++) {
        glBegin(GL_QUADS);
        glEnable(GL_NORMALIZE);
        glNormal3fv(&(n[i][0]));
        glVertex3fv(&v[faces[i][0]][0]);
        glVertex3fv(&v[faces[i][1]][0]);
        glVertex3fv(&v[faces[i][2]][0]);
        glVertex3fv(&v[faces[i][3]][0]);
        glEnd();
    }*/
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawGeometry(BuildTree("C:\\Users\\dimon\\Desktop\\task_description\\binary_dump\\cornell_box_bvh2.bin"));
    glutSwapBuffers();
}

void init()
{

    v[0][0] = v[1][0] = v[2][0] = v[3][0] = -1;
    v[4][0] = v[5][0] = v[6][0] = v[7][0] = 1;
    v[0][1] = v[1][1] = v[4][1] = v[5][1] = -1;
    v[2][1] = v[3][1] = v[6][1] = v[7][1] = 1;
    v[0][2] = v[3][2] = v[4][2] = v[7][2] = 1;
    v[1][2] = v[2][2] = v[5][2] = v[6][2] = -1;

    GLfloat light_diffuse[] = { 1.0, 0.0, 0.0, 1.0 };  /* Red diffuse light. */
    GLfloat light_position[] = { 1.0f, 1.0f, 1.0f, 0.0f };

    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    glEnable(GL_NORMALIZE);
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    gluPerspective( /* field of view in degree */ 40.0,
        /* aspect ratio */ 1.0,
        /* Z near */ 1.0, /* Z far */ 10.0);
    glMatrixMode(GL_MODELVIEW);
    gluLookAt(0.0, 0.0, 5.0,  /* eye is at (0,0,5) */
        0.0, 0.0, 0.0,      /* center is at (0,0,0) */
        0.0, 1.0, 0.0);      /* up is in positive Y direction */

      /* Adjust cube position to be asthetic angle. */
    glTranslatef(0.0, -1.0, -1.0);
    glRotatef(0, 1.0, 0.0, 0.0);
    glRotatef(20, 0.0, 1.0, 0.0);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 800);
    glutCreateWindow("AMD test task");
    glutDisplayFunc(display);
    init();
    glutMainLoop();
    return 0;
}