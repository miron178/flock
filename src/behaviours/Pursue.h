#ifndef PURSUE_H
#define PURSUE_H

#include "Arrive.h"

class Pursue : public Arrive
{
public:
    Pursue(const TransformComponent* a_pAgent, const glm::vec3* a_pTarget, const PhysicsComponent* a_pPhysicsComponent);
    virtual ~Pursue() = default;

private:
    virtual glm::vec3 Target() override;
};

#endif //PURSUE_H
