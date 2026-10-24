#include "Particles.h"

#include "Breakout.h"
#include "Particle.h"
#include "Renderer.h"
#include "Textures.h"

#include <glm/glm.hpp>
#include <vector>

// creates the list of particles
Particles::Particles(int amountTotal)
{
	totalParicles = amountTotal;
	particlesList.resize(totalParicles);
}
// update all particles
void Particles::update(glm::vec2 _pos, glm::vec2 _velocity, glm::vec2 _scale, float _maxLife)
{
	int particlesRecycled{ 0 };

	for (int i = 0; i < particlesList.size(); i++)
	{
		// refrence to avoid calling at(i) many times and simplify the code
		Particle& particle = particlesList.at(i);

		// spawn new particles
		if (!particle.isActive)
		{
			particle.reset(_pos, _velocity, _scale, _maxLife);
			particle.isActive = true;
			break;	// so we don't update it or draw anything else and only 1 particle is drawn at the begining
		}

		// reduce the life of the particle
		particle.life -= g_deltaTime;

		// reset dead particles
		if (particle.life <= 0.0f && particlesRecycled < 2)
		{
			particle.reset(_pos, _velocity, _scale, _maxLife);
			particlesRecycled++;
			continue;
		}

		// calcuate how to decrease the opacity
		float lifePercentage{ particle.life / particle.maxLife };
		// update the color
		particle.color.a = std::abs(lifePercentage);

		// update the velocity
		particle.velocity.x *= lifePercentage;
		particle.velocity.y *= lifePercentage;

		// update the position
		particle.pos -= particle.velocity * g_deltaTime;
	}
}
// draw the particles
void Particles::draw()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);	// additive

	for (int i = 0; i < particlesList.size(); i++)
	{
		Particle& particle = particlesList.at(i);

		if (particle.life <= 0.0f) continue;	// don't render the dead particles

		renderQuadTexture(Textures::particle, particle.pos, particle.scale, particle.rotation, particle.color, glm::vec2(g_gameWidthFloat, g_gameHeightFloat));
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
// de-activate the particles to make a full reset after the ball dies
void Particles::fullRestart()
{
	for (int i = 0; i < particlesList.size(); i++)
	{
		// completely restarts the particles
		particlesList[i].isActive = false;
		particlesList[i].life = 0.0f;	// so the particles are not rendered
	}
}