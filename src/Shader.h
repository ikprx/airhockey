#pragma once
#include <string>
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


class Shader
{
public:
    GLuint id;
    Shader() {}
   
    void use();
   
    void compile(const GLchar * vertexSource, const GLchar * fragmentSource);

    void setFloat(const GLchar * name, GLfloat value);
    void setInt(const GLchar * name, GLint value);
    void setVec2(const GLchar * name, const glm::vec2 & vec);
    void setVec3(const GLchar * name, const glm::vec3 & vec);
    void setVec4(const GLchar * name, const glm::vec4 & vec);
    void setMat4(const GLchar * name, const glm::mat4 & matrix);
private:
    GLuint compileShader(const GLchar * src, GLenum type);
    void checkCompileErros(GLuint obj, GLenum type);
};