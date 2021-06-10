#ifndef ALIGNMENT_H
#define ALIGNMENT_H

#include "Behaviour.h"

class Alignment : public Behaviour
{
public:
    Alignment(const Entity* a_pAgent, const std::map<const unsigned int, Entity*>& xEntityMap);
    virtual ~Alignment() = default;
    virtual glm::vec3 Force() override;

private:
    const std::map<const unsigned int, Entity*>& m_xEntityMap;
};

#endif
