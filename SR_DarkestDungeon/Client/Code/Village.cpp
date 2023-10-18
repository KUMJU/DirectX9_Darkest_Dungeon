#include"pch.h"
#include "Village.h"
#include"Terrain.h"
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
	Ready_Layer_Environment(L"Environment");

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
	m_pLayer->CreateGameObject(L"MainCamera", m_pCamera);

	//Terrain
	Engine::CreateNewTexture(L"Weald_Floor_Texture", TEX_NORMAL,
		L"../Bin/Resource/Image/Dungeons/BackGround/Weald/forest_road_roots_bottom_0.png", 1);

	shared_ptr<CGameObject> m_pTerrain = make_shared<CTerrain>(m_pGraphicDev);
	m_pLayer->CreateGameObject(L"Terrain", m_pTerrain);

	//InteractionObj
	Engine::CreateNewTexture(L"Weald_Floor_Texture", TEX_NORMAL,
		L"../Bin/Resource/Image/Village/Inside/sanitarium/sanitarium.treatment.hero_slot_overlay.png", 1);

	shared_ptr<CGameObject> m_pInteractionObj = make_shared<CInteractionObj>(m_pGraphicDev);
	m_pLayer->CreateGameObject(L"InteractionObj", m_pInteractionObj);


	//가장 최하위 순서에 돌려줄 것
	m_pLayer->ReadyLayer();


	return S_OK;
}

HRESULT CVillage::Ready_Layer_SkyBox(const tstring* pLayerTag)
{
	return E_NOTIMPL;
}

HRESULT CVillage::Ready_Layer_GameObject(const tstring* pLayerTag)
{
	return E_NOTIMPL;
}

HRESULT CVillage::Ready_Layer_UI(const tstring* pLayerTag)
{
	return E_NOTIMPL;
}
