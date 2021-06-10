#ifndef SEEK_H
#define SEEK_H

#include "Arrive.h"

class Seek : public Arrive
{
public:
    Seek(const Entity* a_pAgent, const glm::vec3* a_pTarget);
    virtual ~Seek() = default;
};

#endif //SEEK_H
