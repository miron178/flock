#ifndef ENTITY_H
#define ENTITY_H

class Shader;

#include <vector>
#include <map>

#include "Component.h"
class Entity
{
public:
	Entity();
	~Entity()= default;

	virtual void Update(float a_fDeltaTime);
	virtual void Draw(Shader* a_pShader);

	void AddComponent(Component* a_pComponent);
	Component* FindComponentOfType(COMPONENT_TYPE a_eComponentType) const;

	static const std::map<const unsigned int, Entity*>& GetEntityMap() { return s_xEntityList; }
	unsigned int GetEntityID() const { return m_uEntityID; }

private:
	unsigned int m_uEntityID;
	std::vector<Component*> m_apComponentList;

	static unsigned int s_uEntityCount;
	static std::map<const unsigned int, Entity*> s_xEntityList;
};

#endif ENTITY_H
