#include "ParticleSystem.h"

#include "Element.h"

#include <glm/gtc/constants.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/compatibility.hpp>

ParticleSystem::ParticleSystem(uint32_t maxParticles)
	: m_PoolIndex(maxParticles - 1) {
	
	m_ParticlePool.resize(maxParticles);
}

void ParticleSystem::OnUpdate(Element::Timestep ts) {
	for(auto& particle : m_ParticlePool) {
		if(!particle.Active)
			continue;

		if(particle.LifeRemaining <= 0.0f) {
			particle.Active = false;
			continue;
		}

		particle.LifeRemaining -= ts;
		particle.Position += particle.Velocity * (float)ts;
		particle.Rotation += 0.01f * ts;
	}
}

void ParticleSystem::OnRender(Element::OrthographicCamera& camera) {
	Element::Renderer2D::BeginScene(camera);
	for(auto& particle : m_ParticlePool) {
		if(!particle.Active)
			continue;

		// Fade away particles
		float life = particle.LifeRemaining / particle.LifeTime;
		glm::vec4 color = glm::lerp(particle.ColorEnd, particle.ColorBegin, life);
		//color.a = color.a * life;

		float size = glm::lerp(particle.SizeEnd, particle.SizeBegin, life);
		//glm::vec3 position = particle.Position


		Element::Renderer2D::DrawProporties drawProps;
		drawProps.position = { particle.Position, 0 };
		drawProps.rotation = glm::radians(particle.Rotation);
		drawProps.scale = { size, size };
		drawProps.color = color;
		Element::Renderer2D::DrawQuad(drawProps);
	}
	Element::Renderer2D::EndScene();
}

void ParticleSystem::Emit(const ParticleProps& particleProps) {
	Particle& particle = m_ParticlePool[m_PoolIndex];
	particle.Active = true;
	particle.Position = particleProps.Position;
	particle.Rotation = Element::Random::Float() * 2.0f * glm::pi<float>();

	// Velocity
	particle.Velocity = particleProps.Velocity;
	particle.Velocity.x += particleProps.VelocityVariation.x * (Element::Random::Float() - 0.5f);
	particle.Velocity.y += particleProps.VelocityVariation.y * (Element::Random::Float() - 0.5f);

	// Color
	particle.ColorBegin = particleProps.ColorBegin;
	particle.ColorEnd = particleProps.ColorEnd;

	particle.LifeTime = particleProps.LifeTime;
	particle.LifeRemaining = particleProps.LifeTime;
	particle.SizeBegin = particleProps.SizeBegin + particleProps.SizeVariation * (Element::Random::Float() - 0.5f);
	particle.SizeEnd = particleProps.SizeEnd;

	m_PoolIndex = --m_PoolIndex % m_ParticlePool.size();
}
