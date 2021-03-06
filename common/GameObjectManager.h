//------------------------------------------------------------------------
// GameObjectManager
//
// Created:	2012/12/26
// Author:	Carel Boers
//	
// Manages the collection of Game Objects used by a game.
//------------------------------------------------------------------------

#ifndef GAMEOBJECTMANAGER_H
#define GAMEOBJECTMANAGER_H

#include "GameObject.h"
#include "LuaScriptManager.h"
#include <map>
#include <vector>
#include "tinyxml.h"	// For parsing GameObject XML

namespace Common
{
	class GameObjectManager
	{
	public:
		// Typedef for convenience
		typedef std::map<std::string, GameObject*> GameObjectMap;
		typedef std::vector<GameObject*> GameObjectList;	

		typedef ComponentBase*(*ComponentFactoryMethod)(TiXmlNode* p_pNode);
		typedef std::map<std::string, ComponentFactoryMethod> ComponentFactoryMap;

		//---------------------------------------------------------------------
		// Public interface
		//---------------------------------------------------------------------
		GameObjectManager();
		~GameObjectManager();

		GameObject* CreateGameObject();
		void DestroyGameObject(GameObject* p_pGameObject);
		void DestroyAllGameObjects();
		GameObject* GetGameObject(const std::string &p_strGOGUID);
		bool SetGameObjectGUID(GameObject* p_pGameObject, const std::string &p_strGOGUID);

		// Data driven helpers
		GameObject* CreateGameObject(const std::string& p_strGameObject);
		void RegisterComponentFactory(const std::string& p_strComponentId, ComponentFactoryMethod);

		void Update(float p_fDelta);
		void SyncTransforms();

		// Iterator access to allow outside traversal of GameObjects
		GameObjectMap::iterator Begin()	{ return m_mGOMap.begin(); }
		GameObjectMap::iterator End()	{ return m_mGOMap.end(); }

		// Lua bindings
		static void ExportToLua();
		static LuaPlus::LuaObject LuaNew();
		LuaPlus::LuaObject LuaCreateGameObject();
		LuaPlus::LuaObject LuaCreateGameObjectXML(const char* p_strPath);

		void AddRemovedObject(GameObject *p_pObject);
		bool CheckRemoveObject(GameObject *p_pObject);
		void CheckCollision();
	private:
		//---------------------------------------------------------------------
		// Private members
		//---------------------------------------------------------------------

		// Map of Game Objects
		GameObjectMap m_mGOMap;

		// Map of Component factories
		ComponentFactoryMap m_mComponentFactoryMap;

		// list to be removed next frame
		GameObjectList m_lRemoveGOList;
	};
} // namespace Common

#endif // GAMEOBJECTMANAGER_H