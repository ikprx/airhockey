#include "Shader.h"
#include <iostream>
void Shader::use()
{
    glUseProgram(id);
}

void Shader::compile(const GLchar * vertexSource, const GLchar * fragmentSource)
{
    GLuint sVertex = compileShader(vertexSource, GL_VERTEX_SHADER);
    GLuint sFragment = compileShader(fragmentSource, GL_FRAGMENT_SHADER);

    id = glCreateProgram();
    glAttachShader(id,sVertex);
    glAttachShader(id,sFragment);
    glLinkProgram(id);
    checkCompileErros(id,GL_VERTEX_SHADER + GL_FRAGMENT_SHADER);

    glDeleteShader(sVertex);
    glDeleteShader(sFragment);

}

void Shader::setFloat(const GLchar * name, GLfloat value)
{
    glUniform1f(glGetUniformLocation(id,name),value);
}
void Shader::setInt(const GLchar * name, GLint value)
{
    glUniform1i(glGetUniformLocation(id,name),value);
}
void Shader::setVec2(const GLchar * name, const glm::vec2 & vec)
{
    glUniform2f(glGetUniformLocation(id,name),vec.x, vec.y);
}
void Shader::setVec3(const GLchar * name, const glm::vec3 & vec)
{
    glUniform3f(glGetUniformLocation(id,name),vec.x, vec.y, vec.z);
}
void Shader::setVec4(const GLchar * name, const glm::vec4 & vec)
{
    glUniform4f(glGetUniformLocation(id,name),vec.x, vec.y, vec.z, vec.w);
}
void Shader::setMat4(const GLchar * name, const glm::mat4 & matrix)
{
    glUniformMatrix4fv(glGetUniformLocation(id,name),1,GL_FALSE,glm::value_ptr(matrix));
}

void Shader::checkCompileErros(GLuint obj, GLenum type){
    GLint success;
    GLchar infoLog[1024];
    if(type == GL_VERTEX_SHADER || type == GL_FRAGMENT_SHADER){
        glGetShaderiv(obj, GL_COMPILE_STATUS, &success);
        if(!success){   
            glGetShaderInfoLog(obj,1024,NULL,infoLog);
            std::cout << infoLog << std::endl;
        }
    }
}

GLuint Shader::compileShader(const GLchar * src, GLenum type)
{   
    GLuint shader;
    shader = glCreateShader(type);
    glShaderSource(shader,1,&src,NULL);
    glCompileShader(shader);
    checkCompileErros(shader,type);
    
    return shader;
}