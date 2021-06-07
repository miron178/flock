#include "Seek.h"

Seek::Seek(const TransformComponent* a_pAgent, const glm::vec3* a_pTarget, const PhysicsComponent* a_pPhysicsComponent)
	: Arrive(a_pAgent, a_pTarget, a_pPhysicsComponent, 0)
{}
