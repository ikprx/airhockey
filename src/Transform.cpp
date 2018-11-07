#include "Transform.h"

Transform::Transform()
        : position(0.0f,0.0f,0.0f), rotation(0.0f,0.0f,0.0f), scale(1.0f,1.0f,1.0f)
{

}

glm::mat4 Transform::getModel(){
        glm::mat4 model(1.0f);
        model = glm::translate(model, position);
        model = glm::scale(model, scale);
        model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f,0.0f,0.0f));
        model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f,1.0f,0.0f));
        model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f,0.0f,1.0f));
        return model;
}