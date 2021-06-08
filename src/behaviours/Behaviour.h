#ifndef BEHAVIOUR_H
#define BEHAVIOUR_H
#include "TransformComponent.h"
#include "PhysicsComponent.h"
#include <glm/glm.hpp>
#include <map>

class Entity;

class Behaviour
{
public:
	Behaviour(const TransformComponent* a_pAgent, const glm::vec3* a_pTarget, const PhysicsComponent* pPhysicsComponent);
	virtual ~Behaviour() = default;

	virtual glm::vec3 Force() = 0;

	void SetSpeed(float a_fSpeed) { m_fSpeed = a_fSpeed; }
	void SetMaxSpeed(float a_fMaxSpeed) { m_fMaxSpeed = a_fMaxSpeed; }
	void SetNeighbourRadius(float a_fNeighbourRadius) { m_fNeighbourRadius = a_fNeighbourRadius; }

	void SetScaleFactor(float a_fScaleFactor) { m_fScaleFactor = a_fScaleFactor; }

protected:
	glm::vec3 AgentPos() const { return m_pAgent->GetEntityMatrixRow(POSITION_VECTOR); }
	glm::vec3 AgentForward() const { return m_pAgent->GetEntityMatrixRow(FORWARD_VECTOR); }
	glm::vec3 AgentVelocity() const { return m_pPhysicsComponent->GetVelocity(); }
	glm::vec3 TargetPos() const { return *m_pTarget; }

	glm::vec3 Clamp(const glm::vec3& v3, float a_maxMagnitude) const;
	glm::vec3 ClampVelocity(const glm::vec3& a_v3Velocity) const { return Clamp(a_v3Velocity, m_fMaxSpeed); }

	glm::vec3 SphericalRand(float a_fRadius);

	bool IsNeighbour(const Entity* a_pSelf, const Entity* a_pOther) const;

protected:
	const TransformComponent* m_pAgent;
	const glm::vec3* m_pTarget;
	const PhysicsComponent* m_pPhysicsComponent;

	float m_fSpeed = 1.0f;
	float m_fMaxSpeed = 1.0f;
	float m_fScaleFactor = 1.0f;
	float m_fNeighbourRadius = 1.0f;
};

#endif //BEHAVIOUR_H
