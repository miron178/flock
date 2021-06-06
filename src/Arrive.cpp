#include "Arrive.h"

Arrive::Arrive(const TransformComponent* pAgent, const glm::vec3* pTarget, const glm::vec3* pv3Velocity, float fRadius)
	: Behaviour(pAgent, pTarget, pv3Velocity)
	, m_fRadius(fRadius)
{}

glm::vec3 Arrive::Force()
{
	glm::vec3 v3TargetDir(TargetPos() - AgentPos());
	float fDistance = glm::length(v3TargetDir);
	if (fDistance == 0)
	{
		return glm::vec3(0, 0, 0); //already there
	}
	v3TargetDir = glm::normalize(v3TargetDir);
	
	float fMultiplier = 1.0f;
	if (fDistance < m_fRadius)
	{
		fMultiplier = fDistance / m_fRadius;
	}
	glm::vec3 v3DesiredVelocity = ClampVelocity(v3TargetDir * m_fSpeed * fMultiplier);
	return ClampForce(v3DesiredVelocity - *m_pv3Velocity);
}
