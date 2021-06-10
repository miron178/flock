#ifndef RAYCAST_H
#define RAYCAST_H

#include <glm/glm.hpp>

class ModelComponent;

class Raycast
{
public:
	static bool Hit(glm::vec3 const& a_v3Orig, glm::vec3 const& a_v3Dir, float a_fMaxDistance,
		glm::vec3 const& a_v3Vert0, glm::vec3 const& a_v3Vert1, glm::vec3 const& a_v3Vert2,
		glm::vec3& a_v3Position, float& a_fDistance);

	static bool Hit(glm::vec3 const& a_v3Orig, glm::vec3 const& a_v3Dir, float a_fMaxDistance,
		ModelComponent const* a_pModel,
		glm::vec3& a_v3Position, glm::vec3& a_v3Normal, float& a_fDistance);
};

#endif