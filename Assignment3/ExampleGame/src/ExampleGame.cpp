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
#include "Assignment3/ExampleGame/ComponentRenderableSquare.h"
#include "Assignment3/ExampleGame/ComponentCamera.h"
#include "Assignment3/ExampleGame/ComponentCameraFollow.h"
#include "Assignment3/ExampleGame/ComponentCoinScore.h"
#include "Assignment3/ExampleGame/ComponentCoinLife.h"
#include "Assignment3/ExampleGame/ComponentCoinMovement.h"
#include "Assignment3/ExampleGame/ComponentCollision.h"
#include "Assignment3/ExampleGame/EventManager.h"
#include "Assignment3/ExampleGame/ComponentTimerLogic.h"
#include "Assignment3/ExampleGame/Textbox/TTextBox.h"
#include "Assignment3/ExampleGame/Textbox/TFont.h"
#include "Assignment3/ExampleGame/ComponentCameraFollow.h"
#include "Assignment3/ExampleGame/ComponentCamera.h"
#include "common/BulletPhysicsManager.h"
#include "Assignment3/ExampleGame/ComponentRigidBody.h"
#include "common\StateMachine.h"
#include "Assignment3\ExampleGame\GamePlayState.h"
#include "Assignment3\ExampleGame\PauseState.h"
#include "Assignment3\ExampleGame\States.h"
#include "Assignment3\ExampleGame\ComponentRenderableSprite.h"
#include "Assignment3\ExampleGame\ComponentMouseClick.h"

using namespace week2;

ExampleGame* ExampleGame::s_pInstance = NULL;

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
	m_pGameObjectManager(NULL),
	m_pStateMachine(NULL)
{
	s_pInstance = this;
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

	EventManager::CreateInstance();

	m_pGameObjectManager->RegisterComponentFactory("GOC_RenderableMesh", ComponentRenderableMesh::CreateComponent);
	m_pGameObjectManager->RegisterComponentFactory("GOC_AnimController", ComponentAnimController::CreateComponent);
	m_pGameObjectManager->RegisterComponentFactory("GOC_CharacterController", ComponentCharacterController::CreateComponent);
	m_pGameObjectManager->RegisterComponentFactory("GOC_RenderableSquare", ComponentRenderableSquare::CreateComponent);
	m_pGameObjectManager->RegisterComponentFactory("GOC_CoinScore", ComponentCoinScore::CreateComponent);
	m_pGameObjectManager->RegisterComponentFactory("GOC_LifeSpan", ComponentCoinLife::CreateComponent);
	m_pGameObjectManager->RegisterComponentFactory("GOC_CollisionSphere", ComponentCollision::CreateComponent);
	m_pGameObjectManager->RegisterComponentFactory("GOC_CoinMovement", ComponentCoinMovement::CreateComponent);
	m_pGameObjectManager->RegisterComponentFactory("GOC_TimerLogic", ComponentTimerLogic::CreateComponent);
	m_pGameObjectManager->RegisterComponentFactory("GOC_CameraFollow", ComponentCameraFollow::CreateComponent);
	m_pGameObjectManager->RegisterComponentFactory("GOC_Camera", ComponentCamera::CreateComponent);
	m_pGameObjectManager->RegisterComponentFactory("GOC_RigidBody", ComponentRigidBody::CreateComponent);
	m_pGameObjectManager->RegisterComponentFactory("GOC_MouseClick", ComponentMouseClick::CreateComponent);
	m_pGameObjectManager->RegisterComponentFactory("GOC_RenderableSprite", ComponentRenderableSprite::CreateComponent);

	m_pStateMachine = new Common::StateMachine();
	m_pStateMachine->RegisterState(eStateGame_Play, new GamePlayState());
	m_pStateMachine->RegisterState(eStateGame_Pause, new PauseState());
	m_pStateMachine->GoToState(eStateGame_Play);

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
	EventManager::Instance()->Update(p_fDelta);
	m_pGameObjectManager->Update(p_fDelta);
	m_pStateMachine->Update(p_fDelta);
	
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

	// Render physics debugging
	Common::BulletPhysicsManager::Instance()->Render(Common::SceneManager::Instance()->GetCamera()->GetProjectionMatrix(), 
													 Common::SceneManager::Instance()->GetCamera()->GetViewMatrix());
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