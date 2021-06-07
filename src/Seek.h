#ifndef SEEK_H
#define SEEK_H

#include "Arrive.h"

class Seek : public Arrive
{
public:
    Seek(const TransformComponent* a_pAgent, const glm::vec3* a_pTarget, const glm::vec3* a_pv3Velocity);
    virtual ~Seek() = default;
};

#endif //SEEK_H
