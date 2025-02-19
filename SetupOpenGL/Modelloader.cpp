#include "Modelloader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>

// Constructor: Load and setup model
Modelloader::Modelloader(const std::string& path) {
    loadModel(path);
    setupMesh();
}

// OBJ Loader Function
void Modelloader::loadModel(const std::string& path) {
    std::ifstream file(path);
    if (!file) {
        std::cerr << "Failed to open OBJ file: " << path << std::endl;
        return;
    }

    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texCoords;
    std::unordered_map<std::string, unsigned int> uniqueVertices; // Avoid duplicates
    std::string line;

    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string prefix;
        ss >> prefix;

        if (prefix == "v") { // Vertex positions
            glm::vec3 position(0.0f);
            ss >> position.x >> position.y >> position.z;
            positions.push_back(position);
        }
        else if (prefix == "vn") { // Vertex normals
            glm::vec3 normal(0.0f);
            ss >> normal.x >> normal.y >> normal.z;
            normals.push_back(normal);
        }
        else if (prefix == "vt") { // Texture coordinates
            glm::vec2 texCoord(0.0f);
            ss >> texCoord.x >> texCoord.y;
            texCoord.y = 1.0f - texCoord.y; // Flip V coordinate
            texCoords.push_back(texCoord);
        }
        else if (prefix == "f") { // Faces
            std::vector<unsigned int> faceIndices;
            std::string vertexData;
            while (ss >> vertexData) {
                // Check if this vertex already exists
                if (uniqueVertices.find(vertexData) == uniqueVertices.end()) {
                    std::istringstream vData(vertexData);
                    std::string vIdx, tIdx, nIdx;
                    getline(vData, vIdx, '/'); // Position index
                    getline(vData, tIdx, '/'); // Texture index
                    getline(vData, nIdx, '/'); // Normal index

                    // Convert indices to integers (OBJ starts from 1)
                    int posIdx = std::stoi(vIdx) - 1;
                    int texIdx = (!tIdx.empty()) ? std::stoi(tIdx) - 1 : -1;
                    int normIdx = (!nIdx.empty()) ? std::stoi(nIdx) - 1 : -1;

                    // Create vertex
                    Vertex vertex;
                    vertex.position = positions[posIdx];
                    vertex.normal = (normIdx >= 0) ? normals[normIdx] : glm::vec3(0.0f, 0.0f, 1.0f);
                    vertex.texCoords = (texIdx >= 0) ? texCoords[texIdx] : glm::vec2(0.0f, 0.0f);

                    // Store vertex and index
                    unsigned int newIndex = static_cast<unsigned int>(vertices.size());
                    vertices.push_back(vertex);
                    uniqueVertices[vertexData] = newIndex;
                }

                // Store index
                faceIndices.push_back(uniqueVertices[vertexData]);
            }

            // Ensure proper triangulation
            if (faceIndices.size() == 3) {
                indices.insert(indices.end(), faceIndices.begin(), faceIndices.end());
            }
            else if (faceIndices.size() == 4) {
                // Convert quad to two triangles
                indices.push_back(faceIndices[0]);
                indices.push_back(faceIndices[1]);
                indices.push_back(faceIndices[2]);

                indices.push_back(faceIndices[0]);
                indices.push_back(faceIndices[2]);
                indices.push_back(faceIndices[3]);
            }
        }
    }
}

// Setup OpenGL Buffers
void Modelloader::setupMesh() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    // VBO: Store vertex data
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    // EBO: Store indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // Position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

    // Normal attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

    // Texture coordinate attribute
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

    glBindVertexArray(0);
}

// Draw the model
void Modelloader::draw() const {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

// Destructor: Clean up buffers
Modelloader::~Modelloader() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}
