#ifndef PHYSICS_COMPONENT_H
#define PHYSICS_COMPONENT_H

#include "Component.h"
#include <glm/ext.hpp>

class TransformComponent;

class PhysicsComponent : public Component
{
public:
	PhysicsComponent(Entity* a_pOwner);
	virtual ~PhysicsComponent() = default;

	virtual void Update(float a_fDeltaTime) override;
	virtual void Draw(Shader* a_pShader) override {};


	glm::vec3 GetVelocity() const { return m_v3Velocity; }
	void SetVelocity(glm::vec3 a_v3Velocity) { m_v3Velocity = a_v3Velocity; }

	glm::vec3 GetForce() const { return m_v3Force; }
	void SetForce(glm::vec3 a_v3Force) { m_v3Force = a_v3Force; }
	void AddForce(glm::vec3 a_v3Force) { m_v3Force += a_v3Force; }

	float m_fMass = 1;
	float m_fMaxForce = 10;
	float m_fMaxVelocity = 10;

private:
	TransformComponent* m_pTransformComponent;

	glm::vec3 m_v3Velocity;
	glm::vec3 m_v3Force;

};

#endif