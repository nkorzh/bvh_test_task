/* Renderer.h
 * Author : Kozhukharov Nikita
 */

#pragma once

//#include <glad/glad.h>
//#include <glfw/glfw3.h>
#include <Renderer/Camera.h>
#include <Renderer/Shaders.h>
#include <Renderer/Geometry/Mesh.h>
#include <vector>

class GLRenderer {
    /// add cursor release by ctrl
    class WindowHandler { // should be singletone
        int width;
        int height;
        GLFWwindow* window;
        // timer details
        float prevGlobalTime;
        float deltaTime;
        // mouse details
        float lastX = 0, lastY = 0;
        float curX, curY;

        void makeContextCurrent();
    public:
        Camera camera;
        WindowHandler(int w, int h, const char* window_name);

        float getWindowAspect();
        void updateTime();
        /* Buffers swapping function for smooth picture (double buffering tech) */
        void swapBuffers();
        /* Keyboard and mouse clicks processing function */
        void processInput();
        bool shouldClose(); // returns shouldClose window flag
        bool windowReady(); // returns true if window has been created successfully
    };

    WindowHandler* windowHandler;
    bool renderReady;
    std::vector<ShaderProgram*> shaderPrograms;
    std::vector<Mesh*> meshPtrs;

    bool initGL(int w, int h, const char* window_name);
    bool initGLLoader();

public:
    GLRenderer(int w, int h, const char* window_name = "BVH_task");

    int addShaderProgram(const char*, const char*);
    // ref_ptr should be used
    int addMesh(Mesh*);
    void startDrawLoop();
    ~GLRenderer();
};
