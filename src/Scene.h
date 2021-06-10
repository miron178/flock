#ifndef SCENE_H
#define SCENE_H

#include <glm/glm.hpp>
#include <vector>

//forward decloration
struct GLFWwindow;
class Camera;
class Model;
class Shader;
class Entity;

class Scene
{
public:
	static Scene* GetInstance();

	bool Initialise();
	bool Update();
	void Render();
	void Deinitialise();

private:
	Scene();
	Scene(const Scene&);
	Scene& operator=(const Scene&);

	static void framebuffer_size_callback(GLFWwindow* a_window, int a_width, int a_height);
	static void mouse_callback(GLFWwindow* a_window, double a_xpos, double a_ypos);
	static void scroll_callback(GLFWwindow* a_window, double a_xoffset, double a_yoffset);

	static Scene* s_pSceneInstanc;

	int RandomNumberBetweenRange(int a_iLowerRange, int a_iUpperRange);
	float RandomFloatBetweenRange(float a_fLowerRange, float a_fUpperRange);

	enum class Steering
	{
		Arrive,
		Flee,
		Pursue,
		Seek,
		Wander,
		COUNT
	};

	void UpdateBoidSteering(Entity* a_pBoid);

	void ToggleGui();
	void Gui();

	GLFWwindow* window;
	Camera* camera;
	Model* m_pBoidModel;
	Model* m_pLeaderModel;
	Model* m_pAvoidModel;
	Model* m_pContainmentModel;
	Shader* ourShader;

	float m_fLastX;
	float m_fLastY;
	bool m_bFirstMouse;

	// timing
	float fDeltaTime;
	float fLastFrame;

	glm::vec3 m_v3Target;
	Entity* m_pTarget = nullptr;

	std::vector<const Entity*> m_vAvoid;

	bool m_bEscReleased = true;
	bool m_bShowGui = true;

	std::vector<Entity*> m_vBoids;
	float m_fBoidScale = 0.02f;
	
	float m_fBoidMass = 1.0f;
	float m_fBoidMaxForce = 10.0f;
	float m_fBoidMaxVelocity = 10.0f;

	float m_fBoidAvoidScaleFactor = 0.08f;
	float m_fBoidAvoidRayLength = 1.0f;

	int m_eBoidSteering = static_cast<int>(Steering::Pursue);

	float m_fBoidArriveScaleFactor = 0.5f;
	float m_fBoidFleeScaleFactor = 0.5f;
	float m_fBoidPursueScaleFactor = 0.5f;
	float m_fBoidSeekScaleFactor = 0.5f;
	float m_fBoidWanderScaleFactor = 0.5f;

	float m_fBoidSeparationScaleFactor = 0.5f;
	float m_fBoidSeparationNeighbourRadius = 1.0f;

	float m_fBoidAlignmentScaleFactor = 0.1f;
	float m_fBoidAlignmentNeighbourRadius = 1.0f;

	float m_fBoidCohesionScaleFactor = 0.2f;
	float m_fBoidCohesionNeighbourRadius = 1.0f;
};

#endif // !SCENE_H
