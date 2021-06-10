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
#include "Evade.h"
#include "Wander.h"
#include "Avoid.h"

#include "Alignment.h"
#include "Cohesion.h"
#include "Separation.h"

//STD
#include <iostream>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

//Constants
// settings
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 800;
const unsigned int NUM_OF_BOIDS = 50;

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
    const char* glsl_version = "#version 440 core";

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

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return false;
    }


    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);


    // configure global opengl state
    glEnable(GL_DEPTH_TEST);

    // build and compile shaders
    ourShader = new Shader("shaders/model_loading.vs", "shaders/model_loading.fs");

    // load models
    //m_pNanosuitModel = new Model("models/nanosuit/nanosuit.obj");
    m_pBoidModel = new Model("models/boid/boid.obj");
    m_pLeaderModel = new Model("models/leader/leader.obj");
    m_pContainmentModel = new Model("models/containment/containment.obj");
    m_pAvoidModel = new Model("models/avoid/avoid.obj");

    //camera
    camera = new Camera(glm::vec3(0.0f, 2.0f, 4.0f));

    //seed randGen
    srand(static_cast<unsigned>(time(nullptr)));

    //create avoid entities
    {
        Entity* pEntity = new Entity();
        TransformComponent* pTransformComponent;
        ModelComponent* pModelComponent;

        pEntity = new Entity();
        pTransformComponent = new TransformComponent(pEntity);
        pEntity->AddComponent(pTransformComponent);
        pModelComponent = new ModelComponent(pEntity);
        pModelComponent->SetModel(m_pContainmentModel);
        pModelComponent->m_fModelScale = 0.04f;
        pEntity->AddComponent(pModelComponent);
        m_vAvoid.push_back(pEntity);

        pEntity = new Entity();
        pTransformComponent = new TransformComponent(pEntity);
        pEntity->AddComponent(pTransformComponent);
        pModelComponent = new ModelComponent(pEntity);
        pModelComponent->SetModel(m_pAvoidModel);
        pModelComponent->m_fModelScale =0.004f;
        pEntity->AddComponent(pModelComponent);

        m_vAvoid.push_back(pEntity);
    }

    //create target entity
    {
        m_pTarget = new Entity();

        //transform Component
        TransformComponent* pTransformComponent = new TransformComponent(m_pTarget);
        pTransformComponent->SetEntityMatrixRow(POSITION_VECTOR, glm::vec3(
            RandomFloatBetweenRange(-3, 3),
            RandomFloatBetweenRange(-3, 3),
            RandomFloatBetweenRange(-3, 3)));
        m_pTarget->AddComponent(pTransformComponent);

        //model component
        ModelComponent* pModelComponent = new ModelComponent(m_pTarget);
        pModelComponent->SetModel(m_pLeaderModel);
        pModelComponent->m_fModelScale = 0.02f;
        m_pTarget->AddComponent(pModelComponent);

        //brain component
        BrainComponent* pBrainComponent = new BrainComponent(m_pTarget);
        m_pTarget->AddComponent(pBrainComponent);

        //physics component
        PhysicsComponent* pPhysicsComponent = new PhysicsComponent(m_pTarget);
        m_pTarget->AddComponent(pPhysicsComponent);

        Avoid* pAvoid = new Avoid(m_pTarget, m_vAvoid);
        pBrainComponent->AddBehaviour(0, pAvoid);
        pAvoid->SetScaleFactor(0.8f);

        Wander* pWander = new Wander(m_pTarget);
        pWander->SetMaxSpeed(2);
        pWander->SetSpeed(2);
        pBrainComponent->AddBehaviour(1, pWander);
        pWander->SetScaleFactor(0.2f);

    }

    //create Entities
    for (int i = 0; i < NUM_OF_BOIDS; ++i)
    {
        Entity* pEntity = new Entity();

        //transform Component
        TransformComponent* pTransformComponent = new TransformComponent(pEntity);
        pTransformComponent->SetEntityMatrixRow(POSITION_VECTOR, glm::vec3(
            RandomFloatBetweenRange(-3, 3),
            RandomFloatBetweenRange(-3, 3),
            RandomFloatBetweenRange(-3, 3)));
        pEntity->AddComponent(pTransformComponent);

        //model component
        ModelComponent* pModelComponent = new ModelComponent(pEntity);
        pModelComponent->SetModel(m_pBoidModel);
        pModelComponent->m_fModelScale = m_fBoidScale;
        pEntity->AddComponent(pModelComponent);

        //physics component
        PhysicsComponent* pPhysicsComponent = new PhysicsComponent(pEntity);
        pEntity->AddComponent(pPhysicsComponent);

        //brain component
        BrainComponent* pBrainComponent = new BrainComponent(pEntity);
        pEntity->AddComponent(pBrainComponent);


        Avoid* pAvoid = new Avoid(pEntity, m_vAvoid);
        pBrainComponent->AddBehaviour(0, pAvoid);
        pAvoid->SetScaleFactor(0.8f);

        UpdateBoidSteering(pEntity);
        //Pursue* pPursue = new Pursue(pEntity, &m_v3Target);
        //pBrainComponent->AddBehaviour(1, pPursue);
        //pPursue->SetScaleFactor(0.5);

        Separation* pSeparation = new Separation(pEntity, Entity::GetEntityMap());
        pBrainComponent->AddBehaviour(2, pSeparation);
        pSeparation->SetScaleFactor(0.5f);

        Alignment* pAlignment = new Alignment(pEntity, Entity::GetEntityMap());
        pBrainComponent->AddBehaviour(3, pAlignment);
        pAlignment->SetScaleFactor(0.1f);

        Cohesion* pCohesion = new Cohesion(pEntity, Entity::GetEntityMap());
        pBrainComponent->AddBehaviour(4, pCohesion);
        pCohesion->SetScaleFactor(0.2f);

        m_vBoids.push_back(pEntity);
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
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS && m_bEscReleased)
    { 
        m_bEscReleased = false;
        ToggleGui();
    }
    else if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_RELEASE)
    {
        m_bEscReleased = true;
    }

    if (!m_bShowGui) 
    {
        camera->processInput(window, fDeltaTime);
    }

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

    // add GUI
    if (m_bShowGui)
    {
        Gui();
    }

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    glfwSwapBuffers(window);
    glfwPollEvents();
}

void Scene::Deinitialise()
{
    delete camera;
    delete ourShader;
    delete m_pBoidModel;
    delete m_pLeaderModel;
    delete m_pAvoidModel;
    delete m_pContainmentModel;

    std::map<const unsigned int, Entity*>::const_iterator xIter;
    for (xIter = Entity::GetEntityMap().begin(); xIter != Entity::GetEntityMap().end(); ++xIter)
    {
        Entity* pEntity = xIter->second;
        delete pEntity;
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwDestroyWindow(window);
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

void Scene::UpdateBoidSteering(Entity* a_pBoid)
{
    BrainComponent* pBrainComponent = a_pBoid->FindBrainComponent();

    switch (static_cast<Steering>(m_eBoidSteering))
    {
    case Steering::Arrive:
    {
        Arrive* pArrive = new Arrive(a_pBoid, &m_v3Target);
        Behaviour* pOld = pBrainComponent->AddBehaviour(1, pArrive);
        pArrive->SetScaleFactor(m_fBoidArriveScaleFactor);
        delete pOld;
        break;
    }
    case Steering::Flee:
    {
        Flee* pFlee = new Flee(a_pBoid, &m_v3Target);
        Behaviour* pOld = pBrainComponent->AddBehaviour(1, pFlee);
        pFlee->SetScaleFactor(m_fBoidFleeScaleFactor);
        delete pOld;
        break;
    }
    case Steering::Seek:
    {
        Seek* pSeek = new Seek(a_pBoid, &m_v3Target);
        Behaviour* pOld = pBrainComponent->AddBehaviour(1, pSeek);
        pSeek->SetScaleFactor(m_fBoidSeekScaleFactor);
        delete pOld;
        break;
    }
    case Steering::Pursue:
    {
        Pursue* pPursue = new Pursue(a_pBoid, &m_v3Target);
        Behaviour* pOld = pBrainComponent->AddBehaviour(1, pPursue);
        pPursue->SetScaleFactor(m_fBoidPursueScaleFactor);
        delete pOld;
        break;
    }
    case Steering::Wander:
    {
        Wander* pWander = new Wander(a_pBoid);
        Behaviour* pOld = pBrainComponent->AddBehaviour(1, pWander);
        pWander->SetScaleFactor(m_fBoidWanderScaleFactor);
        delete pOld;
        break;
    }
    }
}

void Scene::ToggleGui()
{
    m_bShowGui = !m_bShowGui;

    if (m_bShowGui)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        glfwSetCursorPosCallback(window, nullptr);
        glfwSetScrollCallback(window, nullptr);
    }
    else
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetCursorPosCallback(window, mouse_callback);
        glfwSetScrollCallback(window, scroll_callback);
    }
}

void Scene::Gui()
{
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
    //if (show_demo_window)
    //    ImGui::ShowDemoWindow(&show_demo_window);

    bool open = true;
    if (!ImGui::Begin("Settings", &open, ImGuiWindowFlags_NoCollapse) || !open)
    {
        // Early out if the window is collapsed, as an optimization.
        ImGui::End();
        ImGui::EndFrame();
        ToggleGui();
        return;
    }

    ImGui::Text("Navigate with mouse and A W S D keys");
    ImGui::Text("ESC to toggle UI");
    ImGui::Separator();
  
    ImGui::Text("Exit the simulation");
    if (ImGui::Button("Exit"))
    {
        glfwSetWindowShouldClose(window, true);
    }
    ImGui::Separator();

    if (ImGui::CollapsingHeader("Target", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::Text("Tweak behaviour of the target boid");
        ImGui::SliderFloat("target scale", &m_pTarget->FindModelComponent()->m_fModelScale, 0.01f, 0.05f, "%.3f");
        if (ImGui::CollapsingHeader("Physics", ImGuiTreeNodeFlags_DefaultOpen))
        {
            PhysicsComponent* pPhysics = m_pTarget->FindPhysicsComponent();
            ImGui::SliderFloat("target mass", &pPhysics->m_fMass, 0.1f, 10.0f, "%.3f");
            ImGui::SliderFloat("tartet max force", &pPhysics->m_fMaxForce, 1.0f, 20.0f, "%.1f");
            ImGui::SliderFloat("target max velocity", &pPhysics->m_fMaxVelocity, 1.0f, 20.0f, "%.1f");
        }

        BrainComponent* pBrain = m_pTarget->FindBrainComponent();
        if (ImGui::CollapsingHeader("Avoid", ImGuiTreeNodeFlags_DefaultOpen))
        {
            Behaviour* pBehaviour = pBrain->GetBehaviour(0);
            ImGui::SliderFloat("avoid scale factor##target", &pBehaviour->m_fScaleFactor, 0.0f, 2.0f, "%.3f");

            Avoid* pAvoid = reinterpret_cast<Avoid*>(pBehaviour);
            ImGui::SliderFloat("avoid ray length##target", &pAvoid->m_fRayLength, 0.5f, 10.0f, "%.3f");
        }
        if (ImGui::CollapsingHeader("Wander", ImGuiTreeNodeFlags_DefaultOpen))
        {
            Behaviour* pBehaviour = pBrain->GetBehaviour(1);
            ImGui::SliderFloat("wander scale factor##target", &pBehaviour->m_fScaleFactor, 0.0f, 2.0f, "%.3f");

            Wander* pWander = reinterpret_cast<Wander*>(pBehaviour);
            ImGui::SliderFloat("wander distance#target", &pWander->m_fDistance, 0.2f, 10.0f, "%.3f");
            ImGui::SliderFloat("wander radius#target", &pWander->m_fRadius, 0.1f, pWander->m_fDistance - 0.1, "%.3f");
            ImGui::SliderFloat("wander jitter#target", &pWander->m_fJitter, 0.0f, pWander->m_fRadius, "%.3f");
        }
    }

    if (ImGui::CollapsingHeader("Flock", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::Text("Tweak behaviour of the flock");
        bool bUpdate = false;
        bUpdate = ImGui::SliderFloat("boid scale", &m_fBoidScale, 0.01f, 0.05f, "%.3f") || bUpdate;
        if (ImGui::CollapsingHeader("Physics", ImGuiTreeNodeFlags_DefaultOpen))
        {
            PhysicsComponent* pPhysics = m_pTarget->FindPhysicsComponent();
            bUpdate = ImGui::SliderFloat("boid mass", &m_fBoidMass, 0.1f, 10.0f, "%.3f") || bUpdate;
            bUpdate = ImGui::SliderFloat("boid max force", &m_fBoidMaxForce, 1.0f, 20.0f, "%.1f") || bUpdate;
            bUpdate = ImGui::SliderFloat("boid max velocity", &m_fBoidMaxVelocity, 1.0f, 20.0f, "%.1f") || bUpdate;
        }
        if (ImGui::CollapsingHeader("Avoid", ImGuiTreeNodeFlags_DefaultOpen))
        {
            bUpdate = ImGui::SliderFloat("avoid scale factor#boid", &m_fBoidAvoidScaleFactor, 0.0f, 2.0f, "%.3f") || bUpdate;
            bUpdate = ImGui::SliderFloat("avoid ray length#boid", &m_fBoidAvoidRayLength, 0.5f, 10.0f, "%.3f") || bUpdate;
        }

        if (ImGui::CollapsingHeader("Steering", ImGuiTreeNodeFlags_DefaultOpen))
        {
            const char* pNames[static_cast<int>(Steering::COUNT)] = { "Arrive", "Flee", "Pursue", "Seek", "Wander" };
            const char* pName = pNames[m_eBoidSteering];
            bUpdate = ImGui::SliderInt("steeing#boid", &m_eBoidSteering, 0, static_cast<int>(Steering::COUNT) - 1, pName) || bUpdate;
        }

        if (ImGui::CollapsingHeader("Separation", ImGuiTreeNodeFlags_DefaultOpen))
        {
            bUpdate = ImGui::SliderFloat("separation scale factor#boid", &m_fBoidSeparationScaleFactor, 0.0f, 2.0f, "%.3f") || bUpdate;
        }
        if (ImGui::CollapsingHeader("Alignment", ImGuiTreeNodeFlags_DefaultOpen))
        {
            bUpdate = ImGui::SliderFloat("alignment scale factor#boid", &m_fBoidAlignmentScaleFactor, 0.0f, 2.0f, "%.3f") || bUpdate;
        }
        if (ImGui::CollapsingHeader("Cohesion", ImGuiTreeNodeFlags_DefaultOpen))
        {
            bUpdate = ImGui::SliderFloat("cohesion scale factor#boid", &m_fBoidCohesionScaleFactor, 0.0f, 2.0f, "%.3f") || bUpdate;
        }

        if (bUpdate)
        {
            for (const auto boid : m_vBoids)
            {
                boid->FindModelComponent()->m_fModelScale = m_fBoidScale;

                PhysicsComponent* pPhysics = m_pTarget->FindPhysicsComponent();
                pPhysics->m_fMass = m_fBoidMass;
                pPhysics->m_fMaxForce = m_fBoidMaxForce;
                pPhysics->m_fMaxVelocity = m_fBoidMaxVelocity;

                BrainComponent* pBrain = boid->FindBrainComponent();

                Behaviour* pBehaviour = pBrain->GetBehaviour(0);
                pBehaviour->m_fScaleFactor = m_fBoidAvoidScaleFactor;
                Avoid* pAvoid = reinterpret_cast<Avoid*>(pBehaviour);
                pAvoid->m_fRayLength = m_fBoidAvoidRayLength;

                UpdateBoidSteering(boid);

                pBehaviour = pBrain->GetBehaviour(2);
                pBehaviour->m_fScaleFactor = m_fBoidSeparationScaleFactor;

                pBehaviour = pBrain->GetBehaviour(3);
                pBehaviour->m_fScaleFactor = m_fBoidAlignmentScaleFactor;

                pBehaviour = pBrain->GetBehaviour(4);
                pBehaviour->m_fScaleFactor = m_fBoidCohesionScaleFactor;
            }
        }
    }

    if (ImGui::CollapsingHeader("Avoid", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::Text("Tweak behaviour of the avoid meshes");
        ImGui::SliderFloat("containment scale", &m_vAvoid[0]->FindModelComponent()->m_fModelScale, 0.02f, 0.1f, "%.3f");
        ImGui::SliderFloat("cube scale", &m_vAvoid[1]->FindModelComponent()->m_fModelScale, 0.001f, 0.01f, "%.4f");
    }

    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
