#ifndef BRAIN_COMPONENT_H
#define BRAIN_COMPONENT_H

#include "Component.h"
#include "Behaviour.h"

#include <glm/glm.hpp>
#include <map>

class Entity;

class BrainComponent : public Component
{
public:
    BrainComponent(Entity* a_pEntity);
    virtual ~BrainComponent();

    virtual void Update(float a_fDeltaTime);
    virtual void Draw(Shader* a_pShader) {}

    glm::vec3 GetCurrentVelocity() const { return m_v3CurrentVelocity; }

    void AddSeekBehaviour(unsigned uPriority, const glm::vec3* pv3Target);
    void AddWanderBehaviour(unsigned uPriority);

    Behaviour* GetBehaviour(unsigned priority) const;

private:
    Behaviour* AddBehaviour(unsigned priority, Behaviour* pBehaviour);
    Behaviour* RemoveBehaviour(unsigned priority);
    void DeleteBehaviour(unsigned priority);
    void ClearBehaviours();

private:
    //flock
    glm::vec3 CalculateSeporationForce();
    glm::vec3 CalculateAlignmentForce();
    glm::vec3 CalculateCohesionForce();

    //Variable
    float m_fMass = 1;
    glm::vec3 m_v3CurrentVelocity;
    glm::vec3 m_v3WanderPoint;

    std::map<unsigned, Behaviour*> m_behaviours;
};

#endif
