#include "Model.h"


Model::Model(const std::string & path)
{
    loadModel(path);
}

void Model::draw(const std::string & shaderName)
{
    for(unsigned int i = 0 ;i <meshes.size();i++){
        meshes[i].draw(shaderName);
    }
}

void Model::loadModel(const std::string & path)
{
    Assimp::Importer importer;

    const aiScene * scene = importer.ReadFile(path, aiProcess_Triangulate);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
        std::cout << importer.GetErrorString() << std::endl;
    }   
    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode * node, const aiScene * scene)
{
    for(unsigned int i = 0; i< node->mNumMeshes; i++){
        aiMesh * mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }
    for(unsigned int i =0;i<node->mNumChildren; i++){
        processNode(node->mChildren[i],scene);
    }
}

Mesh Model::processMesh(aiMesh * mesh, const aiScene * scene)
{
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;

    for(unsigned int i =0; i < mesh->mNumVertices;i++){
        Vertex vertex;

        glm::vec3 vector;
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.position = vector;   

        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;
        vertex.normal = vector;
        vertices.push_back(vertex);
    }

    for(unsigned int i =0 ;i<mesh->mNumFaces;i++){
        aiFace face = mesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices;j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }
    return Mesh(vertices, indices);
}