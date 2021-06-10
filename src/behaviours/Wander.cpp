#include "Wander.h"

Wander::Wander(const Entity* a_pAgent)
	: Seek(a_pAgent, &m_v3Target)
{
	m_v3Direction = SphericalRand(m_fRadius);
	m_fMaxSpeed = m_fDistance + m_fRadius;
}


glm::vec3 Wander::Force()
{
	glm::vec3 v3Jitter = m_fJitter ? SphericalRand(m_fJitter) : glm::vec3(0,0,0);
	m_v3Direction = glm::normalize(m_v3Direction + v3Jitter) * m_fRadius;

	m_v3Target = AgentPos() + glm::normalize(AgentForward()) * m_fDistance + m_v3Direction;
	return Seek::Force();
}
