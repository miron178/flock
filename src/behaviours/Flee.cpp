#include "Flee.h"
#include "PhysicsComponent.h"

Flee::Flee(const Entity* a_pAgent, const glm::vec3* a_pTarget)
	: Behaviour(a_pAgent, a_pTarget)
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
	return (v3DesiredVelocity - m_pPhysicsComponent->GetVelocity()) * m_fScaleFactor;
}
