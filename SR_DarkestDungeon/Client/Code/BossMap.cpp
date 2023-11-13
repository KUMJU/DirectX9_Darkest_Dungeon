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
#include "Bullet1.h"
#include "Bullet2.h"
#include "Laser.h"

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
	m_pRoom2 = make_shared<CDungeonRoom>();

	Ready_Layer_Environment(L"Layer_3_Environment");
	Ready_Layer_SkyBox(L"Layer_1_SkyBox");
	Ready_Layer_GameObject(L"Layer_4_GameObj");
	Ready_Layer_UI(L"Layer_2_UI");
	CUIMgr::GetInstance()->SceneUIInitialize();
	Ready_Layer_Camera(L"Layer_5_Camera");

	for (auto& iter : m_mapLayer) { 
		iter.second->ReadyLayer();
	}

	dynamic_pointer_cast<CPlayer>(CGameMgr::GetInstance()->GetPlayer())->SetPlayerMode(EPlayerMode::BOSS_FIELD);

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
	shared_ptr<CGameObject> m_pPlayer;

	m_pPlayer = dynamic_pointer_cast<CPlayer>(CGameMgr::GetInstance()->GetPlayer());
	m_pPlayer->SetPos({ 320.f, 0.f, 200.f });
	m_pLayer->CreateGameObject(L"Obj_Player", m_pPlayer);

	dynamic_pointer_cast<CPlayer>(m_pPlayer)->SetInDungeon(true);
	dynamic_pointer_cast<CTransform>(m_pPlayer->GetComponent(L"Com_Transform", ID_DYNAMIC))->SetPosition(320.f, 0.f, 200.f);
	dynamic_pointer_cast<CTransform>(m_pPlayer->GetComponent(L"Com_Transform", ID_DYNAMIC))->SetAngle({ 0.f, 0.f, 0.f });

	m_pLayer->CreateGameObject(L"Obj_Player", m_pPlayer);
	m_pLayer->CreateGameObject(L"Obj_PlayerHand", dynamic_pointer_cast<CPlayer>(CGameMgr::GetInstance()->GetPlayer())->GetPlayerHand());
	dynamic_pointer_cast<CPlayer>(m_pPlayer)->SetInDungeon(true);

	// Boss
	shared_ptr<CGameObject> m_pBoss = make_shared<CBoss>(m_pGraphicDev);
	m_pBoss->SetPos({ 320.f, 35.f, 250.f });
	m_pLayer->CreateGameObject(L"Obj_Boss", m_pBoss);

	// BossBullet
	vector<shared_ptr<CBullet1>> pVecProjectile1;
	vector<shared_ptr<CBullet2>> pVecProjectile2;
	vector<shared_ptr<CLaser>> pVecLaser;
	shared_ptr<CBullet1> pBullet1;
	shared_ptr<CBullet2> pBullet2;
	shared_ptr<CLaser> pLaser;
	for (int i = 0; i < 50; i++)
	{
		pBullet1 = nullptr;
		pBullet1 = make_shared<CBullet1>(m_pGraphicDev);
		pBullet1->SetPos({ -100.f, -100.f, -100.f });
		pVecProjectile1.push_back(pBullet1);
		m_pLayer->CreateGameObject(L"Obj_Boss_P1_Bullet1", pVecProjectile1[i]);
	}
	for (int i = 0; i < 50; i++)
	{
		pBullet2 = nullptr;
		pBullet2 = make_shared<CBullet2>(m_pGraphicDev);
		pBullet2->SetPos({ -100.f, -200.f, -100.f });
		pVecProjectile2.push_back(pBullet2);
		m_pLayer->CreateGameObject(L"Obj_Boss_P1_Bullet2", pVecProjectile2[i]);
	}
	for (int i = 0; i < 3; i++)
	{
		pLaser = nullptr;
		pLaser = make_shared<CLaser>(m_pGraphicDev);
		pLaser->SetPos({ -100.f, -200.f, -100.f });
		pVecLaser.push_back(pLaser);
		m_pLayer->CreateGameObject(L"Obj_Boss_P1_Bullet2", pVecLaser[i]);
	}
	dynamic_pointer_cast<CBoss>(m_pBoss)->SetBullet1(pVecProjectile1);
	dynamic_pointer_cast<CBoss>(m_pBoss)->SetBullet2(pVecProjectile2);
	dynamic_pointer_cast<CBoss>(m_pBoss)->SetLaser(pVecLaser);

	// 방에 GameObject 넣기
	// Room1
	vector<shared_ptr<CGameObject>> Room1_v1;
	Room1_v1.push_back(m_pBoss);
	for (int i = 0; i < 50; i++)
	{
		Room1_v1.push_back(pVecProjectile1[i]);
	}
	for (int i = 0; i < 50; i++)
	{
		Room1_v1.push_back(pVecProjectile2[i]);
	}
	m_pRoom1->PushGameObjectVector(Room1_v1);

	// Room2
	vector<shared_ptr<CGameObject>> Room2_v1;
	/*for (int i = 0; i < 30; i++)
	{
		Room2_v1.push_back(m_pBoss);
	}*/
	m_pRoom2->PushGameObjectVector(Room2_v1);

	// 던전에 방 넣기
	vector<shared_ptr<CDungeonRoom>> Dungeon1_v;
	Dungeon1_v.push_back(m_pRoom1);
	Dungeon1_v.push_back(m_pRoom2);
	m_pBossMap->PushDungeonRoomVector(Dungeon1_v);

	// 현재 active 방
	m_pBossMap->DisableAllRoom();
	m_pBossMap->AbleRoom(1);
	dynamic_pointer_cast<CPlayer>(CGameMgr::GetInstance()->GetPlayer())->SetCurrentRoom(1);

	// Layer에 GameObject 넣기

	m_pLayer->CreateGameObject(L"Obj_PlayerHand", dynamic_pointer_cast<CPlayer>(CGameMgr::GetInstance()->GetPlayer())->GetPlayerHand());

	dynamic_pointer_cast<CLayer>(m_pLayer)->AwakeLayer();

	return S_OK;
}

HRESULT CBossMap::Ready_Layer_UI(tstring pLayerTag)
{
	shared_ptr<CLayer> m_pLayer = make_shared<CLayer>();
	m_mapLayer.insert({ pLayerTag, m_pLayer });


	dynamic_pointer_cast<CLayer>(m_pLayer)->AwakeLayer();

	return S_OK;
}
