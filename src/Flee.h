#ifndef FLEE_H
#define FLEE_H

#include "Behaviour.h"

class Flee : public Behaviour
{
public:
    Flee(const TransformComponent* a_pAgent, const glm::vec3* a_pTarget, const PhysicsComponent* a_pPhysicsComponent);
    virtual ~Flee() = default;
    virtual glm::vec3 Force() override;
};

#endif //FLEE_H
