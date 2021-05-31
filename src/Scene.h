#ifndef SCENE_H
#define SCENE_H

//forward decloration
struct GLFWwindow;
class Camera;
class Model;
class Shader;

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

	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

	static Scene* s_pSceneInstanc;

	int RandomNumberBetweenRange(int iLowerRange, int iUpperRange);
	float RandomFloatBetweenRange(float fLowerRange, float fUpperRange);
	
	GLFWwindow* window;
	Camera* camera;
	Model* m_pNanosuitModel;
	Shader* ourShader;

	float m_fLastX;
	float m_fLastY;
	bool m_bFirstMouse;

	// timing
	float fDeltaTime;
	float fLastFrame;
};

#endif // !SCENE_H
