#ifndef BRAIN_COMPONENT_H
#define BRAIN_COMPONENT_H

#include "Component.h"
#include "Seek.h"
#include "Flee.h"
#include "Wander.h"
#include "Arrive.h"

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
    //flock
    glm::vec3 CalculateSeporationForce();
    glm::vec3 CalculateAlignmentForce();
    glm::vec3 CalculateCohesionForce();
#endif

    //Variable
    float m_fMass = 1;
    glm::vec3 m_v3CurrentVelocity;
    glm::vec3 m_v3WanderPoint;

    glm::vec3 m_v3Target;

    Arrive m_arrive;
    Flee m_flee;
    Seek m_seek;
    Wander m_wander;
};

#endif
