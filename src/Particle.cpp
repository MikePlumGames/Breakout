#include "Particle.h"

#include <random>

// reset a particle
void Particle::reset(glm::vec2 _pos, glm::vec2 _velocity, glm::vec2 _scale, float _maxLife)
{
	// generate a random number
	static std::random_device seed;
	static std::mt19937 gen(seed());
	std::uniform_real_distribution<float> random(-0.5f, 0.5f);
	std::uniform_real_distribution<float> randomVelocity(0.8f, 1.1f);
	std::uniform_real_distribution<float> randomRotation(0.0f, 90.0f);

	maxLife = _maxLife;
	life = maxLife;
	color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	pos = _pos + glm::vec2(random(gen), random(gen));
	velocity = _velocity * glm::vec2(randomVelocity(gen), randomVelocity(gen));
	scale = _scale + glm::vec2(random(gen), random(gen));
	rotation = randomRotation(gen);
}