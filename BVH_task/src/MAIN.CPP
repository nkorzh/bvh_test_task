/* Main.cpp
 * Author : Kozhukharov Nikita
 */

#include <iostream>
#include <GLFW/glfw3.h>

#include "Renderer/Renderer.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main(void)
{
    GLRenderer* renderer = new GLRenderer(640, 480, framebuffer_size_callback);

    renderer->startDrawLoop();
 
    delete renderer;
    return 0;
}