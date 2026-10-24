#pragma once

#include <glad/glad.h>		// openGL functions

#include <array>

// forward declarations
struct TextureAttachment;
struct Framebuffer;
struct MultisampleTextureAttachment;
struct FramebufferMultisample;

struct TextureAttachment
{
	unsigned int ID{ 0 };
	//unsigned int slot;	// GL_COLOR_ATTACHMENT 0-31
	bool isActive{ false };
	
	GLenum textureFormat{};
	GLenum textureFormatInternal{};

	GLenum wrappingMode{ GL_REPEAT };
	GLenum mipmapMinMode{ GL_LINEAR };
	GLenum mipmapMagMode{ GL_LINEAR };

	void create(unsigned int width, unsigned int height, GLenum _textureFormat = GL_RGBA, GLenum _textureFormatInternal = GL_RGBA8, GLenum _wrappingMode = GL_REPEAT,
		GLenum _mipmapMinMode = GL_LINEAR, GLenum _mipmapMagMode = GL_LINEAR);
};


struct Framebuffer
{
	// fbo
	unsigned int ID{ 0 };

	unsigned int width{};
	unsigned int height{};

	// texture attachments
	static constexpr unsigned int maxTextureAttachments{ 8 };
	std::array<TextureAttachment, maxTextureAttachments> textures;
	unsigned int activeTextureSlots{};

	// rbo
	unsigned int renderbuffer{};
	GLenum RBOformat{ GL_DEPTH24_STENCIL8 };
	GLenum RBOattachmentType{ GL_DEPTH_STENCIL_ATTACHMENT };

	Framebuffer(unsigned int _width, unsigned int _height);
	~Framebuffer();

	// disallow coping the fbo to avoid problems. Only allow passing by refrence
	Framebuffer(const Framebuffer&) = delete;
	Framebuffer& operator=(const Framebuffer&) = delete;
	
	void bind() const;
	void unbind() const;

	void addTextureAttachment(unsigned int TextureSlot, GLenum textureFormat, GLenum textureFormatInternal, GLenum wrappingMode = GL_REPEAT,
		GLenum mipmapMinMode = GL_LINEAR, GLenum mipmapMagMode = GL_LINEAR);
	void addRBO(GLenum _RBOformat = GL_DEPTH24_STENCIL8);
	void resize(unsigned int _width, unsigned int _height);
	
	void blitFBO(Framebuffer& source) const;
	void blitMultisapmledFBO(FramebufferMultisample& source) const;
};

struct MultisampleTextureAttachment
{
	unsigned int ID{ 0 };

	bool isActive{ false };

	GLenum textureFormat{};
	GLenum textureFormatInternal{};

	unsigned int samples{ 0 };

	
	void create(unsigned int width, unsigned int height, unsigned int _samples, GLenum _textureFormat = GL_RGBA, GLenum _textureFormatInternal = GL_RGBA8);
};

struct FramebufferMultisample
{
	// fbo
	unsigned int ID{ 0 };

	unsigned int width{};
	unsigned int height{};

	unsigned int samples{ 0 };

	// texture attachments
	static constexpr unsigned int maxTextureAttachments{ 8 };
	std::array<MultisampleTextureAttachment, maxTextureAttachments> textures;
	unsigned int activeTextureSlots{};

	// rbo
	unsigned int renderbuffer{};
	GLenum RBOformat{ GL_DEPTH24_STENCIL8 };
	GLenum RBOattachmentType{ GL_DEPTH_STENCIL_ATTACHMENT };

	FramebufferMultisample(unsigned int _width, unsigned int _height, unsigned int _samples);
	~FramebufferMultisample();

	// disallow coping the fbo to avoid problems. Only allow passing by refrence
	FramebufferMultisample(const FramebufferMultisample&) = delete;
	FramebufferMultisample& operator=(const FramebufferMultisample&) = delete;

	void bind() const;
	void unbind() const;

	void addTextureAttachment(unsigned int TextureSlot, GLenum textureFormat, GLenum textureFormatInternal);
	void addRBO(GLenum _RBOformat = GL_DEPTH24_STENCIL8);
	void resize(unsigned int _width, unsigned int _height);

};