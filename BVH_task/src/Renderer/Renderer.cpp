/* Renderer.cpp
 * Author : Kozhukharov Nikita
 */

//#include <../glad/glad.h>
//#include <../GLFW/glfw3.h>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <Renderer/Renderer.h>
#include <Renderer/Shaders.h>

// temporary
#include <vector>
#include <Renderer/Geometry/Mesh.h>

static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

/***
 * WindowHandler methods definition
 ***/
void GLRenderer::WindowHandler::makeContextCurrent() {
    glfwMakeContextCurrent(window);
}

GLRenderer::WindowHandler::WindowHandler(int w, int h, const char* window_name) 
    : width(w), heigth(h) {
    window = glfwCreateWindow(w, h, window_name, NULL, NULL);
    if (windowReady()) {
        makeContextCurrent();
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    }
}

void GLRenderer::WindowHandler::swapBuffers() {
    glfwSwapBuffers(window);
}

/***
 * Keys processing function
 ***/
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
bool GLRenderer::initGL(int w, int h, const char* window_name) {
    if (!glfwInit())
        return false;           

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    windowHandler = new WindowHandler(w, h, window_name);
    if (!windowHandler->windowReady()) {
        glfwTerminate();
        return false;
    }
    /// add  default shaders or exit
    return true;
}

bool GLRenderer::initGLLoader() {
    if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        return true;
    //std::cerr << "Failed to get GL-functions addresses.\n";
    return false;
}

int GLRenderer::addShaderProgram(const char* vertexShaderFile, const char* fragmentShaderFile) {
    ShaderProgram * p = new ShaderProgram(vertexShaderFile, fragmentShaderFile);

    if (!p->isValid()) {
        delete p;
        return -1;
    }
    shaderPrograms.emplace_back(p);
    return shaderPrograms.size() - 1;
}

int GLRenderer::addMesh(Mesh * m) {
    if (!m)
        return -1;
    meshPtrs.emplace_back(m);
    return meshPtrs.size() - 1;
}

GLRenderer::GLRenderer(int w, int h, const char* window_name) {
    renderReady = initGL(w, h, window_name) && initGLLoader();
}

void GLRenderer::startDrawLoop() {
    if (!renderReady) {
        //std::cerr << "not ready to render\n";
        return;
    }
    //std::vector<Vertex> vertices = 
    //    { Vertex(vec3(-0.5f, -0.5f, 0.0f), vec3(), vec2()),
    //      Vertex(vec3(0.5f, -0.5f, 0.0f), vec3(), vec2()),
    //      Vertex(vec3(0.0f,  0.5f, 0.0f), vec3(), vec2())
    //    };
    //std::vector<unsigned int> ind = { 0, 1, 2 };
    //std::vector<Texture> tex = {};

    std::vector<Vertex> box_v =
    { Vertex(vec3(-0.5f, -0.5f, 0.4f), vec3(), vec2()),
      Vertex(vec3(0.0f,  0.5f, 0.0f), vec3(), vec2()),
      Vertex(vec3(0.5f, -0.5f, -0.2f), vec3(), vec2()),
      Vertex(vec3(0.0f,  -0.9f, 0.3f), vec3(), vec2())
    };
    std::vector<unsigned int> ind_b = { 0, 3, 1, 2 };
    //Mesh mesh(vertices, ind, tex);
    Box b(box_v, ind_b);


    while (!windowHandler->shouldClose()) {
        windowHandler->processInput();
        // clear window
        glClearColor(0.2f, 0.1f, 0.3f, 0.5f);   // setting the color, can be called once
        glClear(GL_COLOR_BUFFER_BIT);           // cleaning the color buffer, necessary to call every time

        for (const auto& m : meshPtrs) {
            m->draw(*shaderPrograms[0]);
        }
        //b.draw(*shaderPrograms[0]);

        windowHandler->swapBuffers();
        glfwPollEvents(); // or WaitEvents
    }
}

GLRenderer::~GLRenderer() {
    glfwTerminate();
}


/***
 * Window size change callback
 ***/

static void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

