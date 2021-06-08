#include "Cohesion.h"
#include "Entity.h"
#include "PhysicsComponent.h"

Cohesion::Cohesion(const Entity *a_pSelf, const std::map<const unsigned int, Entity*>& a_xEntityMap)
	: Arrive(a_pSelf->FindTransformComponent(), nullptr, a_pSelf->FindPhysicsComponent())
	, m_pSelf(a_pSelf)
	, m_xEntityMap(a_xEntityMap)
{}

glm::vec3 Cohesion::Target()
{
	glm::vec3 v3Target = glm::vec3(0, 0, 0);

	unsigned int uNeighbourCount = 0;
	//loop over entities
	for (const auto xConstIter : m_xEntityMap)
	{
		const Entity* pNeighbour = xConstIter.second;
		if (IsNeighbour(m_pSelf, pNeighbour))
		{
			const TransformComponent* pNeighboutTransformComponent = pNeighbour->FindTransformComponent();
			glm::vec3 v3NeighbourPos = pNeighboutTransformComponent->GetEntityMatrixRow(POSITION_VECTOR);
			v3Target += v3NeighbourPos;
			uNeighbourCount++;
		}
	}
	if (uNeighbourCount)
	{
		v3Target /= uNeighbourCount;
	}
	return v3Target;
}

