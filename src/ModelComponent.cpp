#include "ModelComponent.h"

#include <learnopengl/shader.h>
#include <learnopengl/model.h>

#include <glm/glm.hpp>
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

	glm::mat4 m$ModelMatrix = pTransFormComponent->GetEntityMatrix();
	m$ModelMatrix = glm::scale(m$ModelMatrix, glm::vec3(m_fModelScale, m_fModelScale, m_fModelScale));
	a_pshader->setMat4("model", m$ModelMatrix);
	m_pModelData->Draw(*a_pshader);
}
