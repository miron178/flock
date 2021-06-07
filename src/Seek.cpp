#include "Seek.h"

Seek::Seek(const TransformComponent* a_pAgent, const glm::vec3* a_pTarget, const glm::vec3* a_pv3Velocity)
	: Arrive(a_pAgent, a_pTarget, a_pv3Velocity, 0)
{}
