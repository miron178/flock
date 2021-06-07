#ifndef SEPARATION_H
#define SEPARATION_H

#include "Behaviour.h"

class Separation : public Behaviour
{
public:
    Separation(const Entity* a_pSelf, const std::map<const unsigned int, Entity*>& xEntityMap);
    virtual ~Separation() = default;
    virtual glm::vec3 Force() override;

private:
    const Entity* m_pSelf;
    const std::map<const unsigned int, Entity*>& m_xEntityMap;
};

#endif //SEPARATION_H
