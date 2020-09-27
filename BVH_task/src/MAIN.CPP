/* Main.cpp
 * Author : Kozhukharov Nikita, Elagin Dmitrii
 */

#include <windows.h>
#include <GLFW/glfw3.h>
#include <BVH/BVH.h>
#include <Renderer/Renderer.h>
#include <Math/MathFunctions.h>
#include <iostream>

void addBoundingBoxes(BvhNodeTree* root, int cur_depth, int target_depth, GLRenderer& renderer, int shaderId, bool enableHierarchy = false)
{
    if (cur_depth < 15) {
        if (root && !(root->leafFlag) && (cur_depth == target_depth || !enableHierarchy)) {
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
            //throw bad_alloc exeption here if render all bvh lvls at the same time
            std::vector<unsigned int> ind_i = { 6, 5, 1, 2, 4, 5, 6, 7, 4, 2, 3, 7, 6, 0, 3, 2, 1, 0 };
            Mesh* i = new Mesh(box_i, ind_i, *(new std::vector<Texture>));
            i->setDrawMode(GL_LINE_STRIP);
            i->setShader(shaderId);
            i->setColor(glm::vec4(1.0, 0.8, 0.0, 1.0));
            renderer.addMesh(i);

            if (enableHierarchy && (cur_depth == target_depth))
            {
                return;
            }
        }
    }
    else
    {
        return;
    }
    if (root && !(root->leafFlag)) {
        addBoundingBoxes(root->child0, cur_depth + 1, target_depth, renderer, shaderId, enableHierarchy);
        addBoundingBoxes(root->child1, cur_depth + 1, target_depth, renderer, shaderId, enableHierarchy);
    }
}

void addGeometry(Tree& BVHTree, GLRenderer& renderer, int shaderId) {
    for (size_t i = 0; i < BVHTree.leafArr.size(); i++) {
        std::vector<Vertex> triangle_i = {
        Vertex(vec3(BVHTree.leafArr[i]->aabb0_min_or_v0.x(), BVHTree.leafArr[i]->aabb0_min_or_v0.y(), BVHTree.leafArr[i]->aabb0_min_or_v0.z()), findNormal(BVHTree.leafArr[i]), vec2()),
        Vertex(vec3(BVHTree.leafArr[i]->aabb0_max_or_v1.x(), BVHTree.leafArr[i]->aabb0_max_or_v1.y(), BVHTree.leafArr[i]->aabb0_max_or_v1.z()), findNormal(BVHTree.leafArr[i]), vec2()),
        Vertex(vec3(BVHTree.leafArr[i]->aabb1_min_or_v2.x(), BVHTree.leafArr[i]->aabb1_min_or_v2.y(), BVHTree.leafArr[i]->aabb1_min_or_v2.z()), findNormal(BVHTree.leafArr[i]), vec2()),
        };
        std::vector<unsigned int> ind_i = {0, 1, 2, 0};
        Mesh* triangle = new Mesh(triangle_i, ind_i, *(new std::vector<Texture>));
        triangle->setDrawMode(GL_TRIANGLES);
        triangle->setShader(shaderId);
        triangle->setColor(glm::vec4(0.9, 0.9, 0.9, 1.0));
        triangle->setShadeMode(true);
        renderer.addMesh(triangle);
    }
}

glm::vec3 getStartPosition(Tree& tree) {
    glm::vec3 startPos;
    startPos.x = (tree.root->aabb0_max_or_v1.x() + tree.root->aabb0_min_or_v0.x()) / 2;
    startPos.y = (tree.root->aabb0_max_or_v1.y() + tree.root->aabb0_min_or_v0.y()) / 2;
    startPos.z = (tree.root->aabb0_max_or_v1.z() + tree.root->aabb0_min_or_v0.z()) / 2;
    return startPos;
}

int main(int argc, char** argv) {

    //HWND hConsole = GetConsoleWindow();
    //ShowWindow(hConsole, SW_HIDE);

    std::string solutionDir(argv[0]);
    try {
        std::string solConfig("Release");       
        
        solConfig = "Debug";

        solutionDir.resize(solutionDir.find(solConfig));
    }
    catch (const std::bad_alloc&) {
        std::cout << "Bad alloc\nCouldnt remember solution directory\n";
    }

    
    GLRenderer renderer(800, 600);

    //BVHTree.leafArr is array of leafs (triangles), I thought it would be 
    //convenient to have a separate array for geometry, each time it takes a very 
    //long time to traverse the tree in search of geometry

    //BVHTree.leafArr[i] stores 4 points in itself, but the fourth point stores zeros and it does not participate 
    //in drawing the triangle. An example of how to access the coordinates in the drawGeometry function in a loop

    //BVHTree.root root node in the tree, from it we start recursive tree traversal to draw the BVH, 
    //the recursive function itself can be observed above (drawBoundingBox), of course, this part will 
    //need to be rewritten for your implementation

    std::string modelFolder(solutionDir + "BVH_task\\models\\sponza_bvh2.bin");

    /**
     * Adding shaders
     */
    std::string shaderFolder(solutionDir + "BVH_task\\src\\Renderer\\Shaders\\");
    std::string vertName = shaderFolder + "Default\\vert.glsl";
    std::string fragName = shaderFolder + "Default\\frag.glsl";

    std::string CMDFilePath(solutionDir + "BVH_task\\models\\" + argv[1] + ".bin");

    int defaultShaderId = renderer.addShaderProgram(vertName.c_str(), fragName.c_str());

    vertName = shaderFolder + "Lighted\\vert.glsl";
    fragName = shaderFolder + "Lighted\\frag.glsl";
    int lightedShaderId = renderer.addShaderProgram(vertName.c_str(), fragName.c_str());

    Tree BVHTree = BuildTree(CMDFilePath.c_str());
    
    bool enableHierarchy = false; //turn on this flag if you need hierarchy bounding box output

    int hierarchyLVL = 3;

    addGeometry(BVHTree, renderer, lightedShaderId);
    addBoundingBoxes(BVHTree.root, 0, hierarchyLVL, renderer, lightedShaderId, enableHierarchy);
    renderer.setCameraPos(getStartPosition(BVHTree));

    renderer.startDrawLoop();

    return 0;
}
