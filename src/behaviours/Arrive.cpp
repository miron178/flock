#include "Arrive.h"
#include "PhysicsComponent.h"

Arrive::Arrive(const TransformComponent* a_pAgent, const glm::vec3* a_pTarget, const PhysicsComponent* a_pPhysicsComponent, float a_fRadius)
	: Behaviour(a_pAgent, a_pTarget, a_pPhysicsComponent)
	, m_fRadius(a_fRadius)
{}

glm::vec3 Arrive::Target()
{
	return glm::vec3(TargetPos() - AgentPos());
}

glm::vec3 Arrive::Force()
{
	glm::vec3 v3TargetDir = Target();

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
	return ClampForce(v3DesiredVelocity - m_pPhysicsComponent->GetVelocity());
}
