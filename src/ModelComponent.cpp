#include "ModelComponent.h"

#include <learnopengl/shader.h>
#include <learnopengl/model.h>

#include <glm/gtc/matrix_transform.hpp>

#include "TransformComponent.h"
#include "Entity.h"

ModelComponent::ModelComponent(Entity* a_OwnerEntity)
	: Component(a_OwnerEntity, COMPONENT_TYPE::MODEL)
	, m_pModelData (nullptr)
	, m_fModelScale(0.0f)
{}

ModelComponent::~ModelComponent()
{

}

glm::mat4 ModelComponent::GetModelMatrix() const
{
	TransformComponent* pTransFormComponent = m_pOwnerEntity->FindTransformComponent();
	glm::mat4 m4ModelMatrix = pTransFormComponent->GetEntityMatrix();
	m4ModelMatrix = glm::scale(m4ModelMatrix, glm::vec3(m_fModelScale, m_fModelScale, m_fModelScale));
	return m4ModelMatrix;
}

void ModelComponent::Draw(Shader* a_pshader)
{
	if (!a_pshader)
	{
		return;
	}

	if (!m_pModelData)
	{
		return;
	}

	//get transform componant
	TransformComponent* pTransFormComponent = m_pOwnerEntity->FindTransformComponent();
	if (!pTransFormComponent)
	{
		//ERROR TODO (maybe)
		return;
	}

	a_pshader->setMat4("model", GetModelMatrix());
	m_pModelData->Draw(*a_pshader);
}
