#ifndef BEHAVIOUR_H
#define BEHAVIOUR_H
#include "TransformComponent.h"
#include "PhysicsComponent.h"
#include "ModelComponent.h"
#include <glm/glm.hpp>
#include <map>

class Entity;

class Behaviour
{
public:
	Behaviour(const Entity* a_pAgent, const glm::vec3* a_pTarget);
	virtual ~Behaviour() = default;

	virtual glm::vec3 Force() = 0;

	//public params
	float m_fSpeed = 1.0f;
	float m_fScaleFactor = 1.0f;
	float m_fNeighbourRadius = 1.0f;

protected:
	glm::vec3 AgentPos() const;
	glm::vec3 AgentForward() const;
	glm::vec3 AgentVelocity() const { return m_pPhysicsComponent->GetVelocity(); }
	glm::vec3 TargetPos() const { return *m_pTarget; }

	glm::vec3 Clamp(const glm::vec3& v3, float a_maxMagnitude) const;

	glm::vec3 SphericalRand(float a_fRadius);

	bool IsNeighbour(const Entity* a_pSelf, const Entity* a_pOther) const;

protected:
	const Entity* m_pAgent;
	const TransformComponent* m_pTransformComponent;
	const PhysicsComponent* m_pPhysicsComponent;
	const ModelComponent* m_pModelComponent;

	const glm::vec3* m_pTarget;
};

#endif //BEHAVIOUR_H
