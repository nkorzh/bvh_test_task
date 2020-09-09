/* Renderer.h
 * Author : Kozhukharov Nikita
 */

#pragma once

#include <vector>
#include <Renderer/Shaders.h>
#include <Renderer/Geometry/Mesh.h>

class GLRenderer {
    class WindowHandler { // should be singletone
        int width;
        int heigth;
        GLFWwindow* window;

        void makeContextCurrent();
    public:
        WindowHandler(int w, int h, const char* window_name);

        /* Buffers swapping function for smooth picture (double buffering tech) */
        void swapBuffers();
        /* Keyboard and mouse clicks processing function */
        void processInput();
        bool shouldClose(); // returns shouldClose window flag
        bool windowReady(); // returns true if window has been created successfully
    };

    WindowHandler* windowHandler;
    bool renderReady;
    std::vector<ShaderProgram *> shaderPrograms;
    std::vector<Mesh*> meshPtrs;

    bool initGL(int w, int h, const char* window_name);
    bool initGLLoader();

public:
    GLRenderer(int w, int h, const char* window_name = "BVH_task");

    int addShaderProgram(const char*, const char*);
    //int addShaderProgram(ShaderProgram *);
    // ref_ptr should be used
    int addMesh(Mesh*);
    void startDrawLoop();
    ~GLRenderer();
};
