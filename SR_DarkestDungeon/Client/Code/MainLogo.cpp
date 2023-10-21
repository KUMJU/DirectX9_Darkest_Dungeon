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

#include "BrigandCutthroat.h"

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

void CMainLogo::FormBattlePosition(vector<shared_ptr<CGameObject>> _pHeroes, vector<shared_ptr<CGameObject>> _pMonsters, _float _fAngle, _vec3 _vOrigin)
{
	// 좌측
	// 3열
	dynamic_pointer_cast<CCreature>(_pHeroes[2])->SetPos(_vec3(_vOrigin.x - 4.f, 3.f, _vOrigin.y + 8.f));
	dynamic_pointer_cast<CCreature>(_pHeroes[2])->SetAngle(_vec3(0.f, _fAngle, 0.f));

	// 4열
	dynamic_pointer_cast<CCreature>(_pHeroes[3])->SetPos(_vec3(_vOrigin.x - 6.f, 3.f, _vOrigin.y + 6.f));
	dynamic_pointer_cast<CCreature>(_pHeroes[3])->SetAngle(_vec3(0.f, _fAngle, 0.f));

	// 1열
	dynamic_pointer_cast<CCreature>(_pHeroes[0])->SetPos(_vec3(_vOrigin.x + 4.f, 3.f, _vOrigin.y + 4.f));
	dynamic_pointer_cast<CCreature>(_pHeroes[0])->SetAngle(_vec3(0.f, _fAngle, 0.f));

	// 2열
	dynamic_pointer_cast<CCreature>(_pHeroes[1])->SetPos(_vec3(_vOrigin.x + 2.f, 3.f, _vOrigin.y + 2.f));
	dynamic_pointer_cast<CCreature>(_pHeroes[1])->SetAngle(_vec3(0.f, _fAngle, 0.f));

	// 우측
	// 3열
	dynamic_pointer_cast<CCreature>(_pMonsters[2])->SetPos(_vec3(_vOrigin.x + 24.f, 3.f, _vOrigin.y + 8.f));
	dynamic_pointer_cast<CCreature>(_pMonsters[2])->SetAngle(_vec3(0.f, _fAngle + PI, 0.f));

	// 4열
	dynamic_pointer_cast<CCreature>(_pMonsters[3])->SetPos(_vec3(_vOrigin.x + 26.f, 3.f, _vOrigin.y + 6.f));
	dynamic_pointer_cast<CCreature>(_pMonsters[3])->SetAngle(_vec3(0.f, _fAngle + PI, 0.f));

	// 1열
	dynamic_pointer_cast<CCreature>(_pMonsters[0])->SetPos(_vec3(_vOrigin.x + 16.f, 3.f, _vOrigin.y + 4.f));
	dynamic_pointer_cast<CCreature>(_pMonsters[0])->SetAngle(_vec3(0.f, _fAngle + PI, 0.f));

	// 2열
	dynamic_pointer_cast<CCreature>(_pMonsters[1])->SetPos(_vec3(_vOrigin.x + 18.f, 3.f, _vOrigin.y + 2.f));
	dynamic_pointer_cast<CCreature>(_pMonsters[1])->SetAngle(_vec3(0.f, _fAngle + PI, 0.f));
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

	shared_ptr<CGameObject> m_pTerrain = make_shared<CTerrain>(m_pGraphicDev, L"Com_Weald_FloorTexture");
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
		m_pWall->SetPos(_vec3(WALLSIZEX * 2.f, WALLSIZEUPY, WALLSIZEX / 2.f + WALLSIZEX * i));
		m_pWall->SetAngle(_vec3(0.f, 0.f, 0.f));
		m_pLayer->CreateGameObject(L"OBJ_Wall", m_pWall);
	}

	for (int i = 0; i < 2; i++)
	{
		m_pWall = make_shared<CWall>(m_pGraphicDev);
		m_pWall->SetPos(_vec3(WALLSIZEX * 2.f + PATHSIZEX + WALLSIZEX * 2.f, WALLSIZEUPY, WALLSIZEX / 2.f + WALLSIZEX * i));
		m_pWall->SetAngle(_vec3(0.f, 0.f, 0.f));
		m_pLayer->CreateGameObject(L"OBJ_Wall", m_pWall);
	}

	for (int i = 0; i < 1; i++)
	{
		m_pWall = make_shared<CWall>(m_pGraphicDev);
		m_pWall->SetPos(_vec3(WALLSIZEX * 2.f + WALLSIZEX / 2.f + WALLSIZEX * i, WALLSIZEUPY, WALLSIZEX * 2.f));
		m_pWall->SetAngle(_vec3(0.f, PI / 2.f, 0.f));
		m_pLayer->CreateGameObject(L"OBJ_Wall", m_pWall);
	}

	for (int i = 0; i < 1; i++)
	{
		m_pWall = make_shared<CWall>(m_pGraphicDev);
		m_pWall->SetPos(_vec3(WALLSIZEX * 2.f + PATHSIZEX + WALLSIZEX * 1.5f + WALLSIZEX * i, WALLSIZEUPY, WALLSIZEX * 2.f));
		m_pWall->SetAngle(_vec3(0.f, PI / 2.f, 0.f));
		m_pLayer->CreateGameObject(L"OBJ_Wall", m_pWall);
	}

	// 통로 1 벽
	for (int i = 0; i < 10; i++)
	{
		m_pWall = make_shared<CWall>(m_pGraphicDev);
		m_pWall->SetPos(_vec3(WALLSIZEX * 2.f + WALLSIZEX * 1.f, WALLSIZEUPY, WALLSIZEX * 2.f + WALLSIZEX / 2.f + WALLSIZEX * i));
		m_pWall->SetAngle(_vec3(0.f, 0.f, 0.f));
		m_pLayer->CreateGameObject(L"OBJ_Wall", m_pWall);
	}

	for (int i = 0; i < 10; i++)
	{
		m_pWall = make_shared<CWall>(m_pGraphicDev);
		m_pWall->SetPos(_vec3(WALLSIZEX * 2.f + PATHSIZEX + WALLSIZEX * 1.f, WALLSIZEUPY, WALLSIZEX * 2.f + WALLSIZEX / 2.f + WALLSIZEX * i));
		m_pWall->SetAngle(_vec3(0.f, 0.f, 0.f));
		m_pLayer->CreateGameObject(L"OBJ_Wall", m_pWall);
	}

	// 방 2 벽
	for (int i = 0; i < 4; i++)
	{
		m_pWall = make_shared<CWall>(m_pGraphicDev);
		m_pWall->SetPos(_vec3(WALLSIZEX * 1.f, WALLSIZEUPY, WALLSIZEX / 2.f + WALLSIZEX * 12.f + WALLSIZEX * i));
		m_pWall->SetAngle(_vec3(0.f, 0.f, 0.f));
		m_pLayer->CreateGameObject(L"OBJ_Wall", m_pWall);
	}

	for (int i = 0; i < 4; i++)
	{
		m_pWall = make_shared<CWall>(m_pGraphicDev);
		m_pWall->SetPos(_vec3(WALLSIZEX * 3.f + PATHSIZEX + WALLSIZEX * 2.f, WALLSIZEUPY, WALLSIZEX / 2.f + WALLSIZEX * 12.f + WALLSIZEX * i));
		m_pWall->SetAngle(_vec3(0.f, 0.f, 0.f));
		m_pLayer->CreateGameObject(L"OBJ_Wall", m_pWall);
	}

	for (int i = 0; i < 2; i++)
	{
		m_pWall = make_shared<CWall>(m_pGraphicDev);
		m_pWall->SetPos(_vec3(WALLSIZEX * 1.5f + WALLSIZEX * i, WALLSIZEUPY, WALLSIZEX * 12.f));
		m_pWall->SetAngle(_vec3(0.f, PI / 2.f, 0.f));
		m_pLayer->CreateGameObject(L"OBJ_Wall", m_pWall);
	}

	for (int i = 0; i < 2; i++)
	{
		m_pWall = make_shared<CWall>(m_pGraphicDev);
		m_pWall->SetPos(_vec3(WALLSIZEX * 3.5f + PATHSIZEX + WALLSIZEX * i, WALLSIZEUPY, WALLSIZEX * 12.f));
		m_pWall->SetAngle(_vec3(0.f, PI / 2.f, 0.f));
		m_pLayer->CreateGameObject(L"OBJ_Wall", m_pWall);
	}

	for (int i = 0; i < 6; i++)
	{
		m_pWall = make_shared<CWall>(m_pGraphicDev);
		m_pWall->SetPos(_vec3(WALLSIZEX * 1.5f + WALLSIZEX * i, WALLSIZEUPY, WALLSIZEX * 16.f));
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

	shared_ptr<CGameObject> m_pSkyBox = make_shared<CSkyBox>(m_pGraphicDev, L"Com_Weald_SkyBoxTexture");
	m_pLayer->CreateGameObject(L"OBJ_Weald_SkyBox", m_pSkyBox);

	dynamic_pointer_cast<CLayer>(m_pLayer)->AwakeLayer();

	return E_NOTIMPL;
}

HRESULT CMainLogo::Ready_Layer_GameObject(tstring pLayerTag)
{
	shared_ptr<CLayer> m_pLayer = make_shared<CLayer>();
	m_mapLayer.insert({ pLayerTag, m_pLayer });

	//shared_ptr<CGameObject> m_pPlayer = make_shared<CPlayer>(m_pGraphicDev);
	//m_pPlayer->SetPos(_vec3(WALLSIZEX + PATHSIZEX, 3.f, WALLSIZEX * 1.f));
	//m_pPlayer->SetAngle(_vec3(0.f, -PI / 2.f, 0.f));
	//m_pLayer->CreateGameObject(L"Obj_Player", m_pPlayer);

	// 좌측
	shared_ptr<CGameObject> m_pBrigandCutthroat_1 = make_shared<CBrigandCutthroat>(m_pGraphicDev);
	shared_ptr<CGameObject> m_pBrigandCutthroat_2 = make_shared<CBrigandCutthroat>(m_pGraphicDev);
	shared_ptr<CGameObject> m_pBrigandCutthroat_3 = make_shared<CBrigandCutthroat>(m_pGraphicDev);
	shared_ptr<CGameObject> m_pBrigandCutthroat_4 = make_shared<CBrigandCutthroat>(m_pGraphicDev);
	shared_ptr<CGameObject> m_pBrigandCutthroat_5 = make_shared<CBrigandCutthroat>(m_pGraphicDev);
	shared_ptr<CGameObject> m_pBrigandCutthroat_6 = make_shared<CBrigandCutthroat>(m_pGraphicDev);
	shared_ptr<CGameObject> m_pBrigandCutthroat_7 = make_shared<CBrigandCutthroat>(m_pGraphicDev);
	shared_ptr<CGameObject> m_pBrigandCutthroat_8 = make_shared<CBrigandCutthroat>(m_pGraphicDev);

	vector<shared_ptr<CGameObject>> vHeroes;
	vector<shared_ptr<CGameObject>> vMonsters;
	vHeroes.push_back(m_pBrigandCutthroat_1);
	vHeroes.push_back(m_pBrigandCutthroat_2);
	vHeroes.push_back(m_pBrigandCutthroat_3);
	vHeroes.push_back(m_pBrigandCutthroat_4);
	vMonsters.push_back(m_pBrigandCutthroat_5);
	vMonsters.push_back(m_pBrigandCutthroat_6);
	vMonsters.push_back(m_pBrigandCutthroat_7);
	vMonsters.push_back(m_pBrigandCutthroat_8);

	FormBattlePosition(vHeroes, vMonsters, -PI / 2.f, _vec3(WALLSIZEX + PATHSIZEX, WALLSIZEX * 14.f, 0.f));
	
	// z축 기준 뒤에거부터
	m_pLayer->CreateGameObject(L"Obj_BrigandCutthroat", m_pBrigandCutthroat_3);
	m_pLayer->CreateGameObject(L"Obj_BrigandCutthroat", m_pBrigandCutthroat_4);
	m_pLayer->CreateGameObject(L"Obj_BrigandCutthroat", m_pBrigandCutthroat_1);
	m_pLayer->CreateGameObject(L"Obj_BrigandCutthroat", m_pBrigandCutthroat_2);
	m_pLayer->CreateGameObject(L"Obj_BrigandCutthroat", m_pBrigandCutthroat_7);
	m_pLayer->CreateGameObject(L"Obj_BrigandCutthroat", m_pBrigandCutthroat_8);
	m_pLayer->CreateGameObject(L"Obj_BrigandCutthroat", m_pBrigandCutthroat_5);
	m_pLayer->CreateGameObject(L"Obj_BrigandCutthroat", m_pBrigandCutthroat_6);

	dynamic_pointer_cast<CLayer>(m_pLayer)->AwakeLayer();

	return E_NOTIMPL;
}

HRESULT CMainLogo::Ready_Layer_UI(const tstring* pLayerTag)
{
	return E_NOTIMPL;
}
