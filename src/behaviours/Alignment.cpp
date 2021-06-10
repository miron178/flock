#include "Alignment.h"
#include "Entity.h"
#include "PhysicsComponent.h"

Alignment::Alignment(const Entity *a_pAgent, const std::map<const unsigned int, Entity*>& a_xEntityMap)
	: Behaviour(a_pAgent, nullptr)
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
		if (IsNeighbour(m_pAgent, pNeighbour))
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
	return v3Force * m_fScaleFactor;
}
