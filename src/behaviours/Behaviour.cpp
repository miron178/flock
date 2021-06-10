#include "Behaviour.h"
#include "Entity.h"

//#define FLAT

Behaviour::Behaviour(const Entity* a_pAgent, const glm::vec3* a_pTarget)
	: m_pAgent(a_pAgent)
	, m_pTarget(a_pTarget)
{
	m_pModelComponent = m_pAgent->FindModelComponent();
	m_pPhysicsComponent = m_pAgent->FindPhysicsComponent();
	m_pTransformComponent = m_pAgent->FindTransformComponent();
}

glm::vec3 Behaviour::AgentPos() const
{
	glm::vec3 v3Local = m_pTransformComponent->GetEntityMatrixRow(POSITION_VECTOR);
	glm::vec3 v3World = m_pModelComponent->GetModelMatrix() * glm::vec4(v3Local, 1.0f);
	return v3World;
}

glm::vec3 Behaviour::AgentForward() const
{ 
	return glm::normalize(m_pTransformComponent->GetEntityMatrixRow(FORWARD_VECTOR));
}

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
	if (a_pSelf != a_pOther && a_pOther->FindBrainComponent())
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
