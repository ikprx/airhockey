#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Camera{
        glm::vec3 position;
        glm::vec3 rotation;

        Camera();
        glm::mat4 getWorldToViewMatrix();    
private:
        glm::vec3 up;
        glm::vec3 viewDirection;
};