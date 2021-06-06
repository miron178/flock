#include "Behaviour.h"

Behaviour::Behaviour(const TransformComponent* pAgent, const glm::vec3* pTarget, const glm::vec3* pv3Velocity)
	: m_pAgent(pAgent)
	, m_pTarget(pTarget)
	, m_pv3Velocity(pv3Velocity)
{}

glm::vec3 Behaviour::Clamp(const glm::vec3& v3, float max) const
{
	return glm::length(v3) > max ? glm::normalize(v3) * max : v3;
}
