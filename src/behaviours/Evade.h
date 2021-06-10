#ifndef EVADE_H
#define EVADE_H

#include "Arrive.h"

class Evade : public Arrive
{
public:
    Evade(const Entity* a_pAgent, const glm::vec3* a_pTarget);
    virtual ~Evade() = default;

private:
    virtual glm::vec3 Target() override;
};

#endif 
