/* Main.cpp
 * Author : Kozhukharov Nikita
 */

#define depth_to_draw 3 //

#include <GLFW/glfw3.h>
#include <BVH/BVH.h>
#include <Renderer/Renderer.h>
#include <Math/MathFunctions.h>
#include <iostream>


void drawBoundingBox(BvhNodeTree* root, int cur_depth, int target_depth, GLRenderer& renderer)
{
    if ((root != nullptr) && (!root->leafFlag)) {
        std::vector<Vertex> box_i = {
            Vertex(vec3(root->aabb0_min_or_v0[0], root->aabb0_min_or_v0[1], root->aabb0_min_or_v0[2]), vec3(), vec2()),
            Vertex(vec3(root->aabb0_max_or_v1[0], root->aabb0_min_or_v0[1], root->aabb0_min_or_v0[2]), vec3(), vec2()),
            Vertex(vec3(root->aabb0_max_or_v1[0], root->aabb0_min_or_v0[1], root->aabb0_max_or_v1[2]), vec3(), vec2()),
            Vertex(vec3(root->aabb0_min_or_v0[0], root->aabb0_min_or_v0[1], root->aabb0_max_or_v1[2]), vec3(), vec2()),

            Vertex(vec3(root->aabb0_max_or_v1[0], root->aabb0_max_or_v1[1], root->aabb0_max_or_v1[2]), vec3(), vec2()),
            Vertex(vec3(root->aabb0_max_or_v1[0], root->aabb0_max_or_v1[1], root->aabb0_min_or_v0[2]), vec3(), vec2()),
            Vertex(vec3(root->aabb0_min_or_v0[0], root->aabb0_max_or_v1[1], root->aabb0_min_or_v0[2]), vec3(), vec2()),
            Vertex(vec3(root->aabb0_min_or_v0[0], root->aabb0_max_or_v1[1], root->aabb0_max_or_v1[2]), vec3(), vec2()),
        };
        std::vector<unsigned int> ind_i = { 6, 5, 1, 2, 4, 5, 6, 7, 4, 2, 3, 7, 6, 0, 3, 2, 1, 0 };
        Mesh* i = new Mesh(box_i, ind_i, *(new std::vector<Texture>));
        i->setDrawMode(GL_LINE_STRIP);
        renderer.addMesh(i);
    }
    if (!root->leafFlag) {
        drawBoundingBox(root->child0, cur_depth + 1, target_depth, renderer);
        drawBoundingBox(root->child1, cur_depth + 1, target_depth, renderer);
    }
}

void drawGeomtry(Tree& BVHTree, GLRenderer& renderer) {

    for (size_t i = 0; i < BVHTree.leafArr.size(); i++)
    {
       
        std::vector<Vertex> triangle_i = {
        Vertex(vec3(BVHTree.leafArr[i]->aabb0_min_or_v0.x(), BVHTree.leafArr[i]->aabb0_min_or_v0.y(), BVHTree.leafArr[i]->aabb0_min_or_v0.z()), findNormal(BVHTree.leafArr[i]), vec2()),
        Vertex(vec3(BVHTree.leafArr[i]->aabb0_max_or_v1.x(), BVHTree.leafArr[i]->aabb0_max_or_v1.y(), BVHTree.leafArr[i]->aabb0_max_or_v1.z()), findNormal(BVHTree.leafArr[i]), vec2()),
        Vertex(vec3(BVHTree.leafArr[i]->aabb1_min_or_v2.x(), BVHTree.leafArr[i]->aabb1_min_or_v2.y(), BVHTree.leafArr[i]->aabb1_min_or_v2.z()), findNormal(BVHTree.leafArr[i]), vec2()),
        };
        std::vector<unsigned int> ind_i = {0, 1, 2, 0};
        Mesh* triangle = new Mesh(triangle_i, ind_i, *(new std::vector<Texture>));
        triangle->setDrawMode(GL_LINE_STRIP);
        renderer.addMesh(triangle);
    }
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

int main(int argc, char** argv) {

    std::string solutionDir(argv[0]);
    try {
        solutionDir.resize(solutionDir.find("Debug"));
    }
    catch (const std::bad_alloc&) {
        std::cout << "1. bad alloc\n";
    }
    // loader class 
    // cin filename
    GLRenderer renderer(800, 600);

    //Tree BVHTree = BuildTree("C:\\Users\\dimon\\Desktop\\task_description\\binary_dump\\cornell_box_bvh2.bin");

    //BVHTree.leafArr is array of leafs (triangles), I thought it would be 
    //convenient to have a separate array for geometry, each time it takes a very 
    //long time to traverse the tree in search of geometry

    //BVHTree.leafArr[i] stores 4 points in itself, but the fourth point stores zeros and it does not participate 
    //in drawing the triangle. An example of how to access the coordinates in the drawGeometry function in a loop

    //BVHTree.root root node in the tree, from it we start recursive tree traversal to draw the BVH, 
    //the recursive function itself can be observed above (drawBoundingBox), of course, this part will 
    //need to be rewritten for your implementation
    /// parser + model creation

    std::string modelFolder(solutionDir + "BVH_task\\models\\cornell_box\\cornell_box.bin");

    /// renderer.addMesh(...);
    Tree BVHTree = BuildTree(modelFolder.c_str());
    std::string shaderFolder(solutionDir + "BVH_task\\src\\Renderer\\Shaders\\Default\\");
    std::string vertName = shaderFolder + "vert.glsl";
    std::string fragName = shaderFolder + "frag.glsl";
    renderer.addShaderProgram(vertName.c_str(), fragName.c_str());

    // add objects3кр
    std::vector<Vertex> box_v =
    { Vertex(vec3(-0.2f, -0.3f, 0.3f), vec3(), vec2()),
      Vertex(vec3(0.3f,  -0.3f, 0.1f), vec3(), vec2()),
      Vertex(vec3(0.0f, 0.6f, -0.05f), vec3(), vec2()),
      Vertex(vec3(0.0f,  -0.45f, -0.5f), vec3(), vec2())
    };
    std::vector<unsigned int> ind_b = { 0, 1, 2, 0, 3, 2, 1, 3 };
    Mesh* b = new Mesh(box_v, ind_b, *(new std::vector<Texture>));
    //renderer.addMesh(b);
    drawBoundingBox(BVHTree.root, 0, 3, renderer);
    drawGeomtry(BVHTree, renderer);
    renderer.startDrawLoop();

    return 0;
}
