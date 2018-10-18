#include "Text.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

void Text::setup(int swidth, int sheight, int fontSize, const std::string & path)
{
	textProjection = glm::ortho(0.0f,static_cast<float>(swidth), 0.0f,static_cast<float>(sheight), -100.0f,100.0f);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 
	
	if(FT_Init_FreeType(&ft)){
		std::cout << "Could not init free type" << std::endl;
	}
	if(FT_New_Face(ft, path.c_str(), 0, &face)){
		std::cout << "failed to load a font" << std::endl;
	}
	FT_Set_Pixel_Sizes(face, 0, fontSize);  
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	for(GLubyte c  = 0;c<128;c++){
		if(FT_Load_Char(face, c, FT_LOAD_RENDER)){
			std::cout << "Failed to load a glyph" << std::endl;
			continue;
		}
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		Character character = {
			texture, 
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
        	glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};
		characters.insert(std::pair<GLchar, Character>(c, character));
	}

	glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);


}

void Text::renderText(const std::string & shadername, const std::string & text,GLfloat x, GLfloat y , GLfloat scale, glm::vec3 color)
{
	ResourceManager::getShader(shadername).use();	
	ResourceManager::getShader(shadername).setVec3("textColor", color);
	ResourceManager::getShader(shadername).setMat4("projection", textProjection);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(VAO);

	std::string::const_iterator c;
	for(c = text.begin();c!=text.end();c++){
		Character ch = characters[*c];
		GLfloat xpos = x + ch.bearing.x *scale;
        GLfloat ypos = y - (ch.size.y - ch.bearing.y) * scale;
		GLfloat w = ch.size.x * scale;
        GLfloat h = ch.size.y * scale;
        GLfloat vertices[6][4] = {
            { xpos,     ypos + h,   0.0, 0.0 },            
            { xpos,     ypos,       0.0, 1.0 },
            { xpos + w, ypos,       1.0, 1.0 },

            { xpos,     ypos + h,   0.0, 0.0 },
            { xpos + w, ypos,       1.0, 1.0 },
            { xpos + w, ypos + h,   1.0, 0.0 }       
		};
		glBindTexture(GL_TEXTURE_2D, ch.textureId);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDrawArrays(GL_TRIANGLES, 0, 6);
		x += (ch.advance >> 6) * scale; 
	}
	glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);

}

Text::~Text(){
	FT_Done_Face(face);
	FT_Done_FreeType(ft);
}