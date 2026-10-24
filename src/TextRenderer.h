#pragma once

#include <glm/glm.hpp>

#include <array>
#include <string>

struct Character
{
	unsigned int TextureID;  // ID handle of the glyph texture
	glm::ivec2   Size;       // Size of glyph
	glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
	unsigned int Advance;    // Offset to advance to next glyph
};

struct Font
{
	std::array<Character, 128> characters{};	// all characters
	unsigned int fontSize{};

	void loadFont(const char* path, unsigned int _fontSize);
};

struct Fonts
{
	inline static Font MartianMonoRegular;
	inline static Font MartianMonoBold;

	static void loadAllFonts()
	{
		MartianMonoRegular.loadFont("assets/fonts/Martian Mono/content/static/MartianMono-regular.ttf", 30);
		MartianMonoBold.loadFont("assets/fonts/Martian Mono/content/static/MartianMono-bold.ttf", 30);
	}
};

// render text
void renderText(const Font& font, std::string text, glm::vec2 pos, float scale, glm::vec3 color, glm::vec2 target);

// get the dimensions for a given text
glm::vec2 textDimensions(const Font& font, std::string text);

// return the scale needed to fit a given text inside a given sized object
float scaleTextToFit(const Font& font, std::string text, glm::vec2 sizeToFit);