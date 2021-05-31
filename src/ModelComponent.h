#ifndef MODEL_COMPONENT_H
#define MODEL_COMPONENT_H

#include "Component.h"

class Model;

class ModelComponent : public Component
{
public:
    ModelComponent(Entity* a_pOwner);
    ~ModelComponent();

    virtual void Update(float a_fDeltaTime) {};
    virtual void Draw(Shader* a_pShader);

    void SetModel(Model* pNewModel) { m_pModelData = pNewModel; }

private:
    Model* m_pModelData;
    float m_fModelScale;
};

#endif
