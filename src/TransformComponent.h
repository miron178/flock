#ifndef TRANSFORM_COMPONENT_H
#define TRANSFORM_COMPONENT_H

#include "Component.h"
#include <glm/ext.hpp>

enum MATRIX_ROW
{
	RIGHT_VECTOR,
	UP_VECTOR,
	FORWARD_VECTOR,
	POSITION_VECTOR,
};

class TransformComponent : public Component
{
public:
	TransformComponent(Entity* pOwner);
	~TransformComponent();

	virtual void Update(float a_fDeltaTime) override {};
	virtual void Draw(Shader* a_pShader) override {};

	const glm::mat4& GetEntityMatrix() { return m_m4EntityMatrix; }

	void SetEntityMatrixRow(MATRIX_ROW a_eRow, glm::vec3 a_v3Vec);
	glm::vec3 GetEntityMatrixRow(MATRIX_ROW a_eRow) const;

private:
	glm::mat4 m_m4EntityMatrix;
};

#endif