#include "Pursue.h"
#include "PhysicsComponent.h"

Pursue::Pursue(const TransformComponent* a_pAgent, const glm::vec3* a_pTarget, const PhysicsComponent* a_pPhysicsComponent)
	: Arrive(a_pAgent, a_pTarget, a_pPhysicsComponent, 0)
{}

glm::vec3 Pursue::Target()
{
	return glm::vec3(TargetPos() + m_pPhysicsComponent->GetVelocity() - AgentPos());
}
