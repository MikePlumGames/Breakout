#pragma once

#include <glm/glm.hpp>

struct Particle
{
	glm::vec2 pos{ 0.0f, 0.0f };
	glm::vec2 scale{ 0.0f, 0.0f };
	glm::vec2 velocity{ 0.0f, 0.0f };

	glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	float rotation{ 0.0f };

	float life{ 0.0f };
	float maxLife{ 0.0f };

	bool isActive{ false };	// if the particle has already been reseted and activated once

	// reset a particle
	void reset(glm::vec2 _pos, glm::vec2 _velocity, glm::vec2 _scale, float _maxLife);
};