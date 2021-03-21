#include "ParticleSystem.h"

#include <Element.h>

#include <glm/gtc/constants.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/compatibility.hpp>

ParticleSystem::ParticleSystem(uint32_t maxParticles)
	: poolIndex(maxParticles - 1) {
	
	particlePool.resize(maxParticles);
}

void ParticleSystem::OnUpdate(Element::Timestep ts) {
	for(auto& particle : particlePool) {
		if(!particle.active)
			continue;

		if(particle.lifeRemaining <= 0.0f) {
			particle.active = false;
			continue;
		}

		particle.lifeRemaining -= ts;
		particle.position += particle.velocity * (float)ts;
		particle.rotation += 0.01f * ts;
	}
}

void ParticleSystem::OnRender(Element::OrthographicCamera& camera) {
	Element::Renderer2D::BeginScene(camera);
	for(auto& particle : particlePool) {
		if(!particle.active)
			continue;

		// Fade away particles
		float life = particle.lifeRemaining / particle.lifeTime;
		glm::vec4 color = glm::lerp(particle.colorEnd, particle.colorBegin, life);
		//color.a = color.a * life;

		float size = glm::lerp(particle.sizeEnd, particle.sizeBegin, life);
		//glm::vec3 position = particle.Position


		Element::Renderer2D::DrawProporties drawProps;
		drawProps.position = { particle.position, 0 };
		drawProps.rotation = glm::radians(particle.rotation);
		drawProps.scale = { size, size };
		drawProps.color = color;
		Element::Renderer2D::DrawQuad(drawProps);
	}
	Element::Renderer2D::EndScene();
}

void ParticleSystem::Emit(const ParticleProps& particleProps) {
	Particle& particle = particlePool[poolIndex];
	particle.active = true;
	particle.position = particleProps.position;
	particle.rotation = Element::Random::Float() * 2.0f * glm::pi<float>();

	// Velocity
	particle.velocity = particleProps.velocity;
	particle.velocity.x += particleProps.velocityVariation.x * (Element::Random::Float() - 0.5f);
	particle.velocity.y += particleProps.velocityVariation.y * (Element::Random::Float() - 0.5f);

	// Color
	particle.colorBegin = particleProps.colorBegin;
	particle.colorEnd = particleProps.colorEnd;

	particle.lifeTime = particleProps.lifeTime;
	particle.lifeRemaining = particleProps.lifeTime;
	particle.sizeBegin = particleProps.sizeBegin + particleProps.sizeVariation * (Element::Random::Float() - 0.5f);
	particle.sizeEnd = particleProps.sizeEnd;

	poolIndex = --poolIndex % particlePool.size();
}
