#ifndef BEHAVIOUR_H
#define BEHAVIOUR_H
#include "TransformComponent.h"
#include <glm/glm.hpp>

class Behaviour
{
public:
	Behaviour(const TransformComponent* pAgent, const glm::vec3* pTarget, const glm::vec3* pv3Velocity);
	virtual ~Behaviour() = default;

	virtual glm::vec3 Force() = 0;

	void SetSpeed(float fSpeed) { m_fSpeed = fSpeed; }
	void SetMaxSpeed(float fMaxSpeed) { m_fMaxSpeed = fMaxSpeed; }
	void SetMaxForce(float fMaxForce) { m_fMaxForce = fMaxForce; }

protected:
	glm::vec3 AgentPos() const { return m_pAgent->GetEntityMatrixRow(POSITION_VECTOR); }
	glm::vec3 AgentForward() const { return m_pAgent->GetEntityMatrixRow(FORWARD_VECTOR); }
	glm::vec3 TargetPos() const { return *m_pTarget; }

	glm::vec3 Clamp(const glm::vec3& v3, float maxMagnitude) const;
	glm::vec3 ClampVelocity(const glm::vec3& v3Velocity) const { return Clamp(v3Velocity, m_fMaxSpeed); }
	glm::vec3 ClampForce(const glm::vec3& v3Force) const { return Clamp(v3Force, m_fMaxForce); }

	glm::vec3 SphericalRand(float fRadius);

protected:
	const TransformComponent* m_pAgent;
	const glm::vec3* m_pTarget;
	const glm::vec3* m_pv3Velocity;

	float m_fSpeed = 1.0f;
	float m_fMaxSpeed = 1.0f;
	float m_fMaxForce = 0.1f;
};

#endif //BEHAVIOUR_H