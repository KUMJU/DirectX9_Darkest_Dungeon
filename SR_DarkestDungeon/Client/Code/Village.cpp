#include"pch.h"
#include "Village.h"

#include"Terrain.h"
#include "Wall.h"
#include"SkyBox.h"

#include "Player.h"
#include "Door.h"

#include"Layer.h"
#include"DynamicCamera.h"
#include "InteractionObj.h"

#include "Vestal.h"
#include "Jester.h"
#include "Highwayman.h"
#include "ShieldBreaker.h"

#include"Export_Utility.h"

CVillage::CVillage(LPDIRECT3DDEVICE9 pGraphicDev)
	:CScene(pGraphicDev)
{
}

CVillage::~CVillage()
{
}

HRESULT CVillage::ReadyScene()
{
	Ready_Layer_Environment(L"Layer_3_Environment");
	Ready_Layer_SkyBox(L"Layer_1_SkyBox");
	Ready_Layer_GameObject(L"Layer_4_GameObj");

	for (auto& iter : m_mapLayer) {
		//GameComponenet Setting
		iter.second->ReadyLayer();
	}

	return __super::ReadyScene();
}

_int CVillage::UpdateScene(const _float& fTimeDelta)
{
	int iExit;
	iExit = __super::UpdateScene(fTimeDelta);

	return iExit;
}

void CVillage::LateUpdateScene()
{
	__super::LateUpdateScene();

}

void CVillage::RenderScene()
{
}

HRESULT CVillage::Ready_Layer_Environment(tstring pLayerTag)
{
	shared_ptr<CLayer> m_pLayer = make_shared<CLayer>();
	m_mapLayer.insert({ pLayerTag, m_pLayer });

	//Camera
	shared_ptr<CGameObject> m_pCamera = make_shared<CDynamicCamera>(m_pGraphicDev);
	m_pLayer->CreateGameObject(L"Obj_Camera", m_pCamera);

	//Terrain
	Engine::CreateNewTexture(L"Village_FloorTexture", TEX_NORMAL,
		L"../Bin/Resource/Image/Village/Floor/road_citystone%d.png", 2);

	shared_ptr<CGameObject> m_pTerrain = make_shared<CTerrain>(m_pGraphicDev, L"Village_FloorTexture");
	m_pLayer->CreateGameObject(L"Obj_Terrain", m_pTerrain);

	//가장 최하위 순서에 돌려줄 것
	dynamic_pointer_cast<CLayer>(m_pLayer)->AwakeLayer();


	return S_OK;
}

HRESULT CVillage::Ready_Layer_SkyBox(tstring pLayerTag)
{
	shared_ptr<CLayer> m_pLayer = make_shared<CLayer>();
	m_mapLayer.insert({ pLayerTag, m_pLayer });

	// SkyBox
	Engine::CreateNewTexture(L"Village_SkyBoxTexture", TEX_CUBE,
		L"../Bin/Resource/Image/SkyBox/VillageSkyBox.dds", 1);

	shared_ptr<CGameObject> m_pSkyBox = make_shared<CSkyBox>(m_pGraphicDev, L"Village_SkyBoxTexture");
	m_pLayer->CreateGameObject(L"Obj_Village_SkyBox", m_pSkyBox);

	dynamic_pointer_cast<CLayer>(m_pLayer)->AwakeLayer();

	return S_OK;
}

HRESULT CVillage::Ready_Layer_GameObject(tstring pLayerTag)
{
	shared_ptr<CLayer> m_pLayer = make_shared<CLayer>();
	m_mapLayer.insert({ pLayerTag, m_pLayer });

	//플레이어
	shared_ptr<CGameObject> m_pPlayer = make_shared<CPlayer>(m_pGraphicDev);
	m_pLayer->CreateGameObject(L"Obj_Player", m_pPlayer);

	CGameMgr::GetInstance()->SetPlayer(m_pPlayer);
	shared_ptr<CGameObject> m_pPlayerHand = make_shared<CPlayerHand>(m_pGraphicDev);
	m_pLayer->CreateGameObject(L"Obj_PlayerHand", m_pPlayerHand);
	(dynamic_pointer_cast<CPlayer>(m_pPlayer))->SetPlayerHand(dynamic_pointer_cast<CPlayerHand>(m_pPlayerHand));

	// 인터랙션 오브젝트 테스트
	Engine::CreateNewTexture(L"Village_Door_Open", TEX_NORMAL,
		L"../Bin/Resource/Image/Village/Interaction/Door/open.png", 1);

	Engine::CreateNewTexture(L"Village_Door_Close", TEX_NORMAL,
		L"../Bin/Resource/Image/Village/Interaction/Door/closed.png", 1);

	shared_ptr<CGameObject> m_pDoor = make_shared<CDoor>(m_pGraphicDev);
	m_pLayer->CreateGameObject(L"Obj_Door", m_pDoor);

	// 영웅 테스트
	shared_ptr<CGameObject> m_pVestal = make_shared<CVestal>(m_pGraphicDev);
	m_pLayer->CreateGameObject(L"Obj_Vestal", m_pVestal);
	shared_ptr<CGameObject> m_pJester = make_shared<CJester>(m_pGraphicDev);
	m_pLayer->CreateGameObject(L"Obj_Jestal", m_pJester);
	shared_ptr<CGameObject> m_pHighwayman = make_shared<CHighwayman>(m_pGraphicDev);
	m_pLayer->CreateGameObject(L"Obj_Highwayman", m_pHighwayman);
	shared_ptr<CGameObject> m_pShieldBreaker = make_shared<CShieldBreaker>(m_pGraphicDev);
	m_pLayer->CreateGameObject(L"Obj_ShieldBreaker", m_pShieldBreaker);

	dynamic_pointer_cast<CLayer>(m_pLayer)->AwakeLayer();

	return S_OK;
}

HRESULT CVillage::Ready_Layer_UI(tstring pLayerTag)
{
	return S_OK;
}
