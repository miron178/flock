#ifndef FLEE_H
#define FLEE_H

#include "Behaviour.h"

class Flee : public Behaviour
{
public:
    Flee(const TransformComponent* pAgent, const glm::vec3* pTarget, const glm::vec3* pv3Velocity);
    virtual ~Flee() = default;
    virtual glm::vec3 Force() override;
};

#endif //FLEE_H
