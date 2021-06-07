#include "Wander.h"

Wander::Wander(const TransformComponent* a_pAgent, const glm::vec3* a_pv3Velocity)
	: Seek(a_pAgent, &m_v3Target, a_pv3Velocity)
{
	m_v3Direction = SphericalRand(m_fRadius);
	m_fMaxSpeed = m_fDistance + m_fRadius;
}


glm::vec3 Wander::Force()
{
	glm::vec3 v3Jitter = SphericalRand(m_fJitter);
	m_v3Direction = glm::normalize(m_v3Direction + v3Jitter) * m_fRadius;

	m_v3Target = AgentPos() + AgentForward() * m_fDistance + m_v3Direction;
	return Seek::Force();
}
