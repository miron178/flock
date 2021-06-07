#include "Scene.h"

//openGL
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <learnopengl/shader.h>
#include <learnopengl/camera.h>
#include <learnopengl/model.h>

//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//project includes
#include "Entity.h"
#include "TransformComponent.h"
#include "ModelComponent.h"
#include "BrainComponent.h"
#include "PhysicsComponent.h"
#include "Seek.h"
#include "Flee.h"
#include "Pursue.h"
#include "Wander.h"

//STD
#include <iostream>

//Constants
// settings
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 800;
const unsigned int NUM_OF_BOIDS = 100;

Scene* Scene::s_pSceneInstanc = nullptr;

Scene* Scene::GetInstance()
{
	if (s_pSceneInstanc == nullptr)
	{
		s_pSceneInstanc = new Scene();
	}
	return s_pSceneInstanc;
}

Scene::Scene()
{
    m_fLastX = SCR_WIDTH / 2.0f;
    m_fLastY = SCR_HEIGHT / 2.0f;
    fDeltaTime = 0.0f;
    fLastFrame = 0.0f;
    m_bFirstMouse=true;
    camera = nullptr;
    window = nullptr;

    m_v3Target = glm::vec3(4.0f, 0.0f, 4.0f);
}

bool Scene::Initialise()
{
    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return false;
    }

    // configure global opengl state
    glEnable(GL_DEPTH_TEST);

    // build and compile shaders
    ourShader = new Shader("shaders/model_loading.vs", "shaders/model_loading.fs");

    // load models
    m_pNanosuitModel= new Model("models/nanosuit/nanosuit.obj");

    //camera
    camera = new Camera(glm::vec3(0.0f, 2.0f, 8.0f));

    //seed randGen
    srand(static_cast<unsigned>(time(nullptr)));

    //create target entity
    m_pTarget = new Entity();

    //transform Component
    TransformComponent* pTransformComponent = new TransformComponent(m_pTarget);
    pTransformComponent->SetEntityMatrixRow(POSITION_VECTOR, glm::vec3(RandomFloatBetweenRange(0, 5),
        0.0f,
        RandomFloatBetweenRange(0, 5)));
    m_pTarget->AddComponent(pTransformComponent);

    //model component
    ModelComponent* pModelComponent = new ModelComponent(m_pTarget);
    pModelComponent->SetModel(m_pNanosuitModel);
    pModelComponent->SetScale(0.02f);
    m_pTarget->AddComponent(pModelComponent);

    //brain component
    BrainComponent* pBrainComponent = new BrainComponent(m_pTarget);
    m_pTarget->AddComponent(pBrainComponent);

    //physics component
    PhysicsComponent* pPhysicsComponent = new PhysicsComponent(m_pTarget);
    m_pTarget->AddComponent(pPhysicsComponent);

    Wander* pWander = new Wander(pTransformComponent, pPhysicsComponent);
    pWander->SetMaxSpeed(2);
    pWander->SetSpeed(2);
    pBrainComponent->AddBehaviour(0, pWander);

    //create Entities
    for (int i = 0; i < NUM_OF_BOIDS; ++i)
    {
        Entity* pEntity = new Entity();

        //transform Component
        TransformComponent* pTransformComponent = new TransformComponent(pEntity);
        pTransformComponent->SetEntityMatrixRow(POSITION_VECTOR, glm::vec3( RandomFloatBetweenRange(0, 5),
                                                                            0.0f,
                                                                            RandomFloatBetweenRange(0, 5) ) );
        pEntity->AddComponent(pTransformComponent);

        //model component
        ModelComponent* pModelComponent = new ModelComponent(pEntity);
        pModelComponent->SetModel(m_pNanosuitModel);
        pModelComponent->SetScale(0.02f);
        pEntity->AddComponent(pModelComponent);

        //physics component
        PhysicsComponent* pPhysicsComponent = new PhysicsComponent(pEntity);
        pEntity->AddComponent(pPhysicsComponent);

        //brain component
        BrainComponent* pBrainComponent = new BrainComponent(pEntity);
        pEntity->AddComponent(pBrainComponent);

        //Seek* pSeek = new Seek(pEntity->FindTransformComponent(), &m_v3Target, pPhysicsComponent);
        //pBrainComponent->AddBehaviour(0, pSeek);

        //Pursue* pPursue = new Pursue(pEntity->FindTransformComponent(), &m_v3Target, pPhysicsComponent);
        //pBrainComponent->AddBehaviour(0, pPursue);

        Flee* pFlee = new Flee(pEntity->FindTransformComponent(), &m_v3Target, pPhysicsComponent);
        pBrainComponent->AddBehaviour(0, pFlee);
    }

    return true;
}

bool Scene::Update()
{
    // per-frame time logic
    float currentFrame = static_cast<float>(glfwGetTime());
    fDeltaTime = currentFrame - fLastFrame;
    fLastFrame = currentFrame;

    // input
    camera->processInput(window, fDeltaTime);

    //update entities
    std::map<const unsigned int, Entity*>::const_iterator xIter;
    for (xIter = Entity::GetEntityMap().begin(); xIter != Entity::GetEntityMap().end(); ++xIter)
    {
        Entity* pEntity = xIter->second;
        if (pEntity)
        {
            pEntity->Update(fDeltaTime);
        }
    }

    TransformComponent* pTargetTransform = m_pTarget->FindTransformComponent();
    m_v3Target = pTargetTransform->GetEntityMatrixRow(POSITION_VECTOR);

    //return close or not
	return !glfwWindowShouldClose(window);
}

void Scene::Render()
{
    // render
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // don't forget to enable shader before setting uniforms
    ourShader->use();

    // view/projection transformations
    glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = camera->GetViewMatrix();
    ourShader->setMat4("projection", projection);
    ourShader->setMat4("view", view);

    //render entities
    std::map<const unsigned int, Entity*>::const_iterator xIter;
    for (xIter = Entity::GetEntityMap().begin(); xIter != Entity::GetEntityMap().end(); ++xIter)
    {
        Entity* pEntity = xIter->second;
        if (pEntity)
        {
            pEntity->Draw(ourShader);
        }
    }

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    glfwSwapBuffers(window);
    glfwPollEvents();
}

void Scene::Deinitialise()
{
    delete camera;
    delete ourShader;
    delete m_pNanosuitModel;

    std::map<const unsigned int, Entity*>::const_iterator xIter;
    for (xIter = Entity::GetEntityMap().begin(); xIter != Entity::GetEntityMap().end(); ++xIter)
    {
        Entity* pEntity = xIter->second;
        delete pEntity;
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void Scene::framebuffer_size_callback(GLFWwindow* a_window, int a_width, int a_height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, a_width, a_height);
}

// glfw: whenever the mouse moves, this callback is called
void Scene::mouse_callback(GLFWwindow* a_window, double a_xpos, double a_ypos)
{
    Scene* pScene = Scene::GetInstance();
    if (!pScene)
    {
        return; //stop program if there is no scene
    }
    if (pScene->m_bFirstMouse)
    {
        pScene->m_fLastX = static_cast<float>(a_xpos);
        pScene->m_fLastY = static_cast<float>(a_ypos);
        pScene->m_bFirstMouse = false;
    }

    float xoffset = static_cast<float>(a_xpos) - pScene->m_fLastX;
    float yoffset = pScene->m_fLastY - static_cast<float>(a_ypos); // reversed since y-coordinates go from bottom to top

    pScene->m_fLastX = static_cast<float>(a_xpos);
    pScene->m_fLastY = static_cast<float>(a_ypos);

    if (pScene->camera)
    {
        pScene->camera->ProcessMouseMovement(xoffset, yoffset);
    }
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
void Scene::scroll_callback(GLFWwindow* a_window, double a_xoffset, double a_yoffset)
{
    Scene* pScene = Scene::GetInstance();
    if (!pScene || !pScene->camera)
    {
        return; //stop program
    }
    pScene->camera->ProcessMouseScroll(static_cast<float>(a_yoffset));
}

int Scene::RandomNumberBetweenRange(int a_iLowerRange, int a_iUpperRange)
{
    return rand() % (glm::abs(a_iLowerRange - a_iUpperRange)) + (a_iLowerRange);
}

float Scene::RandomFloatBetweenRange(float a_fLowerRange, float a_fUpperRange)
{
    return a_fLowerRange + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (a_fUpperRange - a_fLowerRange)));
}
