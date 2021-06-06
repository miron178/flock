#ifndef WANDER_H
#define WANDER_H

#include "Seek.h"

class Wander : public Seek
{
public:
    Wander(const TransformComponent* pAgent, const glm::vec3* pv3Velocity);
    virtual ~Wander() = default;
    virtual glm::vec3 Force() override;

private:
    bool Arrived() const;
    void NewTarget();
    void AddJitter();

private:
    glm::vec3 m_v3WanderPoint;

    float m_fDistance = 1.0f;
    float m_fRadius = 4.0f;
    float m_fJitter = 0.1f;
};

#endif //WANDER_H