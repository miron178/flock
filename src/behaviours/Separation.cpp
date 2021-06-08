#include "Separation.h"
#include "Entity.h"
#include "PhysicsComponent.h"

Separation::Separation(const Entity *a_pSelf, const std::map<const unsigned int, Entity*>& a_xEntityMap)
	: Behaviour(a_pSelf->FindTransformComponent(), nullptr, a_pSelf->FindPhysicsComponent())
	, m_pSelf(a_pSelf)
	, m_xEntityMap(a_xEntityMap)
{}

glm::vec3 Separation::Force()
{
	glm::vec3 v3Force = glm::vec3(0, 0, 0);

	unsigned int uNeighbourCount = 0;
	//loop over entities
	for (const auto xConstIter : m_xEntityMap)
	{
		const Entity* pNeighbour = xConstIter.second;
		if (IsNeighbour(m_pSelf, pNeighbour))
		{
			const TransformComponent* pNeighboutTransformComponent = pNeighbour->FindTransformComponent();
			glm::vec3 v3NeighbourPos = pNeighboutTransformComponent->GetEntityMatrixRow(POSITION_VECTOR);
			glm::vec3 v3AwayDir = AgentPos() - v3NeighbourPos;
			float fDistance = glm::length(v3AwayDir);
			if (fDistance == 0)
			{
				v3AwayDir = SphericalRand(1); //flee in any direction
			}
			else
			{
				float fLength = (m_fNeighbourRadius - fDistance) / m_fNeighbourRadius;
				v3AwayDir = v3AwayDir * fLength / fDistance;
			}
			v3Force += v3AwayDir;
			uNeighbourCount++;
		}
	}
	if (uNeighbourCount)
	{
		v3Force /= uNeighbourCount;
	}
	return ClampForce(v3Force);
}

