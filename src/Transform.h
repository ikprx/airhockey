#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


struct Transform{
public:
        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 scale;
        Transform();

        glm::mat4 getModel();

};