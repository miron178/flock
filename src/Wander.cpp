#include "Wander.h"

Wander::Wander(const TransformComponent* a_pAgent, const glm::vec3* a_pv3Velocity)
	: Seek(a_pAgent, &m_v3WanderPoint, a_pv3Velocity)
{
	NewTarget();
}

bool Wander::Arrived() const
{
	return glm::distance(m_v3WanderPoint, AgentPos()) <= m_fJitter;
}

void Wander::NewTarget()
{
	glm::vec3 v3SphereOrigin = AgentPos() + (AgentForward() * m_fDistance);
	glm::vec3 v3RandomPointOnSphere = SphericalRand(m_fRadius);
	m_v3WanderPoint = v3SphereOrigin + v3RandomPointOnSphere;
}

void Wander::AddJitter()
{
	glm::vec3 v3Jitter = SphericalRand(m_fJitter);
	m_v3WanderPoint += v3Jitter;
}

glm::vec3 Wander::Force()
{
	if (Arrived())
	{
		NewTarget();
	}
	else
	{
		AddJitter();
	}

	return Seek::Force();
}
