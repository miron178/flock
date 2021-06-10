#ifndef PURSUE_H
#define PURSUE_H

#include "Arrive.h"

class Pursue : public Arrive
{
public:
    Pursue(const Entity* a_pAgent, const glm::vec3* a_pTarget);
    virtual ~Pursue() = default;

private:
    virtual glm::vec3 Target() override;
};

#endif //PURSUE_H
