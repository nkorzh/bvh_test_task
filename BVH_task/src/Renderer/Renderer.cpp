/* Renderer.cpp
 * Author : Kozhukharov Nikita
 */

#include <..\glad\glad.h>
#include <..\GLFW\glfw3.h>

#include "Renderer/Renderer.h"

/***
 * WindowHandler methods definition
 ***/
void GLRenderer::WindowHandler::makeContextCurrent() {
    glfwMakeContextCurrent(window);
}

GLRenderer::WindowHandler::WindowHandler(int w, int h, const char* window_name, void(*callback)(GLFWwindow*, int, int)) : width(w), heigth(h) {
    window = glfwCreateWindow(w, h, window_name, NULL, NULL);
    if (windowReady()) {
        makeContextCurrent();
        glfwSetFramebufferSizeCallback(window, callback);
    }
}

void GLRenderer::WindowHandler::swapBuffers() {
    glfwSwapBuffers(window);
}

void GLRenderer::WindowHandler::processInput() {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

bool GLRenderer::WindowHandler::shouldClose() {
    return glfwWindowShouldClose(window);
}

bool GLRenderer::WindowHandler::windowReady() {
    return window != NULL;
}


/***
 * GLRenderer methods definition
 ***/
bool GLRenderer::initGL(int w, int h, const char* window_name, void (*callback)(GLFWwindow*, int, int)) {
    if (!glfwInit())
        return false;           

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    windowHandler = new WindowHandler(w, h, window_name, callback);
    if (!windowHandler->windowReady()) {
        glfwTerminate();
        return false;
    }
    return true;
}

bool GLRenderer::initGLLoader() {
    if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        return true;
    //std::cerr << "Failed to get GL-functions addresses.\n";
    return false;
}

GLRenderer::GLRenderer(int w, int h, void (*callback)(GLFWwindow*, int, int),
    const char* window_name) {
    renderReady = initGL(w, h, window_name, callback) && initGLLoader();
}

void GLRenderer::startDrawLoop() {
    if (!renderReady) {
        //std::cerr << "not ready to render\n";
        return;
    }

    while (!windowHandler->shouldClose()) {
        windowHandler->processInput();
        // clear window
        glClearColor(0.2f, 0.1f, 0.3f, 0.5f);   // setting the color, can be called once
        glClear(GL_COLOR_BUFFER_BIT);           // cleaning the color buffer, necessary to call every time

        /// shader magic

        windowHandler->swapBuffers();
        glfwPollEvents(); // or WaitEvents
    }
}

GLRenderer::~GLRenderer() {
    glfwTerminate();
}
