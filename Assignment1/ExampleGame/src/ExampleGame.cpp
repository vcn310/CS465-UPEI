//------------------------------------------------------------------------
// ExampleGame
//
// Created:	2012/12/06
// Author:	Carel Boers
//	
// Main game class. Derives from Common::Game.
//------------------------------------------------------------------------

#include "ComponentAnimController.h"
#include "ComponentCharacterController.h"
#include "ComponentRenderableMesh.h"
#include "ExampleGame.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "W_Model.h"
#include "Assignment1/ExampleGame/ComponentRenderableSquare.h"
#include "Assignment1/ExampleGame/ComponentCamera.h"

using namespace week2;

//------------------------------------------------------------------------------
// Method:    ExampleGame
// Parameter: void
// Returns:   
// 
// Constructor
//------------------------------------------------------------------------------
ExampleGame::ExampleGame()
	: 
	m_pSceneCamera(NULL),
	m_pGameObjectManager(NULL)
{
}

//------------------------------------------------------------------------------
// Method:    ~ExampleGame
// Parameter: void
// Returns:   
// 
// Destructor.
//------------------------------------------------------------------------------
ExampleGame::~ExampleGame()
{
	assert(!m_pGameObjectManager);
	assert(!m_pSceneCamera);
}

//------------------------------------------------------------------------------
// Method:    Init
// Returns:   bool
// 
// Overridden from Common::Game::Init(). Called when it's time to initialize our
// game. Returns true on successful initialization, false otherwise. If false is
// returned, the game will shut down.
//------------------------------------------------------------------------------
bool ExampleGame::Init()
{
	// Initialize our Scene Manager
	Common::SceneManager::CreateInstance();

	// Initialize our Scene Camera and attach it to the Scene Manager
	m_pSceneCamera = new Common::SceneCamera(45.0f, 1280.0f / 720.0f, 0.1f, 1000.0f, glm::vec3(0.0f, 5.0f, 15.0f), glm::vec3(0.0f,5.0f,0.0f), glm::vec3(0.0f,1.0f,0.0f));
	Common::SceneManager::Instance()->AttachCamera(m_pSceneCamera);

	// Initialize our GameObjectManager
	m_pGameObjectManager = new Common::GameObjectManager();

	// Create an empty GameObject
	Common::GameObject* pCharacter = m_pGameObjectManager->CreateGameObject();
	m_pGameObjectManager->SetGameObjectGUID(pCharacter, "character");
	pCharacter->GetTransform().Scale(glm::vec3(0.05f, 0.05, 0.05f));
	pCharacter->GetTransform().SetTranslation(glm::vec3(3.0f, 0.0, 0.0f));
	pCharacter->GetTransform().Rotate(glm::vec3(0.0f,40.0f,0.0f));

	// Create a renderable component for it
	ComponentRenderableMesh* pRenderableComponent = new ComponentRenderableMesh();
	pRenderableComponent->Init("Assignment1/ExampleGame/data/character/vincent.pod", "Assignment1/ExampleGame/data/character/", "Assignment1/ExampleGame/data/skinned.vsh", "Assignment1/ExampleGame/data/skinned.fsh");
	pCharacter->AddComponent(pRenderableComponent);

	// Create an animation controller component for it
	ComponentAnimController* pAnimControllerComponent = new ComponentAnimController();
	pAnimControllerComponent->AddAnim("idle", 1, 120, true);
	pAnimControllerComponent->AddAnim("run", 252, 276, true);
	pAnimControllerComponent->SetAnim("idle");
	pCharacter->AddComponent(pAnimControllerComponent); 

	// Create a controller component for it
	ComponentCharacterController* pCharacterControllerComponent = new ComponentCharacterController();
	pCharacter->AddComponent(pCharacterControllerComponent);

	// create a camara compoent for character;
	ComponentCamera* pPlayerCamera = new ComponentCamera();
	pPlayerCamera->SetCamera(m_pSceneCamera);
    pCharacter->AddComponent(pPlayerCamera);
	
	// lamp post
    Common::GameObject* pLamp = m_pGameObjectManager->CreateGameObject();
    m_pGameObjectManager->SetGameObjectGUID(pLamp, "lamp");
	pLamp->GetTransform().Scale(glm::vec3(0.4f, 0.4, 0.4f));

    ComponentRenderableMesh* pRenderableComponentLamp = new ComponentRenderableMesh();
	pRenderableComponentLamp->Init("Assignment1/ExampleGame/data/props/lamp.pod", "Assignment1/ExampleGame/data/props/", "Assignment1/ExampleGame/data/shaders/textured.vsh", "Assignment1/ExampleGame/data/shaders/textured.fsh");
	pLamp->AddComponent(pRenderableComponentLamp);

	// create a camara compoent for lamp;
	ComponentCamera* pLampCamera = new ComponentCamera();	
	pLampCamera->GetCamera()->SetPos(glm::vec3(0.0f,25.0f,0.0f));
	pLamp->AddComponent(pLampCamera);

	Common::GameObject* pGround = m_pGameObjectManager->CreateGameObject();
    ComponentRenderableSquare* pRenderableSquare = new ComponentRenderableSquare();
    pRenderableSquare->Init("Assignment1/ExampleGame/data/textures/ground.tga", "Assignment1/ExampleGame/data/shaders/textured.vsh", "Assignment1/ExampleGame/data/shaders/textured.fsh",500.0f, 0.0, 500.0f);
	pGround->AddComponent(pRenderableSquare);

	// Everything initialized OK.
	return true;
}

//------------------------------------------------------------------------------
// Method:    Update
// Parameter: float p_fDelta
// Returns:   bool
// 
// Overridden from Common::Game::Update(). Called once per frame during the main
// loop of the game. The game state is updated here. Returns true on success, 
// false otherwise. If false is returned, the game will shutdown.
//------------------------------------------------------------------------------
bool ExampleGame::Update(float p_fDelta)
{
	m_pGameObjectManager->Update(p_fDelta);

	if (glfwGetKey('C') == GLFW_PRESS && !m_bToggleCamera)
    {
		m_b3PCamera = !m_b3PCamera;
        m_bToggleCamera = true;
    }
    else if (glfwGetKey('C') == GLFW_RELEASE)
        m_bToggleCamera = false;

	Common::SceneCamera* pCamera;
	if (m_b3PCamera)
    {
        pCamera = static_cast<ComponentCamera*>(m_pGameObjectManager->
            GetGameObject("character")->GetComponent("GOC_Camera"))->GetCamera();
    }
    else
        pCamera = static_cast<ComponentCamera*>(m_pGameObjectManager->
            GetGameObject("lamp")->GetComponent("GOC_Camera"))->GetCamera();
  
	Common::SceneManager::Instance()->AttachCamera(pCamera);

	return true;
}

//------------------------------------------------------------------------------
// Method:    Render
// Returns:   void
// 
// Overridden from Common::Game::Render(). Called once per frame during the main
// loop of the game. Any objects in the game that need to be drawn are rendered
// here.
//------------------------------------------------------------------------------
void ExampleGame::Render()
{
	// Sync transforms to render components
	m_pGameObjectManager->SyncTransforms();

	// Render the scene
	Common::SceneManager::Instance()->Render();
}

//------------------------------------------------------------------------------
// Method:    Shutdown
// Returns:   void
// 
// Called when the game is shutting down. All dynamic memory needs to be cleaned
// up.
//------------------------------------------------------------------------------
void ExampleGame::Shutdown()
{
	// Clear our Game Objects
	m_pGameObjectManager->DestroyAllGameObjects();
	delete m_pGameObjectManager;
	m_pGameObjectManager = NULL;

	// Delete our camera
	if (m_pSceneCamera)
	{
		delete m_pSceneCamera;
		m_pSceneCamera = NULL;
	}

	// Destroy the Scene Manager
	Common::SceneManager::DestroyInstance();
}