#include"pch.h"
#include "MainLogo.h"
#include"Terrain.h"
#include"Layer.h"
#include"DynamicCamera.h"
#include"Export_Utility.h"

CMainLogo::CMainLogo(LPDIRECT3DDEVICE9 pGraphicDev)
	:CScene(pGraphicDev)
{
}

CMainLogo::~CMainLogo()
{
}

HRESULT CMainLogo::ReadyScene()
{
	Ready_Layer_Environment(L"Environment");

	return __super::ReadyScene();
}

_int CMainLogo::UpdateScene(const _float& fTimeDelta)
{
	int iExit;
	iExit = __super::UpdateScene(fTimeDelta);

	return iExit;
}

void CMainLogo::LateUpdateScene()
{
	__super::LateUpdateScene();
	
}

void CMainLogo::RenderScene()
{
}

HRESULT CMainLogo::Ready_Layer_Environment(tstring pLayerTag)
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


	//가장 최하위 순서에 돌려줄 것
	m_pLayer->ReadyLayer();


	return S_OK;
}

HRESULT CMainLogo::Ready_Layer_SkyBox(const tstring* pLayerTag)
{
	return E_NOTIMPL;
}

HRESULT CMainLogo::Ready_Layer_GameObject(const tstring* pLayerTag)
{
	return E_NOTIMPL;
}

HRESULT CMainLogo::Ready_Layer_UI(const tstring* pLayerTag)
{
	return E_NOTIMPL;
}
