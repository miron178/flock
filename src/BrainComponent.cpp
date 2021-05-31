#include "BrainComponent.h"

#include "Entity.h"
#include "TransformComponent.h"

static const float fSPEED = 1.0f;

static const float fJITTER = 0.5f;
static const float fWANDER_RADIUS = 4.0f;
static const float fCIRCLE_FORWARD_MULTIPLIER = 1.0f;

BrainComponent::BrainComponent(Entity* a_pOwner)
	: Component(a_pOwner)
	, m_v3CurrentVelocity(0.0f)
	, m_v3WanderPoint(0.0f)
{
	m_eComponentType = BRAIN;
}

void BrainComponent::Update(float a_fDeltaTime)
{
	//get owner
	Entity* pEntity = GetOwnerEntity();
	if (!pEntity) return;

	//get transform
	TransformComponent* pTransComp = static_cast<TransformComponent*>(pEntity->FindComponentOfType(TRANSFORM));
	if (!pTransComp) return;

	//get vectors
	glm::vec3 v3Forward    = pTransComp->GetEntityMatrixRow(FORWARD_VECTOR);
	glm::vec3 v3CurrentPos = pTransComp->GetEntityMatrixRow(POSITION_VECTOR);

	//calculate forces
	glm::vec3 v3NewForce(0.0f);

	//seek
	//v3NewForce = CalculateSeekForce(glm::vec3(4.0f, 0.0f, 4.0f), v3CurrentPos);
	
	//flee
	//v3NewForce = CalculateFleeForce(glm::vec3(0.0f, 0.0f, 0.0f), v3CurrentPos);

	//wander
	v3NewForce = CalculateWanderForce(v3Forward, v3CurrentPos);

	//velocity
	m_v3CurrentVelocity += v3NewForce;
	m_v3CurrentVelocity = glm::clamp(m_v3CurrentVelocity, glm::vec3(-10.0f, 0.0f, -10.0f), glm::vec3(10.0f, 0.0f, 10.0f));
	v3CurrentPos += m_v3CurrentVelocity * a_fDeltaTime;

	v3Forward = m_v3CurrentVelocity;
	if (glm::length(v3Forward) > 0.0f)
	{
		v3Forward = glm::normalize(v3Forward);
	}

	glm::vec3 v3Up = pTransComp->GetEntityMatrixRow(UP_VECTOR);
	glm::vec3 v3Right = glm::cross(v3Up, v3Forward);

	//matrix
	pTransComp->SetEntityMatrixRow(RIGHT_VECTOR, v3Right);
	pTransComp->SetEntityMatrixRow(FORWARD_VECTOR, v3Forward);
	pTransComp->SetEntityMatrixRow(POSITION_VECTOR, v3CurrentPos);
}

glm::vec3 BrainComponent::CalculateSeekForce(const glm::vec3& v3Target, const glm::vec3& v3CurrentPos) const
{
	//target dir
	glm::vec3 v3TargetDir(v3Target - v3CurrentPos);
	if (glm::length(v3TargetDir) > 0.0f)
	{
		v3TargetDir = glm::normalize(v3TargetDir);
	}

	//new velocity
	glm::vec3 v3NewVelocity = v3TargetDir * fSPEED;

	return (v3NewVelocity - m_v3CurrentVelocity);
}

glm::vec3 BrainComponent::CalculateFleeForce(const glm::vec3& v3Target, const glm::vec3& v3CurrentPos) const
{
	//target dir
	glm::vec3 v3TargetDir(v3CurrentPos - v3Target);
	if (glm::length(v3TargetDir) > 0.0f)
	{
		v3TargetDir = glm::normalize(v3TargetDir);
	}

	//new velocity
	glm::vec3 v3NewVelocity = v3TargetDir * fSPEED;

	return (v3NewVelocity - m_v3CurrentVelocity);
}

glm::vec3 BrainComponent::CalculateWanderForce(const glm::vec3& v3Forward, const glm::vec3& v3CurrentPos)
{
	//project point for sphere
	glm::vec3 v3SphereOrigin = v3CurrentPos + (v3Forward * fCIRCLE_FORWARD_MULTIPLIER);

	//point on sphere
	if (glm::length(m_v3WanderPoint) == 0.0f)
	{
		glm::vec3 v3RandomPointOnSphere = glm::sphericalRand(fWANDER_RADIUS);
		m_v3WanderPoint = v3SphereOrigin + v3RandomPointOnSphere;
	}

	//calculate dir
	glm::vec3 v3DirectionToTarget = glm::normalize(m_v3WanderPoint - v3SphereOrigin) * fWANDER_RADIUS;

	//final point
	m_v3WanderPoint = v3SphereOrigin + v3DirectionToTarget;

	m_v3WanderPoint += glm::sphericalRand(fJITTER);

	return CalculateSeekForce(m_v3WanderPoint, v3CurrentPos);
}