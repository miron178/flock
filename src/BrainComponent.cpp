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

	PhysicsComponent* pPhysicsComponent = pEntity->FindPhysicsComponent();
	if (!pPhysicsComponent) return;

	//calculate forces
	float fTotalMagnitude = 0;
	for (auto item : m_behaviours)
	{
		glm::vec3 v3Force = item.second->Force();
		float fMagnitude = glm::length(v3Force);

		if (fTotalMagnitude + fMagnitude < m_fMaxForce)
		{
			pPhysicsComponent->AddForce(v3Force);
			fTotalMagnitude += fMagnitude;
		}
		else
		{
			break;
		}
	}
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
