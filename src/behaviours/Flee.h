#ifndef FLEE_H
#define FLEE_H

#include "Arrive.h"

class Flee : public Arrive
{
public:
    Flee(const TransformComponent* a_pAgent, const glm::vec3* a_pTarget, const PhysicsComponent* a_pPhysicsComponent);
    virtual ~Flee() = default;

private:
    virtual glm::vec3 Target() override;
};

#endif //FLEE_H
