/* Renderer.h
 * Author : Kozhukharov Nikita
 */

#pragma once

class GLRenderer {
    class WindowHandler {
        int width;
        int heigth;
        GLFWwindow* window;

        void makeContextCurrent();
    public:
        WindowHandler(int w, int h, const char* window_name, void (*callback)(GLFWwindow*, int, int));

        /* Buffers swapping function for smooth picture (double buffering tech) */
        void swapBuffers();
        /* Keyboard and mouse clicks processing function */
        void processInput();
        bool shouldClose(); // returns shouldClose window flag
        bool windowReady(); // returns true if window has been created successfully
    };

    WindowHandler* windowHandler;
    bool renderReady;

    bool initGL(int w, int h, const char* window_name, void (*callback)(GLFWwindow*, int, int));
    bool initGLLoader();

public:
    GLRenderer(int w, int h, void (*callback)(GLFWwindow*, int, int),
        const char* window_name = "BVH_task");

    void startDrawLoop();
    ~GLRenderer();
};