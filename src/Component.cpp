#include "Component.h"

#include "Entity.h"

Component::Component(Entity* a_pOwner, COMPONENT_TYPE a_eComponentType)
	: m_pOwnerEntity(a_pOwner)
	, m_eComponentType(a_eComponentType)
{}
