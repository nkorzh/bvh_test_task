/* Mesh.cpp
 * Author : 
 * Mesh class methods definition
 */

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Renderer/Geometry/Mesh.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, 
    std::vector<Texture>& textures) 
    : vertices(std::move(vertices)), indices(std::move(indices)), textures(std::move(textures)),
        pos(0), drawMode(GL_LINE_LOOP) {
    setupMesh();
}

void Mesh::draw(const ShaderProgram& shader) {
    /* manage textures */
    shader.use();
    
    float greenValue = (sin(glfwGetTime()) / 2.0f) + 0.5;
    float blueValue = (sin(glfwGetTime() * 3) / 5.0f) + 0.6;

    glm::mat4 transformation = glm::rotate(glm::mat4(1), (float)glfwGetTime(), glm::vec3(0, 1, 0));
    shader.setVec4(glm::vec4(0.0f, 0.4, 0.5, 1.0f), std::string("vertexCol"));
    shader.setMat4(glm::mat4(1.0), std::string("model"));

    glPolygonMode(GL_FRONT, GL_LINE);
    glBindVertexArray(VAO);
    glDrawElements(drawMode, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Mesh::setupMesh() {
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
    
    glGenBuffers(1, &EBO);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    // 3 coordinates of type float, not normalized
    // stride is either the step, which is the size of each package, or 0:
    // then GL understands, that the data is tightly packed and defines the stride itself
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    // Normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

    // Texture coordinates
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

    glBindVertexArray(0);
}


Mesh::~Mesh() {
    // shouldn`t the order be reversed?
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

Vertex::Vertex(vec3 p, vec3 n, vec2 t) 
    : Position(p), Normal(n), TexCoords(t) {}

Box::Box(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices) 
    : Mesh(vertices, indices, *(new std::vector<Texture>)) {
}

void Box::draw(const ShaderProgram& shader) {
    shader.use();
    glBindVertexArray(VAO);
    glDrawElements(GL_LINE_LOOP, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
