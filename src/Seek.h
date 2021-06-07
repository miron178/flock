#ifndef SEEK_H
#define SEEK_H

#include "Arrive.h"

class Seek : public Arrive
{
public:
    Seek(const TransformComponent* pAgent, const glm::vec3* pTarget, const glm::vec3* pv3Velocity);
    virtual ~Seek() = default;
};

#endif //SEEK_H
