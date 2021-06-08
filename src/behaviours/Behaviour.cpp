#include "Behaviour.h"
#include "Entity.h"

#define FLAT

Behaviour::Behaviour(const TransformComponent* a_pAgent, const glm::vec3* a_pTarget, const PhysicsComponent* a_pPhysicsComponent)
	: m_pAgent(a_pAgent)
	, m_pTarget(a_pTarget)
	, m_pPhysicsComponent(a_pPhysicsComponent)
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

bool Behaviour::IsNeighbour(const Entity* a_pSelf, const Entity* a_pOther) const
{
	if (a_pSelf != a_pOther)
	{
		const TransformComponent* pTargetTransform = a_pOther->FindTransformComponent();
		glm::vec3 v3Neighbour = pTargetTransform->GetEntityMatrixRow(POSITION_VECTOR);
		float fDistanceSquared = glm::distance2(v3Neighbour, AgentPos());
		if (fDistanceSquared <= m_fNeighbourRadius * m_fNeighbourRadius)
		{
			return true;
		}
	}
	return false;
}