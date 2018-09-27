#pragma once
#include "ResourceManager.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <string>
#include <GL/glew.h>

struct Vertex{
    glm::vec3 position;
    glm::vec3 normal;
};

class Mesh{
public:
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;

    Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices);
    void draw(std::string shadername);
private:
    GLuint vao, vbo, ebo;

    void setupMesh();
};