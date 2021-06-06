#ifndef SEEK_H
#define SEEK_H

#include "Behaviour.h"

class Seek : public Behaviour
{
public:
    Seek(const TransformComponent* pAgent, const glm::vec3* pTarget, const glm::vec3* pv3Velocity);
    virtual ~Seek() = default;
    virtual glm::vec3 Force() override;
};

#endif //SEEK_H