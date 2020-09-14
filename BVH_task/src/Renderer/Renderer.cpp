/* Renderer.cpp
 * Author : Kozhukharov Nikita
 */


#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Renderer/Renderer.h>
#include <Renderer/Shaders.h>


static void framebuffer_size_callback(GLFWwindow* window, int width, int height);


/***
 * WindowHandler methods definition
 ***/
void GLRenderer::WindowHandler::makeContextCurrent() {
    glfwMakeContextCurrent(window);
}

GLRenderer::WindowHandler::WindowHandler(int w, int h, const char* window_name) 
    : width(w), height(h) {
    window = glfwCreateWindow(w, h, window_name, NULL, NULL);
    if (windowReady()) {
        makeContextCurrent();
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
        // set cursor to the center of the screen
        lastX = (float)width / 2;
        lastY = (float)height / 2;
        glfwSetCursorPos(window, (double)width / 2, (double)height / 2);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
}

float GLRenderer::WindowHandler::getWindowAspect() {
    return (float)width / height;
}

void GLRenderer::WindowHandler::updateTime() {
    float curTime = (double)glfwGetTime();
    deltaTime = curTime - prevGlobalTime;
    prevGlobalTime = curTime;
}

void GLRenderer::WindowHandler::swapBuffers() {
    glfwSwapBuffers(window);
}

/***
 * Keys processing function
 ***/
void GLRenderer::WindowHandler::processInput() {
    bool mouseBLeftPressed = glfwGetKey(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
    bool mouseBRightPressed = glfwGetKey(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.moveByKeys(Camera::CameraDirection::FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.moveByKeys(Camera::CameraDirection::LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.moveByKeys(Camera::CameraDirection::RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.moveByKeys(Camera::CameraDirection::BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
        camera.printCameraSettings();

    /// todo: enable or disable cursor by ctrl
    double cursorX, cursorY;
    glfwGetCursorPos(window, &cursorX, &cursorY);
    camera.moveByMouse((float)cursorX - lastX, (float)cursorY - lastY, mouseBLeftPressed);
    lastX = (float)cursorX;
    lastY = (float)cursorY;
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
    glEnable(GL_DEPTH_TEST);
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

    //std::vector<Vertex> box_v =
    //{ Vertex(vec3(-0.5f, -0.5f, 0.4f), vec3(), vec2()),
    //  Vertex(vec3(0.0f,  0.5f, 0.0f), vec3(), vec2()),
    //  Vertex(vec3(0.5f, -0.5f, -0.2f), vec3(), vec2()),
    //  Vertex(vec3(0.0f,  -0.9f, 0.3f), vec3(), vec2())
    //};
    //std::vector<unsigned int> ind_b = { 0, 3, 1, 2 };
    //Box b(box_v, ind_b);


    while (!windowHandler->shouldClose()) {
        windowHandler->updateTime();
        windowHandler->processInput();
        // clear window
        glClearColor(0.2f, 0.1f, 0.3f, 0.5f);               // setting the color, can be called once
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // cleaning the color buffer, necessary to call every time

        // send data to shaders
        float width_height = windowHandler->getWindowAspect();
        shaderPrograms[0]->setMat4(windowHandler->camera.getProjMatrix(45.0f, width_height, 0.1f, 100.0f),
            std::string("proj"));
        shaderPrograms[0]->setMat4(windowHandler->camera.getViewMatrix(), std::string("view"));

        for (const auto& m : meshPtrs) {
            m->draw(*shaderPrograms[0]);
        }

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


