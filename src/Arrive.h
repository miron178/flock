#ifndef ARRIVE_H
#define ARRIVE_H

#include "Behaviour.h"

class Arrive : public Behaviour
{
public:
    Arrive(const TransformComponent* a_pAgent, const glm::vec3* a_pTarget, const glm::vec3* a_pv3Velocity, float a_fRadius = 1);
    virtual ~Arrive() = default;
    virtual glm::vec3 Force() override;

private:
    float m_fRadius;
};

#endif //ARRIVE_H
