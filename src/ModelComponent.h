#ifndef MODEL_COMPONENT_H
#define MODEL_COMPONENT_H

#include "Component.h"
#include <glm/glm.hpp>

class Model;

class ModelComponent : public Component
{
public:
    ModelComponent(Entity* a_pOwner);
    ~ModelComponent();

    virtual void Update(float a_fDeltaTime) {};
    virtual void Draw(Shader* a_pShader);

    Model* GetModel() const { return m_pModelData; }
    void SetModel(Model* a_pNewModel) { m_pModelData  = a_pNewModel; }

    void SetScale(float a_fNewScale){ m_fModelScale = a_fNewScale; }
    glm::mat4 GetModelMatrix() const;

private:
    Model* m_pModelData;
    float m_fModelScale;
};

#endif
