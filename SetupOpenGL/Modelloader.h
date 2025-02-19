#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glad/glad.h>

// Vertex structure
struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

class Modelloader {
public:
    // Constructor
    Modelloader(const std::string& path);

    // Draw the model
    void draw() const;

    // Destructor (cleanup OpenGL buffers)
    ~Modelloader();

private:
    // Vertex and index storage
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    // OpenGL buffers
    GLuint VAO, VBO, EBO;

    // Model loading and mesh setup
    void loadModel(const std::string& path);
    void setupMesh();
};
