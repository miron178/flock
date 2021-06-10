#include "Pursue.h"
#include "PhysicsComponent.h"

Pursue::Pursue(const Entity* a_pAgent, const glm::vec3* a_pTarget)
	: Arrive(a_pAgent, a_pTarget, 0)
{}

glm::vec3 Pursue::Target()
{
	glm::vec3 v3FuturePos = TargetPos() + m_pPhysicsComponent->GetVelocity();
	return glm::vec3(v3FuturePos - AgentPos());
}
