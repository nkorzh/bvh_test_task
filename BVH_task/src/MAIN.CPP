/* Main.cpp
 * Author : Kozhukharov Nikita
 */

#define depth_to_draw 3 //

#include <iostream>
#include <GLFW/glfw3.h>
#include <BVH/BVH.h>
#include "Renderer/Renderer.h"
#include <Math/MathFunctions.h>

void drawBoundingBox(BvhNodeTree* root, int cur_depth, int target_depth)
{
    if ((root != nullptr) && (!root->leafFlag) && cur_depth == target_depth) {
        glBegin(GL_QUADS);

        
        glVertex3f(root->aabb0_min_or_v0.vec[0], root->aabb0_min_or_v0.vec[1], root->aabb0_min_or_v0.vec[2]);
        glVertex3f(root->aabb0_max_or_v1.vec[0], root->aabb0_min_or_v0.vec[1], root->aabb0_min_or_v0.vec[2]);
        glVertex3f(root->aabb0_max_or_v1.vec[0], root->aabb0_min_or_v0.vec[1], root->aabb0_max_or_v1.vec[2]);
        glVertex3f(root->aabb0_min_or_v0.vec[0], root->aabb0_min_or_v0.vec[1], root->aabb0_max_or_v1.vec[2]);

        glVertex3f(root->aabb0_max_or_v1.vec[0], root->aabb0_max_or_v1.vec[1], root->aabb0_max_or_v1.vec[2]);
        glVertex3f(root->aabb0_max_or_v1.vec[0], root->aabb0_max_or_v1.vec[1], root->aabb0_min_or_v0.vec[2]);
        glVertex3f(root->aabb0_min_or_v0.vec[0], root->aabb0_max_or_v1.vec[1], root->aabb0_min_or_v0.vec[2]);
        glVertex3f(root->aabb0_min_or_v0.vec[0], root->aabb0_max_or_v1.vec[1], root->aabb0_max_or_v1.vec[2]);

        glVertex3f(root->aabb0_max_or_v1.vec[0], root->aabb0_max_or_v1.vec[1], root->aabb0_max_or_v1.vec[2]);
        glVertex3f(root->aabb0_max_or_v1.vec[0], root->aabb0_max_or_v1.vec[1], root->aabb0_min_or_v0.vec[2]);
        glVertex3f(root->aabb0_max_or_v1.vec[0], root->aabb0_min_or_v0.vec[1], root->aabb0_min_or_v0.vec[2]);
        glVertex3f(root->aabb0_max_or_v1.vec[0], root->aabb0_min_or_v0.vec[1], root->aabb0_max_or_v1.vec[2]);

        glVertex3f(root->aabb0_max_or_v1.vec[0], root->aabb0_max_or_v1.vec[1], root->aabb0_max_or_v1.vec[2]);
        glVertex3f(root->aabb0_min_or_v0.vec[0], root->aabb0_max_or_v1.vec[1], root->aabb0_max_or_v1.vec[2]);
        glVertex3f(root->aabb0_min_or_v0.vec[0], root->aabb0_min_or_v0.vec[1], root->aabb0_max_or_v1.vec[2]);
        glVertex3f(root->aabb0_max_or_v1.vec[0], root->aabb0_min_or_v0.vec[1], root->aabb0_max_or_v1.vec[2]);

        glVertex3f(root->aabb0_min_or_v0.vec[0], root->aabb0_max_or_v1.vec[1], root->aabb0_max_or_v1.vec[2]);
        glVertex3f(root->aabb0_min_or_v0.vec[0], root->aabb0_min_or_v0.vec[1], root->aabb0_max_or_v1.vec[2]);
        glVertex3f(root->aabb0_min_or_v0.vec[0], root->aabb0_min_or_v0.vec[1], root->aabb0_min_or_v0.vec[2]);
        glVertex3f(root->aabb0_min_or_v0.vec[0], root->aabb0_max_or_v1.vec[1], root->aabb0_min_or_v0.vec[2]);
        glEnd();
        return;
    }
    if (!root->leafFlag) {
        drawBoundingBox(root->child0, cur_depth + 1, target_depth);
        drawBoundingBox(root->child1, cur_depth + 1, target_depth);
    }
}

void drawGeometry(Tree tree)
{

    for (size_t i = 0; i < tree.leafArr.size(); i++)
    {
        glBegin(GL_TRIANGLES);
        glNormal3fv(&findNormal(tree.leafArr[i])[0]);
        glVertex3f(tree.leafArr[i]->aabb0_min_or_v0.vec[0], tree.leafArr[i]->aabb0_min_or_v0.vec[1], tree.leafArr[i]->aabb0_min_or_v0.vec[2]);
        glVertex3f(tree.leafArr[i]->aabb0_max_or_v1.vec[0], tree.leafArr[i]->aabb0_max_or_v1.vec[1], tree.leafArr[i]->aabb0_max_or_v1.vec[2]);
        glVertex3f(tree.leafArr[i]->aabb1_min_or_v2.vec[0], tree.leafArr[i]->aabb1_min_or_v2.vec[1], tree.leafArr[i]->aabb1_min_or_v2.vec[2]);
        glEnd();
    }

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    drawBoundingBox(tree.root, 0, depth_to_draw);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

int main(int argc, char ** argv) {
    

    // loader class 
    // cin filename
    GLRenderer* renderer = new GLRenderer(640, 480);

    renderer->addShaderProgram(argv[1], argv[2]);
    renderer->startDrawLoop();
 
    delete renderer;
    return 0;
}