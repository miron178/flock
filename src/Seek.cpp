#include "Seek.h"

Seek::Seek(const TransformComponent* pAgent, const glm::vec3* pTarget, const glm::vec3* pv3Velocity)
	: Arrive(pAgent, pTarget, pv3Velocity, 0)
{}
