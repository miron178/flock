#include "Flee.h"

Flee::Flee(const TransformComponent* pAgent, const glm::vec3* pTarget, const glm::vec3* pv3Velocity)
	: Behaviour(pAgent, pTarget, pv3Velocity)
{}

glm::vec3 Flee::Force()
{
	glm::vec3 v3AwayDir(AgentPos() - TargetPos());
	if (glm::length2(v3AwayDir) == 0)
	{
		v3AwayDir = SphericalRand(1); //flee in any direction
	}
	else
	{
		v3AwayDir = glm::normalize(v3AwayDir);
	}
	glm::vec3 v3DesiredVelocity = ClampVelocity(v3AwayDir * m_fSpeed);
	return ClampForce(v3DesiredVelocity - *m_pv3Velocity);
}
