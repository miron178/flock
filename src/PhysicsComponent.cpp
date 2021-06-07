#include "PhysicsComponent.h"
#include "Entity.h"
#include "TransformComponent.h"

PhysicsComponent::PhysicsComponent(Entity* a_pOwner)
	: Component(a_pOwner, COMPONENT_TYPE::PHYSICS)
	, m_pTransformComponent(reinterpret_cast<TransformComponent*>(a_pOwner->FindComponentOfType(COMPONENT_TYPE::TRANSFORM)))
	, m_v3Force(0,0,0)
	, m_v3Velocity(0,0,0)
{
}

void PhysicsComponent::Update(float a_fDeltaTime)
{
	glm::vec3 v3Position = m_pTransformComponent->GetEntityMatrixRow(POSITION_VECTOR);
	glm::vec3 v3Forward  = m_pTransformComponent->GetEntityMatrixRow(FORWARD_VECTOR);

	m_v3Velocity += m_v3Force / m_fMass;
	//m_v3CurrentVelocity = glm::clamp(m_v3CurrentVelocity, glm::vec3(-10.0f, 0.0f, -10.0f), glm::vec3(10.0f, 0.0f, 10.0f));
	v3Position += m_v3Velocity * a_fDeltaTime;
	v3Forward = m_v3Velocity;
	if (glm::length(v3Forward) > 0.0f)
	{
		v3Forward = glm::normalize(v3Forward);

		glm::vec3 v3Up = m_pTransformComponent->GetEntityMatrixRow(UP_VECTOR);
		glm::vec3 v3Right = glm::cross(v3Up, v3Forward);

		//matrix
		m_pTransformComponent->SetEntityMatrixRow(RIGHT_VECTOR, v3Right);
		m_pTransformComponent->SetEntityMatrixRow(FORWARD_VECTOR, v3Forward);
	}
	m_pTransformComponent->SetEntityMatrixRow(POSITION_VECTOR, v3Position);

	m_v3Force = glm::vec3(0, 0, 0);
}
