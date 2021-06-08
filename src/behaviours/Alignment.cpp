#include "Alignment.h"
#include "Entity.h"
#include "PhysicsComponent.h"

Alignment::Alignment(const Entity *a_pSelf, const std::map<const unsigned int, Entity*>& a_xEntityMap)
	: Behaviour(a_pSelf->FindTransformComponent(), nullptr, a_pSelf->FindPhysicsComponent())
	, m_pSelf(a_pSelf)
	, m_xEntityMap(a_xEntityMap)
{}

glm::vec3 Alignment::Force()
{
	glm::vec3 v3Velocity = glm::vec3(0, 0, 0);
	glm::vec3 v3Force = glm::vec3(0, 0, 0);

	unsigned int uNeighbourCount = 0;
	//loop over entities
	for (const auto xConstIter : m_xEntityMap)
	{
		const Entity* pNeighbour = xConstIter.second;
		if (IsNeighbour(m_pSelf, pNeighbour))
		{
			const PhysicsComponent* pNeighboutPhysicsComponent = pNeighbour->FindPhysicsComponent();
			v3Velocity += pNeighboutPhysicsComponent->GetVelocity();
			uNeighbourCount++;
		}
	}
	if (uNeighbourCount)
	{
		v3Velocity /= uNeighbourCount;
		v3Force = v3Velocity - AgentVelocity();
	}
	return ClampForce(v3Force);
}

