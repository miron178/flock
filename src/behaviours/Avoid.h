#ifndef AVOID_H
#define AVOID_H

#include "Behaviour.h"
#include <vector>

class Avoid : public Behaviour
{
public:
    Avoid(const Entity* a_pAgent, const std::vector<const Entity*>& a_vAvoid);
    virtual ~Avoid() = default;

    virtual glm::vec3 Force() override;

	//public params
    float m_fRayLength = 1.0f;

private:
    const std::vector<const Entity*>& m_vAvoid;
};

#endif 
