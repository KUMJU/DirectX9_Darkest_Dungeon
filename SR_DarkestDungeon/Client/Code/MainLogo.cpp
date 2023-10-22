#include "pch.h"
#include "MainLogo.h"
#include "Terrain.h"
#include "Wall.h"
#include "SkyBox.h"
#include "Layer.h"
#include "DynamicCamera.h"
#include "Player.h"
#include "Export_Utility.h"
#include "StaticCamera.h"
#include "DungeonSystem.h"
#include "DungeonRoom.h"

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
	// Weald 던전
	m_pWealdDungeon = make_shared<CDungeonSystem>();
	// 던전 방1
	m_pRoom1 = make_shared<CDungeonRoom>();
	// 던전 방2
	m_pRoom2 = make_shared<CDungeonRoom>();

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
	// 방 1에 들어가면
	if (GetAsyncKeyState('8') & 0x8000) {
		m_pWealdDungeon->CurrentRoom(0);
		m_pRoom1->BattleStart();
	}

	// 방 2에 들어가면
	if (GetAsyncKeyState('9') & 0x8000) {
		m_pWealdDungeon->CurrentRoom(1);
		//m_pRoom2->BattleStart();
	}


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

	// GameObject
	shared_ptr<CGameObject> m_pBrigandCutthroat_1 = make_shared<CBrigandCutthroat>(m_pGraphicDev);
	shared_ptr<CGameObject> m_pBrigandCutthroat_2 = make_shared<CBrigandCutthroat>(m_pGraphicDev);
	shared_ptr<CGameObject> m_pBrigandCutthroat_3 = make_shared<CBrigandCutthroat>(m_pGraphicDev);
	shared_ptr<CGameObject> m_pBrigandCutthroat_4 = make_shared<CBrigandCutthroat>(m_pGraphicDev);
	shared_ptr<CGameObject> m_pBrigandCutthroat_5 = make_shared<CBrigandCutthroat>(m_pGraphicDev);
	shared_ptr<CGameObject> m_pBrigandCutthroat_6 = make_shared<CBrigandCutthroat>(m_pGraphicDev);
	shared_ptr<CGameObject> m_pBrigandCutthroat_7 = make_shared<CBrigandCutthroat>(m_pGraphicDev);
	shared_ptr<CGameObject> m_pBrigandCutthroat_8 = make_shared<CBrigandCutthroat>(m_pGraphicDev);
	

	// 방에 GameObject 넣기
	vector<shared_ptr<CGameObject>> vect1;
	vect1.push_back(m_pBrigandCutthroat_1);
	vect1.push_back(m_pBrigandCutthroat_2);
	vect1.push_back(m_pBrigandCutthroat_3);
	vect1.push_back(m_pBrigandCutthroat_4);
	vect1.push_back(m_pBrigandCutthroat_5);
	vect1.push_back(m_pBrigandCutthroat_6);
	vect1.push_back(m_pBrigandCutthroat_7);
	vect1.push_back(m_pBrigandCutthroat_8);

	m_pRoom1->PushGameObjectVector(vect1);

	vector<shared_ptr<CGameObject>> vect2;
	vect2.push_back(m_pBrigandCutthroat_1);
	vect2.push_back(m_pBrigandCutthroat_2);
	vect2.push_back(m_pBrigandCutthroat_3);
	vect2.push_back(m_pBrigandCutthroat_4);

	m_pRoom1->PushHeroesVector(vect2);

	vector<shared_ptr<CGameObject>> vect3;
	vect3.push_back(m_pBrigandCutthroat_5);
	vect3.push_back(m_pBrigandCutthroat_6);
	vect3.push_back(m_pBrigandCutthroat_7);
	vect3.push_back(m_pBrigandCutthroat_8);

	m_pRoom1->PushMonstersVector(vect3);

	// 던전에 방 넣기
	vector<shared_ptr<CDungeonRoom>> vect4;
	vect4.push_back(m_pRoom1);
	vect4.push_back(m_pRoom2);

	m_pWealdDungeon->PushDungeonRoomVector(vect4);


	m_pWealdDungeon->CurrentRoom(0);
	m_pRoom1->BattleStart();
	
	// Layer에 GameObject 넣기
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
