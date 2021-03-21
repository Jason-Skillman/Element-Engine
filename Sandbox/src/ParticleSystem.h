#pragma once

#include <Element.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct ParticleProps {
	glm::vec2 position;
	glm::vec2 velocity, velocityVariation;
	glm::vec4 colorBegin, colorEnd;
	float sizeBegin, sizeEnd, sizeVariation;
	float lifeTime = 1.0f;
};

class ParticleSystem {
private:
	struct Particle {
		glm::vec2 position;
		glm::vec2 velocity;
		glm::vec4 colorBegin, colorEnd;
		float rotation = 0.0f;
		float sizeBegin, sizeEnd;

		float lifeTime = 1.0f;
		float lifeRemaining = 0.0f;

		bool active = false;
	};
	
	std::vector<Particle> particlePool;
	uint32_t poolIndex;
	
public:
	ParticleSystem(uint32_t maxParticles = 1000);

	void OnUpdate(Element::Timestep ts);
	void OnRender(Element::OrthographicCamera& camera);

	void Emit(const ParticleProps& particleProps);
};
