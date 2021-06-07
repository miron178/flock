#include "BrainComponent.h"

#include "Entity.h"
#include "PhysicsComponent.h"
#include "TransformComponent.h"
#include "Seek.h"
#include "Wander.h"

#include <cassert>

const float fNEIGHBOURHOOD_RADIUS = 2.0f;

BrainComponent::BrainComponent(Entity* a_pOwner)
	: Component(a_pOwner, COMPONENT_TYPE::BRAIN)
{}

BrainComponent::~BrainComponent()
{
	ClearBehaviours();
}

void BrainComponent::Update(float a_fDeltaTime)
{
	//get owner
	Entity* pEntity = GetOwnerEntity();
	if (!pEntity) return;

	//get transform
	TransformComponent* pTransComp = pEntity->FindTransformComponent();
	if (!pTransComp) return;

	PhysicsComponent* pPhysicsComponent = pEntity->FindPhysicsComponent();
	if (!pPhysicsComponent) return;

	//calculate forces
	glm::vec3 v3FinalForce(0.0f);

	for (auto item : m_behaviours)
	{
		v3FinalForce += item.second->Force();
	}

	pPhysicsComponent->AddForce(v3FinalForce);
}

glm::vec3 BrainComponent::CalculateSeporationForce()
{
	//Seporation Vector
	glm::vec3 v3SeporationVelocity(0.0f);
	unsigned int uNeighbourCount = 0;

	//this transform
	const TransformComponent* pLocalTransform = GetOwnerEntity()->FindTransformComponent();

	//this pos
	glm::vec3 v3LocalPos = pLocalTransform->GetEntityMatrixRow(POSITION_VECTOR);

	//itorator
	const std::map<const unsigned int, Entity*>& xEntityMap = Entity::GetEntityMap();
	std::map<const unsigned int, Entity*>::const_iterator xConstIter;

	//loop over entities
	for (xConstIter = xEntityMap.begin(); xConstIter != xEntityMap.end(); ++xConstIter)
	{
		const Entity* pTarget = xConstIter->second;
		if (pTarget->GetEntityID() != GetOwnerEntity()->GetEntityID())
		{
			const TransformComponent* pTargetTransform = pTarget->FindTransformComponent();

			//find distance
			glm::vec3 v3TargetPos = pTargetTransform->GetEntityMatrixRow(POSITION_VECTOR);
			float fDistanceBetween = glm::length(v3TargetPos - v3LocalPos);

			//neighbourhood distance
			if(fDistanceBetween < fNEIGHBOURHOOD_RADIUS)
			{
				float fStrength = 1 - fDistanceBetween / fNEIGHBOURHOOD_RADIUS;
				v3SeporationVelocity += glm::normalize(v3LocalPos - v3TargetPos) * fStrength;
				uNeighbourCount++;
			}
		}
	}

	if (uNeighbourCount)
	{
		v3SeporationVelocity /= uNeighbourCount;
	}

	return v3SeporationVelocity;
}

glm::vec3 BrainComponent::CalculateAlignmentForce()
{
	//alignment velocity
	glm::vec3 v3AlignmentVelocity(0.0f);
	unsigned int uNeighbourCount = 0;

	//this transform
	const TransformComponent* pLocalTransform = GetOwnerEntity()->FindTransformComponent();

	//this pos
	glm::vec3 v3LocalPos = pLocalTransform->GetEntityMatrixRow(POSITION_VECTOR);

	//itorator
	const std::map<const unsigned int, Entity*>& xEntityMap = Entity::GetEntityMap();
	std::map<const unsigned int, Entity*>::const_iterator xConstIter;

	//loop over entities
	for (xConstIter = xEntityMap.begin(); xConstIter != xEntityMap.end(); ++xConstIter)
	{
		const Entity* pTarget = xConstIter->second;
		if (pTarget->GetEntityID() != GetOwnerEntity()->GetEntityID())
		{
			const TransformComponent* pTargetTransform = pTarget->FindTransformComponent();
			const BrainComponent* pTargetBrain = pTarget->FindBrainComponent();
			const PhysicsComponent* pTargetPhysics = pTarget->FindPhysicsComponent();

			//find distance
			glm::vec3 v3TargetPos = pTargetTransform->GetEntityMatrixRow(POSITION_VECTOR);
			float fDistanceBetween = glm::length(v3TargetPos - v3LocalPos);

			//neighbourhood distance
			if (fDistanceBetween < fNEIGHBOURHOOD_RADIUS)
			{
				float fStrength = 1 - fDistanceBetween / fNEIGHBOURHOOD_RADIUS;
				v3AlignmentVelocity += pTargetPhysics->GetVelocity() * fStrength;
				uNeighbourCount++;
			}
		}
	}

	if (uNeighbourCount)
	{
		v3AlignmentVelocity /= uNeighbourCount;
	}

	return v3AlignmentVelocity;
}

glm::vec3 BrainComponent::CalculateCohesionForce()
{
	//alignment velocity
	glm::vec3 v3CohesionVelocity(0.0f);
	unsigned int uNeighbourCount = 0;

	//this transform
	const TransformComponent* pLocalTransform = GetOwnerEntity()->FindTransformComponent();

	//this pos
	glm::vec3 v3LocalPos = pLocalTransform->GetEntityMatrixRow(POSITION_VECTOR);

	//itorator
	const std::map<const unsigned int, Entity*>& xEntityMap = Entity::GetEntityMap();
	std::map<const unsigned int, Entity*>::const_iterator xConstIter;

	//loop over entities
	for (xConstIter = xEntityMap.begin(); xConstIter != xEntityMap.end(); ++xConstIter)
	{
		const Entity* pTarget = xConstIter->second;
		if (pTarget->GetEntityID() != GetOwnerEntity()->GetEntityID())
		{
			const TransformComponent* pTargetTransform = pTarget->FindTransformComponent();

			//find distance
			glm::vec3 v3TargetPos = pTargetTransform->GetEntityMatrixRow(POSITION_VECTOR);
			float fDistanceBetween = glm::length(v3TargetPos - v3LocalPos);

			//neighbourhood distance
			if (fDistanceBetween < fNEIGHBOURHOOD_RADIUS)
			{
				float fStrength = 1 - fDistanceBetween / fNEIGHBOURHOOD_RADIUS;
				v3CohesionVelocity += v3TargetPos * fStrength;
				uNeighbourCount++;
			}
		}
	}

	if (glm::length(v3CohesionVelocity) > 0.0f && uNeighbourCount)
	{
		v3CohesionVelocity /= uNeighbourCount;
		v3CohesionVelocity = glm::normalize(v3CohesionVelocity- v3LocalPos);
	}

	return v3CohesionVelocity;
}


Behaviour* BrainComponent::AddBehaviour(unsigned a_priority, Behaviour* a_pBehaviour)
{
	assert(a_pBehaviour != nullptr);
	Behaviour* pPrev = nullptr;

	auto previous = m_behaviours.find(a_priority);
	if (previous != m_behaviours.end())
	{
		pPrev = previous->second;
	}
	else
	{
		m_behaviours[a_priority] = a_pBehaviour;
	}
	return pPrev;
}

Behaviour* BrainComponent::GetBehaviour(unsigned a_priority) const
{
	Behaviour* pPrev = nullptr;
	auto previous = m_behaviours.find(a_priority);
	if (previous != m_behaviours.end())
	{
		pPrev = previous->second;
	}
	return pPrev;
}

Behaviour* BrainComponent::RemoveBehaviour(unsigned a_priority)
{
	Behaviour* pPrev = nullptr;
	auto previous = m_behaviours.find(a_priority);
	if (previous != m_behaviours.end())
	{
		pPrev = previous->second;
		m_behaviours.erase(a_priority);
	}
	return pPrev;
}


void BrainComponent::DeleteBehaviour(unsigned a_priority)
{
	auto previous = m_behaviours.find(a_priority);
	if (previous != m_behaviours.end())
	{
		delete previous->second;
		m_behaviours.erase(a_priority);
	}
}

void BrainComponent::ClearBehaviours()
{
	for (auto item : m_behaviours)
	{
		delete item.second;
	}
	m_behaviours.clear();
}
