#ifndef WANDER_H
#define WANDER_H

#include "Seek.h"

class Wander : public Seek
{
public:
    Wander(const TransformComponent* a_pAgent, const glm::vec3* a_pv3Velocity);
    virtual ~Wander() = default;
    virtual glm::vec3 Force() override;

private:
    glm::vec3 m_v3Direction;
    glm::vec3 m_v3Target;

    float m_fDistance = 1.0f;
    float m_fRadius = 0.8f;
    float m_fJitter = 0.4f;
};

#endif //WANDER_H
