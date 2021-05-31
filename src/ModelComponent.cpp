#include "ModelComponent.h"

#include <learnopengl/shader.h>
#include <learnopengl/model.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "TransformComponent.h"
#include "Entity.h"



typedef Component PARENT;

ModelComponent::ModelComponent(Entity* a_OwnerEntity)
	: PARENT(a_OwnerEntity)
	, m_pModelData (nullptr)
	, m_fModelScale(0.0f)
{

}

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
	TransformComponent* pTransFormComponent = static_cast<TransformComponent*>( m_pOwnerEntity->FindComponentOfType(TRANSFORM));
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
