#include "pch.h"
#include "BossMap.h"
#include "Terrain.h"
#include "Wall.h"
#include "SkyBox.h"
#include "Layer.h"
#include "StaticCamera.h"
#include "DynamicCamera.h"
#include "InteractionObj.h"
#include "Player.h"
#include "DungeonSystem.h"
#include "DungeonRoom.h"
#include "BattleSystem.h"

#include"Inventory.h"
#include"PlayerHand.h"
#include"Item.h"

#include"GameMgr.h"
#include"CameraMgr.h"

#include"Village.h"

#include "Highwayman.h"
#include "Jester.h"
#include "Vestal.h"
#include "ShieldBreaker.h"

#include "Boss.h"

#include "Export_System.h"
#include "Export_Utility.h"

#include"ResourceMgr.h"

#include"UIMgr.h"
#include"BattleHeroUI.h"
#include"SoundMgr.h"

#include"Narration.h"

CBossMap::CBossMap(LPDIRECT3DDEVICE9 pGraphicDev)
	:CScene(pGraphicDev)
{
	ZeroMemory(m_szString, sizeof(m_szString));	
}

CBossMap::~CBossMap()
{
}

HRESULT CBossMap::ReadyScene()
{
	CResourceMgr::GetInstance()->BossMapTextureLoad();

	// text
	lstrcpy(m_szString, L"Boss Map");
	// BossMap
	m_pBossMap = make_shared<CDungeonSystem>();
	// 방1
	m_pRoom1 = make_shared<CDungeonRoom>();

	Ready_Layer_Environment(L"Layer_3_Environment");
	Ready_Layer_SkyBox(L"Layer_1_SkyBox");
	Ready_Layer_GameObject(L"Layer_4_GameObj");
	Ready_Layer_UI(L"Layer_2_UI");
	Ready_Layer_Camera(L"Layer_5_Camera");

	for (auto& iter : m_mapLayer) { 
		iter.second->ReadyLayer();
	}

 	return __super::ReadyScene();
}

_int CBossMap::UpdateScene(const _float& fTimeDelta)
{

	int iExit;
	iExit = __super::UpdateScene(fTimeDelta);

	return iExit;
}

void CBossMap::LateUpdateScene()
{
	__super::LateUpdateScene();

}

void CBossMap::RenderScene()
{
}

void CBossMap::SetWall(shared_ptr<CWall> _pWall, shared_ptr<CLayer> _pLayer,
	_float _fX, _float _fZ, _float _fSize, tstring _tKeyName)
{
	for (int i = 0; i < 5; i++)
	{
		switch (i)
		{
		case 0:
			_pWall = make_shared<CWall>(m_pGraphicDev, L"Com_Boss_ObjectWallTexture", 1, true);
			_pWall->SetPos(_vec3(_fX, _fSize, _fZ + _fSize));
			_pWall->SetAngle(_vec3(0.f, PI * 0.5f, 0.f));
			_pWall->SetScale(_vec3(_fSize, _fSize, 1.f));
			dynamic_pointer_cast<CWall>(_pWall)->SetKey2Name(_tKeyName);
			_pLayer->CreateGameObject(L"OBJ_Wall", _pWall);
			m_pVecWallList.push_back(_pWall);
			break;
		case 1:
			_pWall = make_shared<CWall>(m_pGraphicDev, L"Com_Boss_ObjectWallTexture", 1, true);
			_pWall->SetPos(_vec3(_fX, _fSize, _fZ - _fSize));
			_pWall->SetAngle(_vec3(0.f, PI * 0.5f, 0.f));
			_pWall->SetScale(_vec3(_fSize, _fSize, 1.f));
			dynamic_pointer_cast<CWall>(_pWall)->SetKey2Name(_tKeyName);
			_pLayer->CreateGameObject(L"OBJ_Wall", _pWall);
			m_pVecWallList.push_back(_pWall);
			break;
		case 2:
			_pWall = make_shared<CWall>(m_pGraphicDev, L"Com_Boss_ObjectWallTexture", 1, true);
			_pWall->SetPos(_vec3(_fX - _fSize, _fSize, _fZ));
			_pWall->SetAngle(_vec3(0.f, 0.f, 0.f));
			_pWall->SetScale(_vec3(_fSize, _fSize, 1.f));
			dynamic_pointer_cast<CWall>(_pWall)->SetKey2Name(_tKeyName);
			_pLayer->CreateGameObject(L"OBJ_Wall", _pWall);
			m_pVecWallList.push_back(_pWall);
			break;
		case 3:
			_pWall = make_shared<CWall>(m_pGraphicDev, L"Com_Boss_ObjectWallTexture", 1, true);
			_pWall->SetPos(_vec3(_fX + _fSize, _fSize, _fZ));
			_pWall->SetAngle(_vec3(0.f, 0.f, 0.f));
			_pWall->SetScale(_vec3(_fSize, _fSize, 1.f));
			dynamic_pointer_cast<CWall>(_pWall)->SetKey2Name(_tKeyName);
			_pLayer->CreateGameObject(L"OBJ_Wall", _pWall);
			m_pVecWallList.push_back(_pWall);
			break; 

		case 4:
			_pWall = make_shared<CWall>(m_pGraphicDev, L"Com_Boss_ObjectWallTopTexture", 1, true);
			_pWall->SetPos(_vec3(_fX, _fSize * 2.f, _fZ));
			_pWall->SetAngle(_vec3(PI * 0.5f, 0.f, 0.f));
			_pWall->SetScale(_vec3(_fSize, _fSize, 1.f));
			dynamic_pointer_cast<CWall>(_pWall)->SetKey2Name(_tKeyName);
			_pLayer->CreateGameObject(L"OBJ_Wall", _pWall);
			m_pVecWallList.push_back(_pWall);
			break;
		}
	}
}

HRESULT CBossMap::Ready_Layer_Environment(tstring pLayerTag)
{
	shared_ptr<CLayer> m_pLayer = make_shared<CLayer>();
	m_mapLayer.insert({ pLayerTag, m_pLayer });

	shared_ptr<CGameObject> m_pTerrain = make_shared<CTerrain>(m_pGraphicDev, L"Com_Boss_FloorTexture", ETerrainType::DUNGEON2);
	m_pLayer->CreateGameObject(L"OBJ_Terrain", m_pTerrain);

	shared_ptr<CWall> _pWall;


	//벽
	for (int i = 0; i < 20; i++)
	{
		_pWall = make_shared<CWall>(m_pGraphicDev, L"Com_Boss_WallTexture", 6, true);
		_pWall->SetPos(_vec3(0.f, BOSS_WALLSIZEUPY, BOSS_WALLSIZEX + BOSS_WALLSIZEX * 2.f * i));
		_pWall->SetAngle(_vec3(0.f, 0.f, 0.f));
		_pWall->SetScale(_vec3(BOSS_WALLSIZEX, BOSS_WALLSIZEX, 1.f));
		m_pLayer->CreateGameObject(L"OBJ_Wall", _pWall);
	}
	for (int i = 0; i < 20; i++)
	{
		_pWall = make_shared<CWall>(m_pGraphicDev, L"Com_Boss_WallTexture", 6, true);
		_pWall->SetPos(_vec3(640.f, BOSS_WALLSIZEUPY, BOSS_WALLSIZEX + BOSS_WALLSIZEX * 2.f * i));
		_pWall->SetAngle(_vec3(0.f, 0.f, 0.f));
		_pWall->SetScale(_vec3(BOSS_WALLSIZEX, BOSS_WALLSIZEX, 1.f));
		m_pLayer->CreateGameObject(L"OBJ_Wall", _pWall);
	}
	for (int i = 0; i < 20; i++)
	{
		_pWall = make_shared<CWall>(m_pGraphicDev, L"Com_Boss_WallTexture", 6, true);
		_pWall->SetPos(_vec3(BOSS_WALLSIZEX + BOSS_WALLSIZEX * 2.f * i, BOSS_WALLSIZEUPY, 0.f));
		_pWall->SetAngle(_vec3(0.f, PI * 0.5f, 0.f));
		_pWall->SetScale(_vec3(BOSS_WALLSIZEX, BOSS_WALLSIZEX, 1.f));
		m_pLayer->CreateGameObject(L"OBJ_Wall", _pWall);
	}
	for (int i = 0; i < 20; i++)
	{
		_pWall = make_shared<CWall>(m_pGraphicDev, L"Com_Boss_WallTexture", 6, true);
		_pWall->SetPos(_vec3(BOSS_WALLSIZEX + BOSS_WALLSIZEX * 2.f * i, BOSS_WALLSIZEUPY, 640.f));
		_pWall->SetAngle(_vec3(0.f, PI * 0.5f, 0.f));
		_pWall->SetScale(_vec3(BOSS_WALLSIZEX, BOSS_WALLSIZEX, 1.f));
		m_pLayer->CreateGameObject(L"OBJ_Wall", _pWall);
	}

	// 장애물 배치
	SetWall(_pWall, m_pLayer, 100.f, 100.f, 20.f, L"Wall1");
	SetWall(_pWall, m_pLayer, 300.f, 300.f, 20.f, L"Wall2");
	SetWall(_pWall, m_pLayer, 500.f, 500.f, 20.f, L"Wall3");
	SetWall(_pWall, m_pLayer, 100.f, 500.f, 20.f, L"Wall4");
	SetWall(_pWall, m_pLayer, 500.f, 100.f, 20.f, L"Wall5");


	//가장 최하위 순서에 돌려줄 것
	dynamic_pointer_cast<CLayer>(m_pLayer)->AwakeLayer();


	return S_OK;
}

HRESULT CBossMap::Ready_Layer_SkyBox(tstring pLayerTag)
{
	shared_ptr<CLayer> m_pLayer = make_shared<CLayer>();
	m_mapLayer.insert({ pLayerTag, m_pLayer });


	shared_ptr<CGameObject> m_pSkyBox = make_shared<CSkyBox>(m_pGraphicDev, L"Com_BossMap_SkyBoxTexture");
	m_pLayer->CreateGameObject(L"OBJ_BossMap_SkyBox", m_pSkyBox);

	dynamic_pointer_cast<CLayer>(m_pLayer)->AwakeLayer();

	return S_OK;
}

HRESULT CBossMap::Ready_Layer_Camera(tstring pLayerTag)
{
	shared_ptr<CLayer> m_pLayer = make_shared<CLayer>();
	m_mapLayer.insert({ pLayerTag, m_pLayer });

	// Camera
	//shared_ptr<CGameObject> m_pCamera = make_shared<CDynamicCamera>(m_pGraphicDev);
	//m_pLayer->CreateGameObject(L"OBJ_Camera", m_pCamera);

	shared_ptr<CGameObject> m_pCamera = make_shared<CStaticCamera>(m_pGraphicDev);
	m_pLayer->CreateGameObject(L"OBJ_Camera", m_pCamera);
	
	CCameraMgr::GetInstance()->SetMainCamera(dynamic_pointer_cast<CStaticCamera>(m_pCamera));
	CCameraMgr::GetInstance()->SetFPSMode();

	dynamic_pointer_cast<CLayer>(m_pLayer)->AwakeLayer();

	return S_OK;
}

HRESULT CBossMap::Ready_Layer_GameObject(tstring pLayerTag)
{
	shared_ptr<CLayer> m_pLayer = make_shared<CLayer>();
	m_mapLayer.insert({ pLayerTag, m_pLayer });

	// Player
	shared_ptr<CGameObject> m_pPlayer = make_shared<CPlayer>(m_pGraphicDev);
	m_pPlayer->SetPos({ 320.f, 0.f, 200.f });
	m_pLayer->CreateGameObject(L"Obj_Player", m_pPlayer);

	// Boss
	shared_ptr<CGameObject> m_pBoss = make_shared<CBoss>(m_pGraphicDev);
	m_pBoss->SetPos({ 320.f, 35.f, 250.f });

	CGameMgr::GetInstance()->SetPlayer(m_pPlayer);

	dynamic_pointer_cast<CPlayer>(m_pPlayer)->SetInDungeon(true);

	// 방에 GameObject 넣기
	// Room1
	vector<shared_ptr<CGameObject>> Room1_v1;
	Room1_v1.push_back(m_pBoss);
	m_pRoom1->PushGameObjectVector(Room1_v1);


	// 던전에 방 넣기
	vector<shared_ptr<CDungeonRoom>> Dungeon1_v;
	Dungeon1_v.push_back(m_pRoom1);
	m_pBossMap->PushDungeonRoomVector(Dungeon1_v);

	// 현재 active 방
	m_pBossMap->CurrentRoom(1);
	dynamic_pointer_cast<CPlayer>(CGameMgr::GetInstance()->GetPlayer())->SetCurrentRoom(1);

	// Layer에 GameObject 넣기

	//PlayerObj
	shared_ptr<CGameObject> m_pPlayerHand = make_shared<CPlayerHand>(m_pGraphicDev);
	m_pLayer->CreateGameObject(L"Obj_PlayerHand", m_pPlayerHand);
	(dynamic_pointer_cast<CPlayer>(m_pPlayer))->SetPlayerHand(dynamic_pointer_cast<CPlayerHand>(m_pPlayerHand));

	m_pLayer->CreateGameObject(L"Obj_Boss", m_pBoss);
	dynamic_pointer_cast<CLayer>(m_pLayer)->AwakeLayer();

	return S_OK;
}

HRESULT CBossMap::Ready_Layer_UI(tstring pLayerTag)
{
	shared_ptr<CLayer> m_pLayer = make_shared<CLayer>();
	m_mapLayer.insert({ pLayerTag, m_pLayer });

	shared_ptr<CGameObject> m_pInventory = make_shared<CInventory>(m_pGraphicDev);
	m_pLayer->CreateGameObject(L"Obj_UI", m_pInventory);

	CUIMgr::GetInstance()->AddUIObject(L"UI_Inventory", dynamic_pointer_cast<CUIObj>(m_pInventory));

	dynamic_pointer_cast<CPlayer>(CGameMgr::GetInstance()->GetPlayer())->SetInventory(dynamic_pointer_cast<CInventory>(m_pInventory));
	
	dynamic_pointer_cast<CLayer>(m_pLayer)->AwakeLayer();

	return S_OK;
}
