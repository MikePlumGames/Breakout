#include <ft2build.h>
#include FT_FREETYPE_H  

#include "TextRenderer.h"

#include "Breakout.h"
#include "ShaderLoader.h"

#include <glad/glad.h>
#include <glm/glm.hpp>		// matricies and vectores
#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>

#include <algorithm>
#include <iostream>

void Font::loadFont(const char* path, unsigned int _fontSize)
{
	// initialize the free type library
	FT_Library ft;
	if (FT_Init_FreeType(&ft))
	{
		std::cout << "loadFont error: Could not init FreeType Library" << std::endl;
		return;
	}

	// load a font
	FT_Face face;
	if (FT_New_Face(ft, "assets/fonts/Martian Mono/content/static/MartianMono-regular.ttf", 0, &face))
	{
		std::cout << "loadFont error: Failed to load font" << std::endl;
		FT_Done_FreeType(ft);
		return;
	}

	// delete the previous textures
	for (int i = 0; i < characters.size(); i++)
	{
		if (characters[i].TextureID != 0)
			glDeleteTextures(1, &characters[i].TextureID);
	}
	
	// set the size of the font
	fontSize = _fontSize;
	// 0 lets freetype calculate it by itself
	FT_Set_Pixel_Sizes(face, 0, fontSize);

	// disable byte-alignment restriction
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// load all glyphs
	for (unsigned char c = 0; c < 128; c++)
	{
		// load character glyph 
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "loadFont error: Failed to load Glyph: " << c << std::endl;
			continue;
		}
		// generate texture
		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		// we use GL_RED because the image generated is 8-bit grayscale texture, also that why we need: glPixelStorei(GL_UNPACK_ALIGNMENT, 1); above this loop
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
		// set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// strore the character in the array
		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			static_cast<unsigned int>(face->glyph->advance.x)
		};
		characters[c] = character;
	}

	// deallocate resources
	FT_Done_Face(face);
	FT_Done_FreeType(ft);
}

// render text
void renderText(const Font& font, std::string text, glm::vec2 pos, float scale, glm::vec3 color, glm::vec2 target)
{
	static bool isVAOconfigured{ false };
	static unsigned int VAO{};
	static unsigned int VBO{};
	static Shader textShader("assets/shaders/TextRenderingShader/TextRenderingShader.vert",
		"assets/shaders/TextRenderingShader/TextRenderingShader.frag");

	// configure the VAO/VBO
	if (!isVAOconfigured)
	{
		isVAOconfigured = true;
	
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		// 6 * 4 as for a quad we need 6 vertecies with vec2 pos and vec2 TexCoords each
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);	// GL_DYNAMIC_DRAW bc we will change it often
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	// configure the projection
	glm::mat4 projectionMatrix = glm::ortho(0.0f, target.x, target.y, 0.0f, g_closePlane, g_farPlane);



	// activate the shader and set the uniforms
	textShader.use();
	textShader.setMat4("projectionMatrix", projectionMatrix);
	textShader.setVec3("textColor", color.x, color.y, color.z);
	textShader.setInt("text", 0);

	// activate the texture unit 0
	glActiveTexture(GL_TEXTURE0);

	// bind the VAO
	glBindVertexArray(VAO);

	// iterate through all characters of the text
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		// calculate position and scale for each glyph
		Character ch = font.characters[*c];

		float xpos = pos.x + ch.Bearing.x * scale;
		// this line is not typical: float ypos = pos.y - (ch.Size.y - ch.Bearing.y) * scale; as in this case y = 0 is at the top
		float ypos = pos.y + (font.characters['H'].Bearing.y - ch.Bearing.y) * scale;

		float w = ch.Size.x * scale;
		float h = ch.Size.y * scale;

		// update VBO for each character, inverted y as we have y = 0 at the top
		float vertices[6][4] 
		{
			{ xpos,     ypos + h,   0.0f, 1.0f },
			{ xpos + w, ypos,       1.0f, 0.0f },
			{ xpos,     ypos,       0.0f, 0.0f },

			{ xpos,     ypos + h,   0.0f, 1.0f },
			{ xpos + w, ypos + h,   1.0f, 1.0f },
			{ xpos + w, ypos,       1.0f, 0.0f }
		};

		// bind the texture
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);

		// update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// render glyph texture over quad
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		pos.x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

glm::vec2 textDimensions(const Font& font, std::string text)
{
	glm::vec2 textSize{ 0.0f, 0.0f };

	// iterate through the text
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = font.characters[*c];

		// just get the highest y value
		if (textSize.y < ch.Size.y)
		{
			textSize.y = static_cast<float>(ch.Size.y);
		}
		// add to the x value
		textSize.x += (ch.Advance >> 6);
	}
	return textSize;
}

// return the scale needed to fit a given text inside a given sized object
float scaleTextToFit(const Font &font, std::string text, glm::vec2 sizeToFit)
{
	glm::vec2 currentTextDimensions{ textDimensions(font, text) };
	// leave a padding on both sides
	glm::vec2 maxSize{ sizeToFit.x * 0.8f,  sizeToFit.y * 0.5f };

	// calculate the scale factors
	glm::vec2 possibleScale{ maxSize.x / currentTextDimensions.x , maxSize.y / currentTextDimensions.y };
	
	// get the smaller size so it surely fits
	float scale{ std::min(possibleScale.x, possibleScale.y) };

	return scale;
}