#include "Seek.h"

Seek::Seek(const TransformComponent* pAgent, const glm::vec3* pTarget, const glm::vec3* pv3Velocity)
	: Behaviour(pAgent, pTarget, pv3Velocity)
{}

glm::vec3 Seek::Force()
{
	glm::vec3 v3TargetDir(TargetPos() - AgentPos());
	if (glm::length2(v3TargetDir) == 0)
	{
		return glm::vec3(0, 0, 0); //already there
	}
	v3TargetDir = glm::normalize(v3TargetDir);
	glm::vec3 v3DesiredVelocity = ClampVelocity(v3TargetDir * m_fSpeed);
	return ClampForce(v3DesiredVelocity - *m_pv3Velocity);
}
