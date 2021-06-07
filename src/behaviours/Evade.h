#ifndef EVADE_H
#define EVADE_H

#include "Arrive.h"

class Evade : public Arrive
{
public:
    Evade(const TransformComponent* a_pAgent, const glm::vec3* a_pTarget, const PhysicsComponent* a_pPhysicsComponent);
    virtual ~Evade() = default;

private:
    virtual glm::vec3 Target() override;
};

#endif 
