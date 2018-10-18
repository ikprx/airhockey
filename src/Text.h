#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H  
#include <map>
#include <string>
#include "ResourceManager.h"

struct Character{
	GLuint textureId;
	glm::ivec2 size;
	glm::ivec2 bearing;
	long int advance;
};
class Text{
        std::map<GLchar, Character> characters; 
        GLuint VAO, VBO;
        FT_Library ft;
        FT_Face face;
        glm::mat4 textProjection;
public:
        Text(){}
        void setup(int swidth, int sheight,int fontSize , const std::string & path);
        void renderText(const std::string & shaderName, const std::string & text,  GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);
        ~Text();
};