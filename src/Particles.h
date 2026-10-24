#pragma once

#include "Particle.h"

#include <glm/glm.hpp>
#include <vector>

struct Particles
{
	std::vector<Particle> particlesList;

	//int activeParticles{};	// how many particles have been already once activated
	int totalParicles{};

	// creates the list of particles
	Particles(int amountTotal);
	// update the particles
	void update(glm::vec2 objectPos, glm::vec2 objectVelocity, glm::vec2 _scale, float lifeTime);
	// draw the particles
	void draw();
	// de-activate the particles to make a full reset after the ball dies
	void fullRestart();
};