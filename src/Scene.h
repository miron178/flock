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
};

#endif // !SCENE_H
