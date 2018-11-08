#include "ResourceManager.h"
#include <iostream>
#include <fstream>
#include <sstream>
std::map<std::string, Shader> ResourceManager::shaders;
std::map<std::string, Model*> ResourceManager::models;
Text ResourceManager::text;

Model* ResourceManager::loadModel(std::string name, const std::string & path)
{
    models[name] = new Model(path);
    return models[name];
}

Model* ResourceManager::getModel(std::string name)
{
    return models[name];
}

Shader ResourceManager::loadShader(std::string name, const GLchar * vShaderFile, const GLchar * fShaderFile)
{
    shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile);
    return shaders[name];
}

Shader ResourceManager::getShader(std::string name)
{
    return shaders[name];
}

void ResourceManager::clear()
{
    for(auto iter : shaders){
        glDeleteProgram(iter.second.id);
    }
}

Shader ResourceManager::loadShaderFromFile(const GLchar * vShaderFile, const GLchar * fShaderFile)
{
    std::string vertexCode = loadFile(vShaderFile);
    std::string fragmentCode = loadFile(fShaderFile);
    Shader shader;
    shader.compile(vertexCode.c_str(), fragmentCode.c_str());
    return shader;
}

std::string ResourceManager::loadFile(const GLchar * path)
{
    std::string code;
    std::ifstream file(path);
    std::stringstream ss;
    ss << file.rdbuf();
    file.close();
    code = ss.str();
    return code;
}

