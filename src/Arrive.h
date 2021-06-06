#ifndef ARRIVE_H
#define ARRIVE_H

#include "Behaviour.h"

class Arrive : public Behaviour
{
public:
    Arrive(const TransformComponent* pAgent, const glm::vec3* pTarget, const glm::vec3* pv3Velocity, float fRadius = 1);
    virtual ~Arrive() = default;
    virtual glm::vec3 Force() override;

private:
    float m_fRadius;
};

#endif //ARRIVE_H
