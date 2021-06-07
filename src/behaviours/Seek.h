#ifndef SEEK_H
#define SEEK_H

#include "Arrive.h"

class Seek : public Arrive
{
public:
    Seek(const TransformComponent* a_pAgent, const glm::vec3* a_pTarget, const PhysicsComponent* a_pPhysicsComponent);
    virtual ~Seek() = default;
};

#endif //SEEK_H
