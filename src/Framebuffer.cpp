#include "Framebuffer.h"

#include "Breakout.h"

#include <glad/glad.h>		// openGL functions

#include <iostream>

// struct TextureAttachment	-----------------------------------------------------------------------------------------------------------------

void TextureAttachment::create(unsigned int width, unsigned int height, GLenum _textureFormat, GLenum _textureFormatInternal, GLenum _wrappingMode, GLenum _mipmapMinMode, GLenum _mipmapMagMode)
{
	// check if the formats are correct
	if (_textureFormat != GL_RGBA && _textureFormat != GL_RGB && _textureFormat != GL_RG && _textureFormat != GL_RED)
	{
		std::cout << "TextureAttachment::create error: invalid _textureFormat defaults to: GL_RGBA" << std::endl;
		_textureFormat = GL_RGBA;
	}
	textureFormat = _textureFormat;
	if (   _textureFormatInternal != GL_RGBA	&& _textureFormatInternal != GL_RGB    && _textureFormatInternal != GL_RG    && _textureFormatInternal != GL_RED
		&& _textureFormatInternal != GL_RGBA8	&& _textureFormatInternal != GL_RGB8   && _textureFormatInternal != GL_RG8   && _textureFormatInternal != GL_R8
		&& _textureFormatInternal != GL_RGBA16F && _textureFormatInternal != GL_RGB16F && _textureFormatInternal != GL_RG16F && _textureFormatInternal != GL_R16F
		&& _textureFormatInternal != GL_RGBA32F && _textureFormatInternal != GL_RGB32F && _textureFormatInternal != GL_RG32F && _textureFormatInternal != GL_R32F
		&& _textureFormatInternal != GL_SRGB8_ALPHA8 && _textureFormatInternal != GL_SRGB8)
	{
		std::cout << "TextureAttachment::create error: invalid _textureFormatInternal defaults to: GL_RGBA8" << std::endl;
		_textureFormatInternal = GL_RGBA8;
	}
	textureFormatInternal = _textureFormatInternal;
	if (_wrappingMode != GL_REPEAT && _wrappingMode != GL_MIRRORED_REPEAT && _wrappingMode != GL_CLAMP_TO_EDGE)
	{
		std::cout << "TextureAttachment::create error: invalid _wrappingMode defaults to: GL_REPEAT" << std::endl;
		_wrappingMode = GL_REPEAT;
	}
	wrappingMode = _wrappingMode;
	if (_mipmapMinMode != GL_NEAREST_MIPMAP_NEAREST && _mipmapMinMode != GL_LINEAR_MIPMAP_NEAREST &&
		_mipmapMinMode != GL_NEAREST_MIPMAP_LINEAR && _mipmapMinMode != GL_LINEAR_MIPMAP_LINEAR &&
		_mipmapMinMode != GL_LINEAR && _mipmapMinMode != GL_NEAREST)
	{
		std::cout << "TextureAttachment::create error: invalid _mipmapMinMode defaults to: GL_LINEAR" << std::endl;
		_mipmapMinMode = GL_LINEAR;
	}
	mipmapMinMode = _mipmapMinMode;
	if (_mipmapMagMode != GL_NEAREST && _mipmapMagMode != GL_LINEAR)
	{
		std::cout << "TextureAttachment::create error: invalid _mipmapMagMode defaults to: GL_LINEAR" << std::endl;
		_mipmapMagMode = GL_LINEAR;
	}
	mipmapMagMode = _mipmapMagMode;

	// check what type of data storage  to use
	GLenum dataType = GL_UNSIGNED_BYTE;

	if (textureFormatInternal == GL_RGBA16F || textureFormatInternal == GL_RGB16F ||
		textureFormatInternal == GL_RG16F || textureFormatInternal == GL_R16F)
		dataType = GL_HALF_FLOAT;
	else if (textureFormatInternal == GL_RGBA32F || textureFormatInternal == GL_RGB32F ||
		textureFormatInternal == GL_RG32F || textureFormatInternal == GL_R32F)
		dataType = GL_FLOAT;

	// destroy the texture if it is unused
	if (ID != 0)
		glDeleteTextures(1, &ID);

	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_2D, ID);
	glTexImage2D(GL_TEXTURE_2D, 0, textureFormatInternal, width, height, 0, textureFormat, dataType, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrappingMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrappingMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mipmapMinMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mipmapMagMode);

	// generate mipmap if there is a request
	if (mipmapMinMode == GL_NEAREST_MIPMAP_NEAREST || mipmapMinMode == GL_LINEAR_MIPMAP_NEAREST ||
		mipmapMinMode == GL_NEAREST_MIPMAP_LINEAR || mipmapMinMode == GL_LINEAR_MIPMAP_LINEAR)
		glGenerateMipmap(GL_TEXTURE_2D);


	isActive = true;
}

// struct Framebuffer	-----------------------------------------------------------------------------------------------------------------

Framebuffer::Framebuffer(unsigned int _width, unsigned int _height)
{
	width = _width;
	height = _height;
	glGenFramebuffers(1, &ID);
}
Framebuffer::~Framebuffer()
{
	glDeleteFramebuffers(1, &ID);
	if (renderbuffer != 0)
		glDeleteRenderbuffers(1, &renderbuffer);
	for (int i = 0; i < textures.size(); i++)
	{
		if(textures[i].isActive)
			glDeleteTextures(1, &textures[i].ID);
	}
}

void Framebuffer::bind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, ID);
	glViewport(0, 0, width, height);
}
void Framebuffer::unbind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, g_screenWidth, g_screenHeight);
}

void Framebuffer::addTextureAttachment(unsigned int TextureSlot, GLenum textureFormat, GLenum textureFormatInternal, GLenum wrappingMode, GLenum mipmapMinMode, GLenum mipmapMagMode)
{
	// check if we are trying to access a valid texture attachment	>= because maxTextureAttachments = 8 but we access with range 0-7
	if (TextureSlot >= maxTextureAttachments)
	{
		std::cout << "Framebuffer::addTextureAttachment error: Trying to create invalid texture attachment" << std::endl;
		return;
	}
	// increment active texture slots counter
	if (activeTextureSlots < maxTextureAttachments) activeTextureSlots++;

	// add the texture attachment

	// bind the fbo
	glBindFramebuffer(GL_FRAMEBUFFER, ID);
	// create
	textures[TextureSlot].create(width, height, textureFormat, textureFormatInternal, wrappingMode, mipmapMinMode, mipmapMagMode);
	// attach
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + TextureSlot, GL_TEXTURE_2D, textures[TextureSlot].ID, 0);

	// unbind the fbo
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::addRBO(GLenum _RBOformat)
{
	if (_RBOformat != GL_DEPTH_COMPONENT16 && _RBOformat != GL_DEPTH_COMPONENT24 && _RBOformat != GL_DEPTH_COMPONENT32F
		&& _RBOformat != GL_DEPTH24_STENCIL8 && _RBOformat != GL_DEPTH32F_STENCIL8)
	{
		std::cout << "Framebuffer::addRBO error: invalid _RBOformat defaults to: GL_DEPTH24_STENCIL8" << std::endl;
		_RBOformat = GL_DEPTH24_STENCIL8;
	}
	RBOformat = _RBOformat;

	// figure out what are we attaching
	if (RBOformat == GL_DEPTH_COMPONENT16 || RBOformat == GL_DEPTH_COMPONENT24 || RBOformat == GL_DEPTH_COMPONENT32F)
		RBOattachmentType = GL_DEPTH_ATTACHMENT;
	else
		RBOattachmentType = GL_DEPTH_STENCIL_ATTACHMENT;

	// bind the fbo
	glBindFramebuffer(GL_FRAMEBUFFER, ID);

	// destroy the rbo if it already exists
	if (renderbuffer != 0)
		glDeleteRenderbuffers(1, &renderbuffer);

	// create and attach the rbo
	glGenRenderbuffers(1, &renderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, RBOformat, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, RBOattachmentType, GL_RENDERBUFFER, renderbuffer);

	// unbind the fbo
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

// go through all texture attachments and the rbo, resize them and delete the data
void Framebuffer::resize(unsigned int _width, unsigned int _height)
{
	width = _width;
	height = _height;

	// bind the fbo
	glBindFramebuffer(GL_FRAMEBUFFER, ID);

	// texture attachments
	for (int i = 0; i < textures.size(); i++)
	{
		if (!textures[i].isActive) continue;
		// update the texture
		textures[i].create(width, height, textures[i].textureFormat, textures[i].textureFormatInternal, textures[i].wrappingMode, textures[i].mipmapMinMode, textures[i].mipmapMagMode);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, textures[i].ID, 0);
	}

	// rbo
	if (renderbuffer != 0)
	{
		glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, RBOformat, width, height);
	}

	// check for errors
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer::resize error: framebuffer isn't complete!" << std::endl;

	// unbind the fbo
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::blitFBO(Framebuffer& source) const	// blit the fbo to another one
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, source.ID);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, ID);
	glBlitFramebuffer(0, 0, source.width, source.height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::blitMultisapmledFBO(FramebufferMultisample& source) const	// blit the fbo to another one
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, source.ID);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, ID);
	glBlitFramebuffer(0, 0, source.width, source.height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

// struct MultisampleTextureAttachment	-----------------------------------------------------------------------------------------------------------------

void MultisampleTextureAttachment::create(unsigned int width, unsigned int height, unsigned int _samples, GLenum _textureFormat, GLenum _textureFormatInternal)
{
	// check if the formats are correct
	if (_textureFormat != GL_RGBA && _textureFormat != GL_RGB && _textureFormat != GL_RG && _textureFormat != GL_RED)
	{
		std::cout << "MultisampleTextureAttachment::create error: invalid _textureFormat defaults to: GL_RGBA" << std::endl;
		_textureFormat = GL_RGBA;
	}
	textureFormat = _textureFormat;
	if (_textureFormatInternal != GL_RGBA && _textureFormatInternal != GL_RGB && _textureFormatInternal != GL_RG && _textureFormatInternal != GL_RED
		&& _textureFormatInternal != GL_RGBA8 && _textureFormatInternal != GL_RGB8 && _textureFormatInternal != GL_RG8 && _textureFormatInternal != GL_R8
		&& _textureFormatInternal != GL_RGBA16F && _textureFormatInternal != GL_RGB16F && _textureFormatInternal != GL_RG16F && _textureFormatInternal != GL_R16F
		&& _textureFormatInternal != GL_RGBA32F && _textureFormatInternal != GL_RGB32F && _textureFormatInternal != GL_RG32F && _textureFormatInternal != GL_R32F
		&& _textureFormatInternal != GL_SRGB8_ALPHA8 && _textureFormatInternal != GL_SRGB8)
	{
		std::cout << "MultisampleTextureAttachment::create error: invalid _textureFormatInternal defaults to: GL_RGBA8" << std::endl;
		_textureFormatInternal = GL_RGBA8;
	}
	textureFormatInternal = _textureFormatInternal;

	samples = _samples;

	// destroy the texture if it is unused
	if (ID != 0)
		glDeleteTextures(1, &ID);

	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, ID);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, textureFormatInternal, width, height, GL_TRUE);


	isActive = true;
}

// struct FramebufferMultisample	-----------------------------------------------------------------------------------------------------------------

FramebufferMultisample::FramebufferMultisample(unsigned int _width, unsigned int _height, unsigned int _samples)
{
	glGenFramebuffers(1, &ID);
	width = _width;
	height = _height;
	samples = _samples;
}
FramebufferMultisample::~FramebufferMultisample()
{
	glDeleteFramebuffers(1, &ID);
	if (renderbuffer != 0)
		glDeleteRenderbuffers(1, &renderbuffer);
	for (int i = 0; i < textures.size(); i++)
	{
		if (textures[i].isActive)
			glDeleteTextures(1, &textures[i].ID);
	}
}

void FramebufferMultisample::bind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, ID);
	glViewport(0, 0, width, height);
}
void FramebufferMultisample::unbind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, g_screenWidth, g_screenHeight);
}

void FramebufferMultisample::addTextureAttachment(unsigned int TextureSlot, GLenum textureFormat, GLenum textureFormatInternal)
{
	// check if we are trying to access a valid texture attachment	>= because maxTextureAttachments = 8 but we access with range 0-7
	if (TextureSlot >= maxTextureAttachments)
	{
		std::cout << "FramebufferMultisample::addTextureAttachment error: Trying to create invalid texture attachment" << std::endl;
		return;
	}
	// increment active texture slots counter
	if (activeTextureSlots < maxTextureAttachments) activeTextureSlots++;

	// add the texture attachment

	// bind the fbo
	glBindFramebuffer(GL_FRAMEBUFFER, ID);
	// create
	textures[TextureSlot].create(width, height, samples, textureFormat, textureFormatInternal);
	// attach
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + TextureSlot, GL_TEXTURE_2D_MULTISAMPLE, textures[TextureSlot].ID, 0);

	// unbind the fbo
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FramebufferMultisample::addRBO(GLenum _RBOformat)
{
	if (_RBOformat != GL_DEPTH_COMPONENT16 && _RBOformat != GL_DEPTH_COMPONENT24 && _RBOformat != GL_DEPTH_COMPONENT32F
		&& _RBOformat != GL_DEPTH24_STENCIL8 && _RBOformat != GL_DEPTH32F_STENCIL8)
	{
		std::cout << "FramebufferMultisample::addRBO error: invalid _RBOformat defaults to: GL_DEPTH24_STENCIL8" << std::endl;
		_RBOformat = GL_DEPTH24_STENCIL8;
	}
	RBOformat = _RBOformat;

	// figure out what are we attaching
	if (RBOformat == GL_DEPTH_COMPONENT16 || RBOformat == GL_DEPTH_COMPONENT24 || RBOformat == GL_DEPTH_COMPONENT32F)
		RBOattachmentType = GL_DEPTH_ATTACHMENT;
	else
		RBOattachmentType = GL_DEPTH_STENCIL_ATTACHMENT;

	// destroy the rbo if it already exists
	if (renderbuffer != 0)
		glDeleteRenderbuffers(1, &renderbuffer);

	// bind the fbo
	glBindFramebuffer(GL_FRAMEBUFFER, ID);

	// create and attach the rbo
	glGenRenderbuffers(1, &renderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, RBOformat, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, RBOattachmentType, GL_RENDERBUFFER, renderbuffer);

	// unbind the fbo
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FramebufferMultisample::resize(unsigned int _width, unsigned int _height)
{
	width = _width;
	height = _height;

	// bind the fbo
	glBindFramebuffer(GL_FRAMEBUFFER, ID);

	// texture attachments
	for (int i = 0; i < textures.size(); i++)
	{
		if (!textures[i].isActive) continue;
		// update the texture
		textures[i].create(width, height, samples, textures[i].textureFormat, textures[i].textureFormatInternal);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D_MULTISAMPLE, textures[i].ID, 0);
	}

	// rbo
	if (renderbuffer != 0)
	{
		glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, RBOformat, width, height);
	}

	// check for errors
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "FramebufferMultisample::resize error: framebuffer isn't complete!" << std::endl;

	// unbind the fbo
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}