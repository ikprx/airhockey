#pragma once

#include <map>
#include <GL/glew.h>
#include <string>
#include "Shader.h"

#include "Mesh.h"
class Mesh;
class Texture;
#include "Model.h"
class Model;

#include "Text.h"
class Text;

class ResourceManager{
public:
    static std::map<std::string, Shader> shaders;
    static std::map<std::string, Model> models;
    static Text text;
    static Shader loadShader(std::string name, const GLchar * vShaderFile, const GLchar * fShaderFile);
    static Shader getShader(std::string name);
    static Model loadModel(std::string name, const std::string & path);
    static Model getModel(std::string name);
    static void clear();
private:
    ResourceManager() {
        
    }
    static Shader loadShaderFromFile(const GLchar * vShaderFile, const GLchar * fShaderFile);
    static std::string loadFile(const GLchar * path);
};