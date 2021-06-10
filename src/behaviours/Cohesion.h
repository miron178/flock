#ifndef COHESION_H
#define COHESION_H

#include "Arrive.h"

class Cohesion : public Arrive
{
public:
    Cohesion(const Entity* a_pSelf, const std::map<const unsigned int, Entity*>& xEntityMap);
    virtual ~Cohesion() = default;

private:
    virtual glm::vec3 Target();

    const std::map<const unsigned int, Entity*>& m_xEntityMap;
};

#endif //COHESION_H
