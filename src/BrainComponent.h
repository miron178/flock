#ifndef BRAIN_COMPONENT_H
#define BRAIN_COMPONENT_H

#include "Component.h"

#include <glm/glm.hpp>

class Entity;

class BrainComponent : public Component
{
public:
    BrainComponent(Entity* a_pEntity);

    virtual void Update(float a_fDeltaTime);
    virtual void Draw(Shader* a_pShader) {}

private:
    glm::vec3 CalculateSeekForce  (const glm::vec3& v3Target,  const glm::vec3& v3CurrentPos) const;
    glm::vec3 CalculateFleeForce  (const glm::vec3& v3Target,  const glm::vec3& v3CurrentPos) const;
    glm::vec3 CalculateWanderForce(const glm::vec3& v3Forward, const glm::vec3& v3CurrentPos);

    //Variable
    glm::vec3 m_v3CurrentVelocity;
    glm::vec3 m_v3WanderPoint;
};

#endif
