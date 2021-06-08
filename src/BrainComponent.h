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

    Behaviour* GetBehaviour(unsigned a_priority) const;
    Behaviour* AddBehaviour(unsigned a_priority, Behaviour* a_pBehaviour);
    Behaviour* RemoveBehaviour(unsigned a_priority);
    void DeleteBehaviour(unsigned a_priority);
    void ClearBehaviours();

private:
    //Variable
    std::map<unsigned, Behaviour*> m_behaviours;
};

#endif
