//------------------------------------------------------------------------
// SceneManager
//
// Created:	2012/12/23
// Author:	Carel Boers
//	
// A simple scene manager to manage our models and camera.
//------------------------------------------------------------------------

#ifndef COMMON_SCENEMANAGER_H
#define COMMON_SCENEMANAGER_H

#include "W_Model.h"
#include "W_Sprite.h"
#include "SceneCamera.h"
#include "Assignment4\ExampleGame\Square.h"
#include "Assignment4\ExampleGame\ComponentPointLight.h"
#include "Assignment4\ExampleGame\Textbox\TTextBox.h"

namespace Common
{
	class SceneManager
	{
		// Typedef for convenience
		typedef std::vector<wolf::Model*> ModelList;
		typedef std::vector<wolf::Sprite*> SpriteList;
		typedef std::vector<Square*> SquareTextureList;
		typedef std::vector<PointLight*> PointLightList;

		// A directional light
		struct DirectionalLight
		{
			glm::vec3		m_vDirection;
			wolf::Color4    m_ambient;
			wolf::Color4	m_diffuse;
			wolf::Color4	m_specular;
			DirectionalLight() : m_diffuse(0,0,0,0), m_specular(0,0,0,0), m_ambient(0,0,0,0) {}    
		};

	public:
		//------------------------------------------------------------------------------
		// Public methods.
		//------------------------------------------------------------------------------
		static void CreateInstance();
		static void DestroyInstance();
		static SceneManager* Instance();

		void AddModel(wolf::Model* p_pModel);
		void RemoveModel(wolf::Model* p_pModel);
		void Clear();

		void AddSquareTexture(Square* p_pSquare);
		void RemoveSquareTexture(Square* p_oSquare);
		void ClearSquareTexure();

		void AddSprite(wolf::Sprite* p_pSprite);
		void RemoveSprite(wolf::Sprite* p_pSprite);
		void ClearSprites();

		void AddPointLight(PointLight* p_pPointLight);
		void RemovePointLight(PointLight* p_pPointLight);
		void ClearPointLights();

		void AttachCamera(SceneCamera* p_pCamera);

		SceneCamera* GetCamera();

		DirectionalLight* GetDefaultLight() { return m_pLight; }

		void Render();

		void Update(float p_fDelta);

		void AttachHUDTextBox(TTextBox *p_pTextBox) {m_pTextBox = p_pTextBox;};
		TTextBox* GetHUDTextBox() {return m_pTextBox;};
	private:
		//------------------------------------------------------------------------------
		// Private methods.
		//------------------------------------------------------------------------------
		
		// Constructor/Destructor are private because we're a Singleton
		SceneManager();
		virtual ~SceneManager();

	private:
		//------------------------------------------------------------------------------
		// Private members.
		//------------------------------------------------------------------------------

		// Static singleton instance
		static SceneManager* s_pSceneManagerInstance;

		// A list of models to render
		ModelList m_lModelList;

		// A list of squares/quads
		SquareTextureList m_lSquareList;

		// A list of sprites to render
		SpriteList m_lSpriteList;

		// A list of point lights
		PointLightList m_lPointLightList;

		// A camera to view the scene
		SceneCamera* m_pCamera;
		
		// Default directional light
		DirectionalLight* m_pLight;

		TTextBox* m_pTextBox;
	};

} // namespace common

#endif // COMMON_SCENEMANAGER_H