#include "ComponentTimerLogic.h"
#include "GameObjectManager.h"
#include "assignment2/ExampleGame/ComponentCoinMovement.h"
#include "assignment2/ExampleGame/ComponentCollision.h"
#include "assignment2/ExampleGame/ComponentCoinLife.h"
#include "assignment2/ExampleGame/ComponentCoinScore.h"
#include "assignment2/ExampleGame/src/ComponentRenderableMesh.h"
#include <algorithm>
#include "EventObjectCollision.h"
#include "EventCoinDisappeared.h"
#include "EventCoinSpawned.h"
#include "EventCoinCollected.h"

using namespace week2;

ComponentTimerLogic::ComponentTimerLogic()
{
	EventListener eventListener1 = std::tr1::bind(&ComponentTimerLogic::HandleCoinCollision, this, std::tr1::placeholders::_1);
	EventManager::Instance()->AddListener(Event_ObjectCollision, eventListener1);

	EventListener eventListener2 = std::tr1::bind(&ComponentTimerLogic::HandleCoinDisappeared, this, std::tr1::placeholders::_1);
	EventManager::Instance()->AddListener(Event_CoinDisappeared, eventListener2);
}

Common::ComponentBase* ComponentTimerLogic::CreateComponent(TiXmlNode* p_pNode)
{
	assert(strcmp(p_pNode->Value(), "GOC_TimerLogic") == 0);
	ComponentTimerLogic* pTimerComponent = new ComponentTimerLogic();

	TiXmlNode* pChildNode = p_pNode->FirstChild();
	while (pChildNode != NULL)
	{
		const char* szNodeName = pChildNode->Value();
		if (strcmp(szNodeName, "MaxObject") == 0)
		{
			// Parse attributes
			TiXmlElement* pElement = pChildNode->ToElement();

			const char* szValue = pElement->Attribute("value");
			if (szValue == NULL)
			{
				delete pTimerComponent;
				return NULL;
			}

			int iValue = atoi(szValue);
			pTimerComponent->SetMaxCoin(iValue);
		}

		if (strcmp(szNodeName, "Frequency") == 0)
		{
			// Parse attributes
			TiXmlElement* pElement = pChildNode->ToElement();

			const char* szValue = pElement->Attribute("value");
			if (szValue == NULL)
			{
				delete pTimerComponent;
				return NULL;
			}

			float fValue = atof(szValue);
			pTimerComponent->SetFrequency(fValue);
		}

		if (strcmp(szNodeName, "DefaultObject") == 0)
		{
			// Parse attributes
			TiXmlElement* pElement = pChildNode->ToElement();

			const char* szValue = pElement->Attribute("value");
			if (szValue == NULL)
			{
				delete pTimerComponent;
				return NULL;
			}

			std::string sValue = szValue;
			pTimerComponent->SetDefaultObject(sValue);
		}

		pChildNode = pChildNode->NextSibling();
	}

	return pTimerComponent;
}

void ComponentTimerLogic::Update(float p_fDelta)
{
	if (abs(m_fTimePassed) > 1000) m_fTimePassed = 0.0f; 
	m_fTimePassed += p_fDelta;
	if (m_fTimePassed > m_fFrequency)
	{
		m_fTimePassed = 0.0f;
		if (m_lCoinList.size() < m_iMaxNumCoin)
		{
			float randomX = rand()%20 -rand()%40;
			float randomZ = rand()%20 -rand()%40;;
	
			Common::GameObject* pCoin = this->GetGameObject()->GetManager()->CreateGameObject("Assignment2/ExampleGame/data/xml/" + m_sObjectName+".xml");

			pCoin->GetTransform().Translate(glm::vec3(randomX, 10.0f, randomZ));
			
			m_lCoinList.push_back(pCoin);

			EventManager::Instance()->QueueEvent(new EventCoinSpawned(pCoin));
		}
	}
}

void ComponentTimerLogic::HandleCoinCollision(BaseEvent *p_Event)
{
	EventObjectCollision *pEventCollision = static_cast<EventObjectCollision*>(p_Event);
	Common::GameObject *pCoin = pEventCollision->GetGameObject2();

	std::vector<Common::GameObject*>::iterator it = std::find(m_lCoinList.begin(), m_lCoinList.end(), pCoin);
	if (it != m_lCoinList.end())
	{
		m_lCoinList.erase(it);
	}	
	this->GetGameObject()->GetManager()->AddRemovedObject(pCoin);

	EventManager::Instance()->QueueEvent(new EventCoinCollected(pEventCollision->GetGameObject1(), pCoin));
	m_fTimePassed = 0.0f;
}

void ComponentTimerLogic::HandleCoinDisappeared(BaseEvent *p_Event)
{
	EventCoinDisappeared *pEventCollision = static_cast<EventCoinDisappeared*>(p_Event);
	Common::GameObject *pCoin = pEventCollision->GetCoin();

	std::vector<Common::GameObject*>::iterator it = std::find(m_lCoinList.begin(), m_lCoinList.end(), pCoin);
	if (it != m_lCoinList.end())
	{
		m_lCoinList.erase(it);
	}	
	m_fTimePassed = 0.0f;
}