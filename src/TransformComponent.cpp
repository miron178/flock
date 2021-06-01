#include "TransformComponent.h"

typedef Component PARANT;

TransformComponent::TransformComponent(Entity* a_pOwner)
	: PARANT(a_pOwner)
	, m_m4EntityMatrix(glm::mat4(1.0f))
{
	m_eComponentType = TRANSFORM;
}

TransformComponent::~TransformComponent()
{

}

void TransformComponent::SetEntityMatrixRow(MATRIX_ROW a_eRow, glm::vec3 a_v3Vec)
{
	m_m4EntityMatrix[a_eRow] = glm::vec4(a_v3Vec, (a_eRow == POSITION_VECTOR ? 1.f : 0.0f));
}

glm::vec3 TransformComponent::GetEntityMatrixRow(MATRIX_ROW a_eRow) const
{
	return m_m4EntityMatrix[a_eRow];
}
