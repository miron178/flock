#include "BrainComponent.h"

#include "Entity.h"
#include "TransformComponent.h"
#include "Seek.h"
#include "Wander.h"

#include <cassert>

const float fSPEED = 1.0f;
const float fNEIGHBOURHOOD_RADIUS = 2.0f;

const float fJITTER = 0.5f;
const float fWANDER_RADIUS = 4.0f;
const float fCIRCLE_FORWARD_MULTIPLIER = 1.0f;

BrainComponent::BrainComponent(Entity* a_pOwner)
	: Component(a_pOwner)
	, m_v3CurrentVelocity(0.0f)
	, m_v3WanderPoint(0.0f)
{
	m_eComponentType = BRAIN;
}

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
	TransformComponent* pTransComp = static_cast<TransformComponent*>(pEntity->FindComponentOfType(TRANSFORM));
	if (!pTransComp) return;

	//get vectors
	glm::vec3 v3Forward    = pTransComp->GetEntityMatrixRow(FORWARD_VECTOR);
	glm::vec3 v3CurrentPos = pTransComp->GetEntityMatrixRow(POSITION_VECTOR);

	//calculate forces
	glm::vec3 v3FinalForce(0.0f);

	for (auto item : m_behaviours)
	{
		v3FinalForce += item.second->Force();
	}

	//velocity
	m_v3CurrentVelocity += v3FinalForce / m_fMass;
	m_v3CurrentVelocity = glm::clamp(m_v3CurrentVelocity, glm::vec3(-10.0f, 0.0f, -10.0f), glm::vec3(10.0f, 0.0f, 10.0f));
	v3CurrentPos += m_v3CurrentVelocity * a_fDeltaTime;

	v3Forward = m_v3CurrentVelocity;
	if (glm::length(v3Forward) > 0.0f)
	{
		v3Forward = glm::normalize(v3Forward);

		glm::vec3 v3Up = pTransComp->GetEntityMatrixRow(UP_VECTOR);
		glm::vec3 v3Right = glm::cross(v3Up, v3Forward);

		//matrix
		pTransComp->SetEntityMatrixRow(RIGHT_VECTOR, v3Right);
		pTransComp->SetEntityMatrixRow(FORWARD_VECTOR, v3Forward);
	}
	pTransComp->SetEntityMatrixRow(POSITION_VECTOR, v3CurrentPos);
}

glm::vec3 BrainComponent::CalculateSeporationForce()
{
	//Seporation Vector
	glm::vec3 v3SeporationVelocity(0.0f);
	unsigned int uNeighbourCount = 0;

	//this transform
	const TransformComponent* pLocalTransform = static_cast<TransformComponent*>(GetOwnerEntity()->FindComponentOfType(TRANSFORM));

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
			const TransformComponent* pTargetTransform = static_cast<TransformComponent*>(pTarget->FindComponentOfType(TRANSFORM));

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
	const TransformComponent* pLocalTransform = static_cast<TransformComponent*>(GetOwnerEntity()->FindComponentOfType(TRANSFORM));

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
			const TransformComponent* pTargetTransform = static_cast<TransformComponent*>(pTarget->FindComponentOfType(TRANSFORM));
			const BrainComponent* pTargetBrain = static_cast<BrainComponent*>(pTarget->FindComponentOfType(BRAIN));

			//find distance
			glm::vec3 v3TargetPos = pTargetTransform->GetEntityMatrixRow(POSITION_VECTOR);
			float fDistanceBetween = glm::length(v3TargetPos - v3LocalPos);

			//neighbourhood distance
			if (fDistanceBetween < fNEIGHBOURHOOD_RADIUS)
			{
				float fStrength = 1 - fDistanceBetween / fNEIGHBOURHOOD_RADIUS;
				v3AlignmentVelocity += pTargetBrain->GetCurrentVelocity() * fStrength;
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
	const TransformComponent* pLocalTransform = static_cast<TransformComponent*>(GetOwnerEntity()->FindComponentOfType(TRANSFORM));

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
			const TransformComponent* pTargetTransform = static_cast<TransformComponent*>(pTarget->FindComponentOfType(TRANSFORM));

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


Behaviour* BrainComponent::AddBehaviour(unsigned priority, Behaviour* pBehaviour)
{
	assert(pBehaviour != nullptr);
	Behaviour* pPrev = nullptr;

	auto previous = m_behaviours.find(priority);
	if (previous != m_behaviours.end())
	{
		pPrev = previous->second;
	}
	else
	{
		m_behaviours[priority] = pBehaviour;
	}
	return pPrev;
}

Behaviour* BrainComponent::GetBehaviour(unsigned priority) const
{
	Behaviour* pPrev = nullptr;
	auto previous = m_behaviours.find(priority);
	if (previous != m_behaviours.end())
	{
		pPrev = previous->second;
	}
	return pPrev;
}

Behaviour* BrainComponent::RemoveBehaviour(unsigned priority)
{
	Behaviour* pPrev = nullptr;
	auto previous = m_behaviours.find(priority);
	if (previous != m_behaviours.end())
	{
		pPrev = previous->second;
		m_behaviours.erase(priority);
	}
	return pPrev;
}


void BrainComponent::DeleteBehaviour(unsigned priority)
{
	auto previous = m_behaviours.find(priority);
	if (previous != m_behaviours.end())
	{
		delete previous->second;
		m_behaviours.erase(priority);
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

void BrainComponent::AddSeekBehaviour(unsigned uPriority, const glm::vec3* pv3Target)
{
	Entity* pEntity = GetOwnerEntity();
	if (!pEntity) return;

	Seek* pSeek = new Seek(static_cast<TransformComponent*>(pEntity->FindComponentOfType(TRANSFORM)), pv3Target, &m_v3CurrentVelocity);
	Behaviour* pPrev = AddBehaviour(uPriority, pSeek);
	delete pPrev;
}

void BrainComponent::AddWanderBehaviour(unsigned uPriority)
{
	Entity* pEntity = GetOwnerEntity();
	if (!pEntity) return;

	Wander* pWander = new Wander(static_cast<TransformComponent*>(pEntity->FindComponentOfType(TRANSFORM)), &m_v3CurrentVelocity);
	Behaviour* pPrev = AddBehaviour(uPriority, pWander);
	delete pPrev;
}
