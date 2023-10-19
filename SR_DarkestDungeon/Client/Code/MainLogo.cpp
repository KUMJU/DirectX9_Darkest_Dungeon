#include"pch.h"
#include "MainLogo.h"
#include"Terrain.h"
#include "Wall.h"
#include"SkyBox.h"
#include"Layer.h"
#include"DynamicCamera.h"
#include"Player.h"
#include"Export_Utility.h"
#include"StaticCamera.h"

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
	Ready_Layer_GameObject(L"Layer_GameObj");

	for (auto& iter : m_mapLayer) {
		//GameComponenet Setting
		iter.second->ReadyLayer();
	}

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
		L"../Bin/Resource/Image/Dungeons/BackGround/Weald/weald.corridor_wall.%d.png", 9);
	Engine::CreateNewTexture(L"Com_Weald_BackWallTexture", TEX_NORMAL,
		L"../Bin/Resource/Image/Dungeons/BackGround/Weald/weald.corridor_mid.png", 1);

	shared_ptr<CGameObject> m_pWall;
	// 진입방 벽
	for (int i = 0; i < 2; i++)
	{
		m_pWall = make_shared<CWall>(m_pGraphicDev);
		m_pWall->SetPos(_vec3(WALLSIZEX * 2.f, 2.5f, WALLSIZEX / 2.f + WALLSIZEX * i));
		m_pWall->SetAngle(_vec3(0.f, 0.f, 0.f));
		m_pLayer->CreateGameObject(L"OBJ_Wall", m_pWall);
	}

	for (int i = 0; i < 2; i++)
	{
		m_pWall = make_shared<CWall>(m_pGraphicDev);
		m_pWall->SetPos(_vec3(WALLSIZEX * 2.f + PATHSIZEX + WALLSIZEX * 2.f, 2.5f, WALLSIZEX / 2.f + WALLSIZEX * i));
		m_pWall->SetAngle(_vec3(0.f, 0.f, 0.f));
		m_pLayer->CreateGameObject(L"OBJ_Wall", m_pWall);
	}

	for (int i = 0; i < 1; i++)
	{
		m_pWall = make_shared<CWall>(m_pGraphicDev);
		m_pWall->SetPos(_vec3(WALLSIZEX * 2.f + WALLSIZEX / 2.f + WALLSIZEX * i, 2.5f, WALLSIZEX * 2.f));
		m_pWall->SetAngle(_vec3(0.f, PI / 2.f, 0.f));
		m_pLayer->CreateGameObject(L"OBJ_Wall", m_pWall);
	}

	for (int i = 0; i < 1; i++)
	{
		m_pWall = make_shared<CWall>(m_pGraphicDev);
		m_pWall->SetPos(_vec3(WALLSIZEX * 2.f + PATHSIZEX + WALLSIZEX * 1.5f + WALLSIZEX * i, 2.5f, WALLSIZEX * 2.f));
		m_pWall->SetAngle(_vec3(0.f, PI / 2.f, 0.f));
		m_pLayer->CreateGameObject(L"OBJ_Wall", m_pWall);
	}

	// 통로 1 벽
	for (int i = 0; i < 10; i++)
	{
		m_pWall = make_shared<CWall>(m_pGraphicDev);
		m_pWall->SetPos(_vec3(WALLSIZEX * 2.f + WALLSIZEX * 1.f, 2.5f, WALLSIZEX * 2.f + WALLSIZEX / 2.f + WALLSIZEX * i));
		m_pWall->SetAngle(_vec3(0.f, 0.f, 0.f));
		m_pLayer->CreateGameObject(L"OBJ_Wall", m_pWall);
	}

	for (int i = 0; i < 10; i++)
	{
		m_pWall = make_shared<CWall>(m_pGraphicDev);
		m_pWall->SetPos(_vec3(WALLSIZEX * 2.f + PATHSIZEX + WALLSIZEX * 1.f, 2.5f, WALLSIZEX * 2.f + WALLSIZEX / 2.f + WALLSIZEX * i));
		m_pWall->SetAngle(_vec3(0.f, 0.f, 0.f));
		m_pLayer->CreateGameObject(L"OBJ_Wall", m_pWall);
	}

	// 방 2 벽
	for (int i = 0; i < 4; i++)
	{
		m_pWall = make_shared<CWall>(m_pGraphicDev);
		m_pWall->SetPos(_vec3(WALLSIZEX * 1.f, 2.5f, WALLSIZEX / 2.f + WALLSIZEX * 12.f + WALLSIZEX * i));
		m_pWall->SetAngle(_vec3(0.f, 0.f, 0.f));
		m_pLayer->CreateGameObject(L"OBJ_Wall", m_pWall);
	}

	for (int i = 0; i < 4; i++)
	{
		m_pWall = make_shared<CWall>(m_pGraphicDev);
		m_pWall->SetPos(_vec3(WALLSIZEX * 3.f + PATHSIZEX + WALLSIZEX * 2.f, 2.5f, WALLSIZEX / 2.f + WALLSIZEX * 12.f + WALLSIZEX * i));
		m_pWall->SetAngle(_vec3(0.f, 0.f, 0.f));
		m_pLayer->CreateGameObject(L"OBJ_Wall", m_pWall);
	}

	for (int i = 0; i < 2; i++)
	{
		m_pWall = make_shared<CWall>(m_pGraphicDev);
		m_pWall->SetPos(_vec3(WALLSIZEX * 1.5f + WALLSIZEX * i, 2.5f, WALLSIZEX * 12.f));
		m_pWall->SetAngle(_vec3(0.f, PI / 2.f, 0.f));
		m_pLayer->CreateGameObject(L"OBJ_Wall", m_pWall);
	}

	for (int i = 0; i < 2; i++)
	{
		m_pWall = make_shared<CWall>(m_pGraphicDev);
		m_pWall->SetPos(_vec3(WALLSIZEX * 3.5f + PATHSIZEX + WALLSIZEX * i, 2.5f, WALLSIZEX * 12.f));
		m_pWall->SetAngle(_vec3(0.f, PI / 2.f, 0.f));
		m_pLayer->CreateGameObject(L"OBJ_Wall", m_pWall);
	}

	for (int i = 0; i < 6; i++)
	{
		m_pWall = make_shared<CWall>(m_pGraphicDev);
		m_pWall->SetPos(_vec3(WALLSIZEX * 1.5f + WALLSIZEX * i, 2.5f, WALLSIZEX * 16.f));
		m_pWall->SetAngle(_vec3(0.f, PI / 2.f, 0.f));
		m_pLayer->CreateGameObject(L"OBJ_Wall", m_pWall);
	}


	//가장 최하위 순서에 돌려줄 것
	dynamic_pointer_cast<CLayer>(m_pLayer)->AwakeLayer();


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

	dynamic_pointer_cast<CLayer>(m_pLayer)->AwakeLayer();

	return E_NOTIMPL;
}

HRESULT CMainLogo::Ready_Layer_GameObject(tstring pLayerTag)
{
	shared_ptr<CLayer> m_pLayer = make_shared<CLayer>();
	m_mapLayer.insert({ pLayerTag, m_pLayer });

	shared_ptr<CGameObject> m_pPlayer = make_shared<CPlayer>(m_pGraphicDev);
	m_pLayer->CreateGameObject(L"Obj_Player", m_pPlayer);

	dynamic_pointer_cast<CLayer>(m_pLayer)->AwakeLayer();

	return E_NOTIMPL;
}

HRESULT CMainLogo::Ready_Layer_UI(const tstring* pLayerTag)
{
	return E_NOTIMPL;
}
