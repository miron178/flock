#include "Pursue.h"
#include "PhysicsComponent.h"

Pursue::Pursue(const TransformComponent* a_pAgent, const glm::vec3* a_pTarget, const PhysicsComponent* a_pPhysicsComponent)
	: Arrive(a_pAgent, a_pTarget, a_pPhysicsComponent, 0)
{}

glm::vec3 Pursue::Target()
{
	glm::vec3 v3FuturePos = TargetPos() + m_pPhysicsComponent->GetVelocity();
	return glm::vec3(v3FuturePos - AgentPos());
}
