#pragma once
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "ResourceManager.h"
#include "Mesh.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

#include <glm/glm.hpp>
#include "Transform.h"
class Model 
{
public:
    glm::vec2 acceleration;
    glm::vec3 boundariesMAX;
    glm::vec3 boundariesMIN;
    glm::vec3 color; 

    std::vector<Texture> textures_loaded;
    std::vector<Mesh> meshes;
    std::string directory;
    bool gammaCorrection;

    Model(const std::string & path);

    void draw(std::string  shader, glm::mat4 view, glm::mat4 projection);
    
    void loadModel(std::string const &path);
    Transform transform;
private:

    void processNode(aiNode *node, const aiScene *scene);

    Mesh processMesh(aiMesh *mesh, const aiScene *scene);

    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
};


