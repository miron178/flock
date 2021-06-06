#include "Wander.h"

#define FLAT

Wander::Wander(const TransformComponent* pAgent, const glm::vec3* pv3Velocity)
	: Seek(pAgent, &m_v3WanderPoint, pv3Velocity)
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
#ifdef FLAT
	glm::vec2 v3RandomPointOnCircle = glm::circularRand(m_fRadius);
	glm::vec3 v3RandomPointOnSphere = glm::vec3(v3RandomPointOnCircle.x, 0, v3RandomPointOnCircle.y);
#else
	glm::vec3 v3RandomPointOnSphere = glm::sphericalRand(m_fRadius);
#endif
	m_v3WanderPoint = v3SphereOrigin + v3RandomPointOnSphere;
}

void Wander::AddJitter()
{
#ifdef FLAT
	glm::vec2 v2Jitter = glm::circularRand(m_fJitter);
	glm::vec3 v3Jitter = glm::vec3(v2Jitter.x, 0, v2Jitter.y);
#else
	glm::vec3 v3Jitter = glm::sphericalRand(m_fJitter);
#endif
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
