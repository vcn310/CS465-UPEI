//------------------------------------------------------------------------
// ExampleGame
//
// Created:	2012/12/06
// Author:	Carel Boers
//	
// Main game class. Derives from Common::Game.
//------------------------------------------------------------------------

#include "common/BulletPhysicsManager.h"
#include "ComponentAnimController.h"
#include "ComponentCharacterController.h"
#include "ComponentRenderableMesh.h"
#include "ComponentRigidBody.h"
#include "ExampleGame.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "W_Model.h"

using namespace week7;

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
	m_pSceneCamera = new Common::SceneCamera(45.0f, 1280.0f / 720.0f, 0.1f, 1000.0f, glm::vec3(0.0f, 15.0f, 25.0f), glm::vec3(0.0f,5.0f,0.0f), glm::vec3(0.0f,1.0f,0.0f));
	Common::SceneManager::Instance()->AttachCamera(m_pSceneCamera);

	// Initialize our GameObjectManager
	m_pGameObjectManager = new Common::GameObjectManager();

	// Register factory methods
	m_pGameObjectManager->RegisterComponentFactory("GOC_AnimController", ComponentAnimController::CreateComponent);

	// Create a Physics Manager to manage physics simulation
	Common::BulletPhysicsManager::CreateInstance("week7/ExampleGame/data/physics_materials.xml",
												"week7/ExampleGame/data/shaders/lines.vsh", 
												"week7/ExampleGame/data/shaders/lines.fsh");

	// Create an initialized GameObject
	Common::GameObject* pCharacter = m_pGameObjectManager->CreateGameObject();
	m_pGameObjectManager->SetGameObjectGUID(pCharacter, "Character");
	pCharacter->GetTransform().Scale(glm::vec3(0.05f, 0.05, 0.05f));

	// Create a renderable component for it
	ComponentRenderableMesh* pRenderableComponent = new ComponentRenderableMesh();
	pRenderableComponent->Init("week7/ExampleGame/data/character/vincent.pod", "week7/ExampleGame/data/character/", "week7/ExampleGame/data/shaders/skinned.vsh", "week7/ExampleGame/data/shaders/skinned.fsh");
	pCharacter->AddComponent(pRenderableComponent);

	// Create an animation controller component for it
	ComponentAnimController* pAnimControllerComponent = new ComponentAnimController();
	pAnimControllerComponent->AddAnim("idle", 1, 120, true);
	pAnimControllerComponent->AddAnim("run", 252, 276, true);
	pAnimControllerComponent->AddAnim("walk", 305, 340, true);
	pAnimControllerComponent->SetAnim("idle");
	pCharacter->AddComponent(pAnimControllerComponent); 

	// Add a rigid body to the player
	Common::ComponentRigidBody* pRigidBody = new Common::ComponentRigidBody();
	pCharacter->AddComponent(pRigidBody);
	pRigidBody->Init(new btCapsuleShape(2.0f, 6.0f), "Normal", 20.0f, glm::vec3(0.0f, -3.0f, 0.0f), true /* Kinematic */);

	// Create a controller component for it
	ComponentCharacterController* pCharacterControllerComponent = new ComponentCharacterController();
	pCharacter->AddComponent(pCharacterControllerComponent);

	// Ground Game Object
	Common::GameObject* pGround = m_pGameObjectManager->CreateGameObject();
	m_pGameObjectManager->SetGameObjectGUID(pGround, "Ground");
	pRenderableComponent = new ComponentRenderableMesh();
	pRenderableComponent->Init("week7/ExampleGame/data/ground.pod", "week7/ExampleGame/data/", "week7/ExampleGame/data/shaders/textured.vsh", "week7/ExampleGame/data/shaders/textured.fsh");
	pGround->AddComponent(pRenderableComponent);
	pRigidBody = new Common::ComponentRigidBody();
	pGround->AddComponent(pRigidBody);
	pRigidBody->Init(new btStaticPlaneShape(btVector3(0,1,0), 0), "Normal", 0.0f, glm::vec3());

	// Create 100 crates at random positions
	for (int i = 0; i < 2; ++i)
	{
		// Crate Game Object
		Common::GameObject* pCrate = m_pGameObjectManager->CreateGameObject();
		char buff[64];
		sprintf(buff, "Crate%d", i);
		m_pGameObjectManager->SetGameObjectGUID(pCrate, buff);

		// Random position
		glm::vec3 vPosition;
		vPosition.x = (rand() % 10) - 5;
		vPosition.y = (rand() % 10) + 5;
		vPosition.z = (rand() % 10) - 5;

		pCrate->GetTransform().SetTranslation(vPosition);
		pRenderableComponent = new ComponentRenderableMesh();
		pRenderableComponent->Init("week7/ExampleGame/data/crate.pod", "week7/ExampleGame/data/", "week7/ExampleGame/data/shaders/textured.vsh", "week7/ExampleGame/data/shaders/textured.fsh");
		pCrate->AddComponent(pRenderableComponent);
		pRigidBody = new Common::ComponentRigidBody();
		pCrate->AddComponent(pRigidBody);

		pRigidBody->Init(new btBoxShape(btVector3(1.5f, 1.5f, 1.5f)), "Normal", 2.0f, glm::vec3(0.0f, -1.5f, 0.0f));
		//pRigidBody->Init(new btSphereShape(btVector3(1.5f, 1.5f, 1.5f)), "Normal", 2.0f, glm::vec3(0.0f, -1.5f, 0.0f));
		//pRigidBody->Init(new btSphereShape(btScalar(3.0f)), "Normal",2.0f, glm::vec3(0.0f, -1.5f, 0.0f));
		//pRigidBody->Init(new btConeShape(btScalar(1.5f), btScalar(6.0f)), "Normal", 2.0f, glm::vec3(0.0f, -1.5f, 0.0f));
	}

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
	Common::BulletPhysicsManager::Instance()->Update(p_fDelta);
	m_pGameObjectManager->Update(p_fDelta);

	// Toggle physics debug rendering
	static bool bLastKeyDown = false;
	bool bCurrentKeyDown = glfwGetKey('Z');
	if (bCurrentKeyDown && !bLastKeyDown)
	{
		//Common::BulletPhysicsManager::Instance()->ToggleDebugRendering();
		std::map<std::string, Common::GameObject*>::iterator iter;
		for (iter = m_pGameObjectManager->Begin(); iter != m_pGameObjectManager->End(); iter++)
		{
			Common::GameObject* pGO = iter->second;
			Common::ComponentRigidBody* pRigidBodyCompoonent = static_cast<Common::ComponentRigidBody*>(pGO->GetComponent("GOC_RigidBody"));
			btRigidBody* pBody = pRigidBodyCompoonent->getRigidBody();
			//pBody->setActivationState(DISABLE_DEACTIVATION);
			pBody->applyCentralImpulse(btVector3(10.0f, 0.0f, 0.0f));
		}
	}
	bLastKeyDown = bCurrentKeyDown;

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

	// Destroy physics manager
	Common::BulletPhysicsManager::DestroyInstance();

	// Destroy the Scene Manager
	Common::SceneManager::DestroyInstance();
}