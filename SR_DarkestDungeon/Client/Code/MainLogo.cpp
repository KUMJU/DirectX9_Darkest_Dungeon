#include"pch.h"
#include "MainLogo.h"
#include"Terrain.h"
#include "Wall.h"
#include"SkyBox.h"
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
	Ready_Layer_Environment(L"Layer_Environment");
	Ready_Layer_SkyBox(L"Layer_SkyBox");

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

	// Camera
	shared_ptr<CGameObject> m_pCamera = make_shared<CDynamicCamera>(m_pGraphicDev);
	m_pLayer->CreateGameObject(L"OBJ_Camera", m_pCamera);
	
	// Terrain
	Engine::CreateNewTexture(L"Com_Weald_FloorTexture", TEX_NORMAL,
		L"../Bin/Resource/Image/Dungeons/BackGround/Weald/forest_Tiles.png", 1);

	shared_ptr<CGameObject> m_pTerrain = make_shared<CTerrain>(m_pGraphicDev);
	m_pLayer->CreateGameObject(L"OBJ_Terrain", m_pTerrain);

	// Wall
	Engine::CreateNewTexture(L"Com_Weald_WallTexture", TEX_NORMAL,
		L"../Bin/Resource/Image/Dungeons/BackGround/Weald/weald.corridor_wall.0%d.png", 9);
	Engine::CreateNewTexture(L"Com_Weald_BackWallTexture", TEX_NORMAL,
		L"../Bin/Resource/Image/Dungeons/BackGround/Weald/weald.corridor_mid.png", 1);

	shared_ptr<CGameObject> m_pWall;
	for (int i = 0; i < 16; i++)
	{
		m_pWall = make_shared<CWall>(m_pGraphicDev);
		m_pWall->SetPos(_vec3(0.f, 2.5f, 4.f + 8.f*i));
		m_pWall->SetAngle(_vec3(0.f, 0.f, 0.f));
		m_pLayer->CreateGameObject(L"OBJ_Wall", m_pWall);
	}

	for (int i = 0; i < 4; i++)
	{
		m_pWall = make_shared<CWall>(m_pGraphicDev);
		m_pWall->SetPos(_vec3(12.f, 2.5f, 4.f + 8.f * i));
		m_pWall->SetAngle(_vec3(0.f, 0.f, 0.f));
		m_pLayer->CreateGameObject(L"OBJ_Wall", m_pWall);
	}

	for (int i = 0; i < 10; i++)
	{
		m_pWall = make_shared<CWall>(m_pGraphicDev);
		m_pWall->SetPos(_vec3(12.f, 2.5f, 48.f + 8.f * i));
		m_pWall->SetAngle(_vec3(0.f, 0.f, 0.f));
		m_pLayer->CreateGameObject(L"OBJ_Wall", m_pWall);
	}

	for (int i = 0; i < 10; i++)
	{
		m_pWall = make_shared<CWall>(m_pGraphicDev);
		m_pWall->SetPos(_vec3(16.f + 8.f * i, 2.5f, 44.f));
		m_pWall->SetAngle(_vec3(0.f, PI/2.f, 0.f));
		m_pLayer->CreateGameObject(L"OBJ_Wall", m_pWall);
	}

	for (int i = 0; i < 10; i++)
	{
		m_pWall = make_shared<CWall>(m_pGraphicDev);
		m_pWall->SetPos(_vec3(16.f + 8.f * i, 2.5f, 32.f));
		m_pWall->SetAngle(_vec3(0.f, PI / 2.f, 0.f));
		m_pLayer->CreateGameObject(L"OBJ_Wall", m_pWall);
	}

	//가장 최하위 순서에 돌려줄 것
	m_pLayer->ReadyLayer();
	

	return S_OK;
}

HRESULT CMainLogo::Ready_Layer_SkyBox(tstring pLayerTag)
{
	shared_ptr<CLayer> m_pLayer = make_shared<CLayer>();
	m_mapLayer.insert({ pLayerTag, m_pLayer });

	// SkyBox
	Engine::CreateNewTexture(L"Com_Weald_SkyBoxTexture", TEX_CUBE,
		L"../Bin/Resource/Image/SkyBox/WealdSkyBox.dds", 1);

	shared_ptr<CGameObject> m_pSkyBox = make_shared<CSkyBox>(m_pGraphicDev);
	m_pLayer->CreateGameObject(L"OBJ_Weald_SkyBox", m_pSkyBox);

	//가장 최하위 순서에 돌려줄 것
	m_pLayer->ReadyLayer();

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
