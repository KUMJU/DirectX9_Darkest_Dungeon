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
	Ready_Layer_Environment(L"Layer_Environment");
	Ready_Layer_SkyBox(L"Layer_SkyBox");
	Ready_Layer_GameObject(L"Layer_GameObj");

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

	shared_ptr<CGameObject> m_pPlayer = make_shared<CPlayer>(m_pGraphicDev);
	m_pLayer->CreateGameObject(L"Obj_Player", m_pPlayer);

	Engine::CGameMgr::GetInstance()->SetPlayer(m_pPlayer);

	Engine::CreateNewTexture(L"Village_Door_Open", TEX_NORMAL,
		L"../Bin/Resource/Image/Village/Interaction/Door/open.png", 1);

	Engine::CreateNewTexture(L"Village_Door_Close", TEX_NORMAL,
		L"../Bin/Resource/Image/Village/Interaction/Door/closed.png", 1);

	shared_ptr<CGameObject> m_pDoor = make_shared<CDoor>(m_pGraphicDev);
	m_pLayer->CreateGameObject(L"Obj_Door", m_pDoor);

	dynamic_pointer_cast<CLayer>(m_pLayer)->AwakeLayer();

	return S_OK;
}

HRESULT CVillage::Ready_Layer_UI(tstring pLayerTag)
{
	return S_OK;
}
