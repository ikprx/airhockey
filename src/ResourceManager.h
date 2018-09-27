#pragma once

#include <map>
#include <GL/glew.h>
#include <string>
#include "Shader.h"


class ResourceManager{
public:
    static std::map<std::string, Shader> shaders;
    static Shader loadShader(std::string name, const GLchar * vShaderFile, const GLchar * fShaderFile);
    static Shader getShader(std::string name);


    static void clear();
private:
    ResourceManager() {}
    static Shader loadShaderFromFile(const GLchar * vShaderFile, const GLchar * fShaderFile);
    static std::string loadFile(const GLchar * path);
};