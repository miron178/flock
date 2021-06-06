#include "Seek.h"

Seek::Seek(const TransformComponent* pAgent, const glm::vec3* pTarget, const glm::vec3* pv3Velocity)
	: Behaviour(pAgent, pTarget, pv3Velocity)
{}

glm::vec3 Seek::Force()
{
	glm::vec3 v3Force(0, 0, 0);
	glm::vec3 v3TargetDir(TargetPos() - AgentPos());
	if (glm::length2(v3TargetDir) > 0)
	{
		glm::vec3 v3DesiredVelocity = ClampVelocity(glm::normalize(v3TargetDir) * m_fSpeed);
		v3Force = ClampForce(v3DesiredVelocity - *m_pv3Velocity);
	}
	return v3Force;
}
