#include "Avoid.h"
#include "Entity.h"
#include "Raycast.h"

Avoid::Avoid(const Entity* a_pAgent, const std::vector<const Entity*>& a_vAvoid)
	: Behaviour(a_pAgent, nullptr)
	, m_vAvoid(a_vAvoid)
{}

glm::vec3 Avoid::Force()
{
	glm::vec3 v3Force(0, 0, 0);
	glm::vec3 v3Position;
	glm::vec3 v3Normal;
	float fDistance;
	
	for (const auto pAvoid : m_vAvoid)
	{
		bool bHit = Raycast::Hit(AgentPos(), AgentForward(), m_fRayLength,
			pAvoid->FindModelComponent(), v3Position, v3Normal, fDistance);

		if (bHit)
		{
			//float fMultiplier = (m_fRayLength - fDistance) / m_fRayLength;
			v3Force = v3Normal; // *fMultiplier;
			break;
		}
	}
	return v3Force * m_fScaleFactor;
}
