#include "Behaviour.h"

#define FLAT

Behaviour::Behaviour(const TransformComponent* a_pAgent, const glm::vec3* a_pTarget, const glm::vec3* a_pv3Velocity)
	: m_pAgent(a_pAgent)
	, m_pTarget(a_pTarget)
	, m_pv3Velocity(a_pv3Velocity)
{}

glm::vec3 Behaviour::Clamp(const glm::vec3& a_v3, float a_max) const
{
	return glm::length(a_v3) > a_max ? glm::normalize(a_v3) * a_max : a_v3;
}

glm::vec3 Behaviour::SphericalRand(float fRadius)
{
#ifdef FLAT
	glm::vec2 v3RandomPointOnCircle = glm::circularRand(fRadius);
	glm::vec3 v3RandomPointOnSphere = glm::vec3(v3RandomPointOnCircle.x, 0, v3RandomPointOnCircle.y);
#else
	glm::vec3 v3RandomPointOnSphere = glm::sphericalRand(fRadius);
#endif
	return v3RandomPointOnSphere;
}
