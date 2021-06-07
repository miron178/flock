#ifndef COMPONENT_H
#define COMPONENT_H

class Entity;
class Shader;


enum class COMPONENT_TYPE
{
	NONE,
	TRANSFORM,
	MODEL,
	BRAIN,
};

class Component
{
public:
	Component(Entity* a_pOwner, COMPONENT_TYPE a_eComponentType);
	~Component() = default;

	virtual void Update(float a_fDeltaTime) = 0;
	virtual void Draw(Shader* a_pShader) = 0;

	inline Entity* GetOwnerEntity() { return m_pOwnerEntity; }
	inline COMPONENT_TYPE GetComponentType() { return m_eComponentType; }

protected:
	Entity* m_pOwnerEntity;
	COMPONENT_TYPE m_eComponentType;
};

#endif
