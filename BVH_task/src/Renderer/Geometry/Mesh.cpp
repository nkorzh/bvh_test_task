/* Mesh.cpp
 * Author : 
 * Mesh class methods definition
 */

//#include <../glad/glad.h>
//#include <../GLFW/glfw3.h>

#include "glad//glad.h"
#include "GLFW/glfw3.h"
#include "Renderer/Geometry/Mesh.h"

//#include <../Renderer/Geometry/Mesh.h>

Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, 
    std::vector<Texture>& textures) 
    : vertices(std::move(vertices)), indices(std::move(indices)), textures(std::move(textures)) {
    setupMesh();
}

void Mesh::draw(const ShaderProgram& shader) {
    /* manage textures */
    shader.use();
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
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
