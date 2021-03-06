#include "Model.h"
#include <stb_image.h>
#include <algorithm>


unsigned int textureFromFile(const char *path, const std::string &directory, bool gamma = false);

Model::Model(const std::string & path) : gammaCorrection(false), acceleration(0.0f,0.0f), color(1.0f,1.0f,1.0f)
{
        loadModel(path);
}

void Model::draw(std::string  shader, glm::mat4 view, glm::mat4 projection)
{
	glm::mat4 model = transform.getModel();
	glm::mat4 mvp = projection *  view * model;
	ResourceManager::getShader(shader).use();
        ResourceManager::getShader(shader).setMat4("mvp", mvp);
        ResourceManager::getShader(shader).setVec3("color", color);
        for(unsigned int i = 0; i < meshes.size(); i++)
                meshes[i].draw(shader);
}

void Model::loadModel(std::string const &path)
{
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
        if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
        {
                cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
                return;
        }
        directory = path.substr(0, path.find_last_of('/'));

        processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *node, const aiScene *scene)
{
        for(unsigned int i = 0; i < node->mNumMeshes; i++)
        {
                aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
                meshes.push_back(processMesh(mesh, scene));
        }
        for(unsigned int i = 0; i < node->mNumChildren; i++)
        {
                processNode(node->mChildren[i], scene);
        }

}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene)
{
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;

    
        boundariesMIN.x = mesh->mVertices[0].x;
        boundariesMIN.y= mesh->mVertices[0].y;
        boundariesMIN.z = mesh->mVertices[0].z;

        boundariesMAX.x = mesh->mVertices[0].x;
        boundariesMAX.y= mesh->mVertices[0].y;
        boundariesMAX.z = mesh->mVertices[0].z;


        for(unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
                Vertex vertex;
                glm::vec3 vector; 
                vector.x = mesh->mVertices[i].x;
                vector.y = mesh->mVertices[i].y;
                vector.z = mesh->mVertices[i].z;
                vertex.position = vector;

                boundariesMIN.x = std::min(boundariesMIN.x, vector.x);
                boundariesMIN.y = std::min(boundariesMIN.y, vector.y);
                boundariesMIN.z = std::min(boundariesMIN.z, vector.z);

                boundariesMAX.x = std::max(boundariesMAX.x, vector.x);
                boundariesMAX.y = std::max(boundariesMAX.y, vector.y);
                boundariesMAX.z = std::max(boundariesMAX.z, vector.z);

                vector.x = mesh->mNormals[i].x;
                vector.y = mesh->mNormals[i].y;
                vector.z = mesh->mNormals[i].z;
                vertex.normal = vector;

                if(mesh->mTextureCoords[0]) 
                {
                glm::vec2 vec;
                vec.x = mesh->mTextureCoords[0][i].x; 
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.texCoords = vec;
                }
                else
                vertex.texCoords = glm::vec2(0.0f, 0.0f);
                vector.x = mesh->mTangents[i].x;
                vector.y = mesh->mTangents[i].y;
                vector.z = mesh->mTangents[i].z;
                vertex.tangent = vector;

                vector.x = mesh->mBitangents[i].x;
                vector.y = mesh->mBitangents[i].y;
                vector.z = mesh->mBitangents[i].z;
                vertex.bitangent = vector;
                vertices.push_back(vertex);
        }


        for(unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
        aiFace face = mesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; j++)
        indices.push_back(face.mIndices[j]);
        }
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];    

        std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

        std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

        std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

        std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
        textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

        return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName)
{
        std::vector<Texture> textures;
        for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
        {
                aiString str;
                mat->GetTexture(type, i, &str);
                bool skip = false;
                for(unsigned int j = 0; j < textures_loaded.size(); j++)
                {
                if(std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
                {
                        textures.push_back(textures_loaded[j]);
                        skip = true; 
                        break;
                }
                }
                if(!skip)
                {   
                Texture texture;
                texture.id = textureFromFile(str.C_Str(), this->directory);
                texture.type = typeName;
                texture.path = str.C_Str();
                textures.push_back(texture);
                textures_loaded.push_back(texture);  
                }
        }
        return textures;
}

unsigned int textureFromFile(const char *path, const std::string &directory, bool gamma)
{
    std::string filename = std::string(path);
    filename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
