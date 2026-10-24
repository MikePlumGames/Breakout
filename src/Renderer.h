#pragma once

#include <glm/glm.hpp>	// matricies and vectores

// render a quad without texture
void renderQuad(glm::vec2 position, glm::vec2 size, float rotation, glm::vec4 color, glm::vec2 target);
// render a quad with texture
void renderQuadTexture(unsigned int texture, glm::vec2 position, glm::vec2 size, float rotation, glm::vec4 color, glm::vec2 target);
// render a simple quad
void renderQuadSimple(glm::vec2 position, float scale, glm::vec4 color, glm::vec2 target);
// render a screen quad with postprocessing shader applied
void renderPostprocessedQuad(unsigned int texture, glm::vec2 position, glm::vec2 size, float rotation, bool shake, bool confuse, bool chaos, glm::vec2 target);