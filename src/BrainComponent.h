#ifndef BRAIN_COMPONENT_H
#define BRAIN_COMPONENT_H

#include "Component.h"
#include "Seek.h"
#include "Flee.h"
#include "Wander.h"

#include <glm/glm.hpp>

class Entity;

class BrainComponent : public Component
{
public:
    BrainComponent(Entity* a_pEntity);

    virtual void Update(float a_fDeltaTime);
    virtual void Draw(Shader* a_pShader) {}

    glm::vec3 GetCurrentVelocity() const { return m_v3CurrentVelocity; }

private:
#if 1
    //steer
    glm::vec3 CalculateSeekForce  (const glm::vec3& v3Target,  const glm::vec3& v3CurrentPos) const;
    glm::vec3 CalculateFleeForce  (const glm::vec3& v3Target,  const glm::vec3& v3CurrentPos) const;
    glm::vec3 CalculateWanderForce(const glm::vec3& v3Forward, const glm::vec3& v3CurrentPos);

    //flock
    glm::vec3 CalculateSeporationForce();
    glm::vec3 CalculateAlignmentForce();
    glm::vec3 CalculateCohesionForce();
#endif

    //Variable
    glm::vec3 m_v3CurrentVelocity;
    glm::vec3 m_v3WanderPoint;

    glm::vec3 m_v3Target;
    Seek m_seek;
    Flee m_flee;
    Wander m_wander;
};

#endif
