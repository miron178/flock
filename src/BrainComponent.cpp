#include "BrainComponent.h"

#include "Entity.h"
#include "TransformComponent.h"

const float fSPEED = 1.0f;
const float fNEIGHBOURHOOD_RADIUS = 2.0f;

const float fJITTER = 0.5f;
const float fWANDER_RADIUS = 4.0f;
const float fCIRCLE_FORWARD_MULTIPLIER = 1.0f;

BrainComponent::BrainComponent(Entity* a_pOwner)
	: Component(a_pOwner)
	, m_v3CurrentVelocity(0.0f)
	, m_v3WanderPoint(0.0f)
	, m_v3Target(4.0f, 0.0f, 4.0f)
	, m_seek(static_cast<TransformComponent*>(a_pOwner->FindComponentOfType(TRANSFORM)), &m_v3Target, &m_v3CurrentVelocity)
	, m_flee(static_cast<TransformComponent*>(a_pOwner->FindComponentOfType(TRANSFORM)), &m_v3Target, &m_v3CurrentVelocity)
	, m_wander(static_cast<TransformComponent*>(a_pOwner->FindComponentOfType(TRANSFORM)), &m_v3CurrentVelocity)
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
	glm::vec3 v3FinalForce(0.0f);

	//glm::vec3 v3SeporationForce = CalculateSeporationForce();
	//glm::vec3 v3AlignmentForce = CalculateAlignmentForce();
	//glm::vec3 v3CohisionForce = CalculateCohesionForce();

	//seek
	//glm::vec3 v3SeekForce = CalculateSeekForce(glm::vec3(4.0f, 0.0f, 4.0f), v3CurrentPos);
	glm::vec3 v3SeekForce = m_seek.Force();
	
	//flee
	//glm::vec3 v3FleeForce = CalculateFleeForce(glm::vec3(0.0f, 0.0f, 0.0f), v3CurrentPos);
	glm::vec3 v3FleeForce = m_flee.Force();

	//wander
	//glm::vec3 v3WanderForce = CalculateWanderForce(v3Forward, v3CurrentPos);
	glm::vec3 v3WanderForce = m_wander.Force();

	//v3FinalForce = v3SeporationForce * 3 + v3CohisionForce * 1 + v3AlignmentForce;
	v3FinalForce = v3SeekForce;

	//velocity
	m_v3CurrentVelocity += v3FinalForce;
	m_v3CurrentVelocity = glm::clamp(m_v3CurrentVelocity, glm::vec3(-10.0f, 0.0f, -10.0f), glm::vec3(10.0f, 0.0f, 10.0f));
	v3CurrentPos += m_v3CurrentVelocity * a_fDeltaTime;

	v3Forward = m_v3CurrentVelocity;
	if (glm::length(v3Forward) > 0.0f)
	{
		v3Forward = glm::normalize(v3Forward);

		glm::vec3 v3Up = pTransComp->GetEntityMatrixRow(UP_VECTOR);
		glm::vec3 v3Right = glm::cross(v3Up, v3Forward);

		//matrix
		pTransComp->SetEntityMatrixRow(RIGHT_VECTOR, v3Right);
		pTransComp->SetEntityMatrixRow(FORWARD_VECTOR, v3Forward);
	}
	pTransComp->SetEntityMatrixRow(POSITION_VECTOR, v3CurrentPos);
}

#if 1
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

glm::vec3 BrainComponent::CalculateSeporationForce()
{
	//Seporation Vector
	glm::vec3 v3SeporationVelocity(0.0f);
	unsigned int uNeighbourCount = 0;

	//this transform
	const TransformComponent* pLocalTransform = static_cast<TransformComponent*>(GetOwnerEntity()->FindComponentOfType(TRANSFORM));

	//this pos
	glm::vec3 v3LocalPos = pLocalTransform->GetEntityMatrixRow(POSITION_VECTOR);

	//itorator
	const std::map<const unsigned int, Entity*>& xEntityMap = Entity::GetEntityMap();
	std::map<const unsigned int, Entity*>::const_iterator xConstIter;

	//loop over entities
	for (xConstIter = xEntityMap.begin(); xConstIter != xEntityMap.end(); ++xConstIter)
	{
		const Entity* pTarget = xConstIter->second;
		if (pTarget->GetEntityID() != GetOwnerEntity()->GetEntityID())
		{
			const TransformComponent* pTargetTransform = static_cast<TransformComponent*>(pTarget->FindComponentOfType(TRANSFORM));

			//find distance
			glm::vec3 v3TargetPos = pTargetTransform->GetEntityMatrixRow(POSITION_VECTOR);
			float fDistanceBetween = glm::length(v3TargetPos - v3LocalPos);

			//neighbourhood distance
			if(fDistanceBetween < fNEIGHBOURHOOD_RADIUS)
			{
				float fStrength = 1 - fDistanceBetween / fNEIGHBOURHOOD_RADIUS;
				v3SeporationVelocity += glm::normalize(v3LocalPos - v3TargetPos) * fStrength;
				uNeighbourCount++;
			}
		}
	}

	if (uNeighbourCount)
	{
		v3SeporationVelocity /= uNeighbourCount;
	}

	return v3SeporationVelocity;
}

glm::vec3 BrainComponent::CalculateAlignmentForce()
{
	//alignment velocity
	glm::vec3 v3AlignmentVelocity(0.0f);
	unsigned int uNeighbourCount = 0;

	//this transform
	const TransformComponent* pLocalTransform = static_cast<TransformComponent*>(GetOwnerEntity()->FindComponentOfType(TRANSFORM));

	//this pos
	glm::vec3 v3LocalPos = pLocalTransform->GetEntityMatrixRow(POSITION_VECTOR);

	//itorator
	const std::map<const unsigned int, Entity*>& xEntityMap = Entity::GetEntityMap();
	std::map<const unsigned int, Entity*>::const_iterator xConstIter;

	//loop over entities
	for (xConstIter = xEntityMap.begin(); xConstIter != xEntityMap.end(); ++xConstIter)
	{
		const Entity* pTarget = xConstIter->second;
		if (pTarget->GetEntityID() != GetOwnerEntity()->GetEntityID())
		{
			const TransformComponent* pTargetTransform = static_cast<TransformComponent*>(pTarget->FindComponentOfType(TRANSFORM));
			const BrainComponent* pTargetBrain = static_cast<BrainComponent*>(pTarget->FindComponentOfType(BRAIN));

			//find distance
			glm::vec3 v3TargetPos = pTargetTransform->GetEntityMatrixRow(POSITION_VECTOR);
			float fDistanceBetween = glm::length(v3TargetPos - v3LocalPos);

			//neighbourhood distance
			if (fDistanceBetween < fNEIGHBOURHOOD_RADIUS)
			{
				float fStrength = 1 - fDistanceBetween / fNEIGHBOURHOOD_RADIUS;
				v3AlignmentVelocity += pTargetBrain->GetCurrentVelocity() * fStrength;
				uNeighbourCount++;
			}
		}
	}

	if (uNeighbourCount)
	{
		v3AlignmentVelocity /= uNeighbourCount;
	}

	return v3AlignmentVelocity;
}

glm::vec3 BrainComponent::CalculateCohesionForce()
{
	//alignment velocity
	glm::vec3 v3CohesionVelocity(0.0f);
	unsigned int uNeighbourCount = 0;

	//this transform
	const TransformComponent* pLocalTransform = static_cast<TransformComponent*>(GetOwnerEntity()->FindComponentOfType(TRANSFORM));

	//this pos
	glm::vec3 v3LocalPos = pLocalTransform->GetEntityMatrixRow(POSITION_VECTOR);

	//itorator
	const std::map<const unsigned int, Entity*>& xEntityMap = Entity::GetEntityMap();
	std::map<const unsigned int, Entity*>::const_iterator xConstIter;

	//loop over entities
	for (xConstIter = xEntityMap.begin(); xConstIter != xEntityMap.end(); ++xConstIter)
	{
		const Entity* pTarget = xConstIter->second;
		if (pTarget->GetEntityID() != GetOwnerEntity()->GetEntityID())
		{
			const TransformComponent* pTargetTransform = static_cast<TransformComponent*>(pTarget->FindComponentOfType(TRANSFORM));

			//find distance
			glm::vec3 v3TargetPos = pTargetTransform->GetEntityMatrixRow(POSITION_VECTOR);
			float fDistanceBetween = glm::length(v3TargetPos - v3LocalPos);

			//neighbourhood distance
			if (fDistanceBetween < fNEIGHBOURHOOD_RADIUS)
			{
				float fStrength = 1 - fDistanceBetween / fNEIGHBOURHOOD_RADIUS;
				v3CohesionVelocity += v3TargetPos * fStrength;
				uNeighbourCount++;
			}
		}
	}

	if (glm::length(v3CohesionVelocity) > 0.0f && uNeighbourCount)
	{
		v3CohesionVelocity /= uNeighbourCount;
		v3CohesionVelocity = glm::normalize(v3CohesionVelocity- v3LocalPos);
	}

	return v3CohesionVelocity;
}
#endif
