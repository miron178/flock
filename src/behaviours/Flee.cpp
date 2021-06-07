#include "Flee.h"
#include "PhysicsComponent.h"

Flee::Flee(const TransformComponent* a_pAgent, const glm::vec3* a_pTarget, const PhysicsComponent* a_pPhysicsComponent)
	: Arrive(a_pAgent, a_pTarget, a_pPhysicsComponent, 0)
{}

glm::vec3 Flee::Target()
{
	return glm::vec3(AgentPos() - TargetPos());
}
