#include "TextureLoader.h"

#include <glad/glad.h>
#include <stb_image.h>

#include <iostream>


void flipTexturesOnLoad(bool state)
{
	stbi_set_flip_vertically_on_load(state);
}

// loading textures
unsigned int loadTexture(char const* path, GLint wrapingMode, GLint mipmapMinMode, GLint mipmapMagMode, bool useSRGB)
{
	// wrapingMode: GL_REPEAT, GL_MIRRORED_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER
	// if GL_CLAMP_TO_BORDER:
	// glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	// filtering mode: GL_NEAREST, GL_LINEAR
	//				choose mipmap				sample the color
	// for mipmaps: GL_LINEAR/NEAREST_MIPMAP_LINEAR/NEAREST

	// check if user values are corect and if not set default values
	// exluding GL_CLAMP_TO_BORDER because requires a border color
	if (wrapingMode != GL_REPEAT && wrapingMode != GL_MIRRORED_REPEAT &&
		wrapingMode != GL_CLAMP_TO_EDGE)
		wrapingMode = GL_REPEAT;

	if (mipmapMinMode != GL_NEAREST_MIPMAP_NEAREST && mipmapMinMode != GL_LINEAR_MIPMAP_NEAREST &&
		mipmapMinMode != GL_NEAREST_MIPMAP_LINEAR && mipmapMinMode != GL_LINEAR_MIPMAP_LINEAR &&
		mipmapMinMode != GL_LINEAR && mipmapMinMode != GL_NEAREST)
		mipmapMinMode = GL_LINEAR_MIPMAP_LINEAR;

	if (mipmapMagMode != GL_NEAREST && mipmapMagMode != GL_LINEAR)
		mipmapMagMode = GL_LINEAR;

	// generate a texture object
	unsigned int textureID;
	glGenTextures(1, &textureID);

	// read the data from the path
	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		// depending on nrComponents it has or has't transparency - A value
		GLenum format{};
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 2)
			format = GL_RG;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		GLenum formatInternal{};
		if (useSRGB && nrComponents == 4)
			formatInternal = GL_SRGB8_ALPHA8;
		else if (useSRGB && nrComponents == 3)
			formatInternal = GL_SRGB8;
		else if (nrComponents == 4)
			formatInternal = GL_RGBA8;
		else if (nrComponents == 3)
			formatInternal = GL_RGB8;
		else if (nrComponents == 2)
			format = GL_RG8;
		else if (nrComponents == 1)
			format = GL_R8;

		glBindTexture(GL_TEXTURE_2D, textureID);
		// creating the texture
		glTexImage2D(GL_TEXTURE_2D, 0, formatInternal, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		// generating mipmap (low res textures for far away)
		glGenerateMipmap(GL_TEXTURE_2D);

		// set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapingMode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapingMode);
		// set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mipmapMinMode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mipmapMagMode);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}