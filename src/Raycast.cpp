#include "Raycast.h"
#include "Entity.h"
#include "ModelComponent.h"
#include "TransformComponent.h"

#include <glm/ext.hpp>
#include <learnopengl/model.h>
#include <learnopengl/mesh.h>

bool Raycast::Hit(glm::vec3 const& a_v3Orig, glm::vec3 const& a_v3Dir, float a_fMaxDistance,
	glm::vec3 const& a_v3Vert0, glm::vec3 const& a_v3Vert1, glm::vec3 const& a_v3Vert2,
	glm::vec3& a_v3Position, float& a_fDistance)
{
	glm::vec2 v2BaryPosition;
	bool bHit = glm::intersectRayTriangle(a_v3Orig, a_v3Dir, a_v3Vert0, a_v3Vert1, a_v3Vert2, v2BaryPosition, a_fDistance);


	if (bHit && a_fDistance >= 0 && a_fDistance < a_fMaxDistance)
	{
		a_v3Position = (1.0f - v2BaryPosition.x - v2BaryPosition.y) * a_v3Vert0 +
			v2BaryPosition.x * a_v3Vert1 +
			v2BaryPosition.y * a_v3Vert2;
	}
	else
	{
		bHit = false;
	}

	return bHit;
}

bool Raycast::Hit(glm::vec3 const& a_v3Orig, glm::vec3 const& a_v3Dir, float a_fMaxDistance,
	ModelComponent const* a_pModelComponent,
	glm::vec3& a_v3Position, glm::vec3& a_v3Normal, float& a_fDistance)
{
	bool bHit = false;
	glm::vec3 v3NearestHitPosition;
	float fNearestHitDistance = a_fMaxDistance;
	const Mesh* pNearestHitMesh = nullptr;
	size_t uNearestHitIndex = 0;

	glm::mat4 m4ModelMatrix = a_pModelComponent->GetModelMatrix();
	Model* pModel = a_pModelComponent->GetModel();

	for (const auto& mesh : pModel->meshes)
	{
		size_t size = mesh.indices.size();
		assert(size % 3 == 0);
		//size_t count = size / 3;

		for (size_t i = 0; i < size; i += 3)
		{
			const Vertex& v0 = mesh.vertices[mesh.indices[i + 0]];
			const Vertex& v1 = mesh.vertices[mesh.indices[i + 1]];
			const Vertex& v2 = mesh.vertices[mesh.indices[i + 2]];

			glm::vec3 v3WorldV0 = m4ModelMatrix * glm::vec4(v0.Position, 1.0f);
			glm::vec3 v3WorldV1 = m4ModelMatrix * glm::vec4(v1.Position, 1.0f);
			glm::vec3 v3WorldV2 = m4ModelMatrix * glm::vec4(v2.Position, 1.0f);

			bHit = Hit(a_v3Orig, a_v3Dir, a_fMaxDistance,
				v3WorldV0, v3WorldV1, v3WorldV2,
				a_v3Position, a_fDistance);

			if (bHit && a_fDistance <= fNearestHitDistance)
			{
				v3NearestHitPosition = a_v3Position;
				fNearestHitDistance = a_fDistance;
				pNearestHitMesh = &mesh;
				uNearestHitIndex = i;
			}
		}
	}

	if (pNearestHitMesh)
	{
		const Vertex& v0 = pNearestHitMesh->vertices[pNearestHitMesh->indices[uNearestHitIndex + 0]];
		const Vertex& v1 = pNearestHitMesh->vertices[pNearestHitMesh->indices[uNearestHitIndex + 1]];
		const Vertex& v2 = pNearestHitMesh->vertices[pNearestHitMesh->indices[uNearestHitIndex + 2]];

		a_v3Position = v3NearestHitPosition;
		a_v3Normal = (v0.Normal + v1.Normal + v2.Normal) / 3.0f;
		a_fDistance = fNearestHitDistance;
		bHit = true;
	}

	return bHit;
}
