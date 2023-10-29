#include "pch.h"
#include "Weald_Dungeon.h"
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

#include "WealdDungeonDoor.h"

#include "BrigandCutthroat.h"
#include "BrigandFusilier.h"
#include "BrigandBloodletter.h"

#include "BoneCourtier.h"
#include "BoneDefender.h"
#include "BoneSoldier.h"

#include "Highwayman.h"
#include "Jester.h"
#include "Vestal.h"
#include "ShieldBreaker.h"

#include "Export_System.h"
#include "Export_Utility.h"

#include"ResourceMgr.h"

CWeald_Dungeon::CWeald_Dungeon(LPDIRECT3DDEVICE9 pGraphicDev)
	:CScene(pGraphicDev)
{
	ZeroMemory(m_szString, sizeof(m_szString));
}

CWeald_Dungeon::~CWeald_Dungeon()
{
}

HRESULT CWeald_Dungeon::ReadyScene()
{
	CResourceMgr::GetInstance()->WealdDungeonTextureLoad();

	// text
	lstrcpy(m_szString, L"Weald Dungeon");
	// Weald 던전
	m_pWealdDungeon = make_shared<CDungeonSystem>();
	// 던전 방1
	m_pRoom1 = make_shared<CDungeonRoom>();
	m_pRoom2 = make_shared<CDungeonRoom>();
	m_pRoom3 = make_shared<CDungeonRoom>();
	m_pRoom4 = make_shared<CDungeonRoom>();

	Ready_Layer_Environment(L"Layer_3_Environment");
	Ready_Layer_SkyBox(L"Layer_1_SkyBox");
	Ready_Layer_GameObject(L"Layer_4_GameObj");
	Ready_Layer_UI(L"Layer_2_UI");
	Ready_Layer_Camera(L"Layer_5_Camera");

	for (auto& iter : m_mapLayer) {
		//GameComponenet Setting
		iter.second->ReadyLayer();
	}

	return __super::ReadyScene();
}

_int CWeald_Dungeon::UpdateScene(const _float& fTimeDelta)
{
	// 방 이동했는지 체크
	if (dynamic_pointer_cast<CPlayer>(CGameMgr::GetInstance()->GetPlayer())->GetRoomChange())
	{
		dynamic_pointer_cast<CPlayer>(CGameMgr::GetInstance()->GetPlayer())->SetRoomChange(false);
		m_pWealdDungeon->CurrentRoom(dynamic_pointer_cast<CPlayer>(CGameMgr::GetInstance()->GetPlayer())->GetCurrentRoom());
	}

	if (dynamic_pointer_cast<CPlayer>(CGameMgr::GetInstance()->GetPlayer())->GetCurrentRoom() == 3)
	{
		m_pRoom3->BattleUpdate(fTimeDelta);
	}

	//// 방 1에 들어가면
	//if (GetAsyncKeyState('8') & 0x8000) {
	//	m_pWealdDungeon->CurrentRoom(0);
	//	m_bTestBattle = true;
	//}

	// 방 2에 들어가면
	if (GetAsyncKeyState('9') & 0x8000) {
		m_pWealdDungeon->CurrentRoom(1);
	}

	//if (m_bTestBattle)
	//{
	//	m_pRoom3->BattleUpdate(fTimeDelta);
	//}

	int iExit;
	iExit = __super::UpdateScene(fTimeDelta);

	if (GetAsyncKeyState('5') & 0x8000) {
		shared_ptr<CVillage> pScene = make_shared<CVillage>(m_pGraphicDev);
		CSceneMgr::GetInstance()->ChangeScene(pScene);
		pScene->ReadyScene();
	}

	return iExit;
}

void CWeald_Dungeon::LateUpdateScene()
{
	__super::LateUpdateScene();
	
}

void CWeald_Dungeon::RenderScene()
{
	// Font
	_vec2 vDungeonName = _vec2(0.f, 0.f);
	Engine::Render_Font(L"Font_Default", m_szString,
		&vDungeonName, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

	_vec2 vFontPos = _vec2(400.f, 100.f);
	Engine::Render_Font(L"Font_Default",
		dynamic_pointer_cast<CCreature>((m_pRoom3->GetBattleSystem()->GetHeroesVector())[0])->Get_String(),
		&vFontPos, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

	vFontPos = _vec2(400.f, 200.f);
	Engine::Render_Font(L"Font_Default",
		dynamic_pointer_cast<CCreature>((m_pRoom3->GetBattleSystem()->GetHeroesVector())[0])->Get_String2(),
		&vFontPos, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

	vFontPos = _vec2(400.f, 300.f);
	Engine::Render_Font(L"Font_Default",
		dynamic_pointer_cast<CCreature>((m_pRoom3->GetBattleSystem()->GetHeroesVector())[0])->Get_String3(),
		&vFontPos, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

	for (int i = 0; i < 3; i++)
	{
		vFontPos = _vec2(300.f - i*100.f, 100.f);
		Engine::Render_Font(L"Font_Default",
			dynamic_pointer_cast<CCreature>((m_pRoom3->GetBattleSystem()->GetHeroesVector())[i + 1])->Get_String(),
			&vFontPos, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

		vFontPos = _vec2(300.f - i * 100.f, 200.f);
		Engine::Render_Font(L"Font_Default",
			dynamic_pointer_cast<CCreature>((m_pRoom3->GetBattleSystem()->GetHeroesVector())[i + 1])->Get_String2(),
			&vFontPos, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

		vFontPos = _vec2(300.f - i * 100.f, 300.f);
		Engine::Render_Font(L"Font_Default",
			dynamic_pointer_cast<CCreature>((m_pRoom3->GetBattleSystem()->GetHeroesVector())[i + 1])->Get_String3(),
			&vFontPos, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
	}

	for (int i = 0; i < 4; i++)
	{
		vFontPos = _vec2(600.f + i * 100.f, 100.f);
		Engine::Render_Font(L"Font_Default",
			dynamic_pointer_cast<CCreature>((m_pRoom3->GetBattleSystem()->GetMonstersVector())[i])->Get_String(),
			&vFontPos, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

		vFontPos = _vec2(600.f + i * 100.f, 200.f);
		Engine::Render_Font(L"Font_Default",
			dynamic_pointer_cast<CCreature>((m_pRoom3->GetBattleSystem()->GetMonstersVector())[i])->Get_String2(),
			&vFontPos, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

		vFontPos = _vec2(600.f + i * 100.f, 300.f);
		Engine::Render_Font(L"Font_Default",
			dynamic_pointer_cast<CCreature>((m_pRoom3->GetBattleSystem()->GetMonstersVector())[i])->Get_String3(),
			&vFontPos, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
	}
}

HRESULT CWeald_Dungeon::Ready_Layer_Environment(tstring pLayerTag)
{
	shared_ptr<CLayer> m_pLayer = make_shared<CLayer>();
	m_mapLayer.insert({ pLayerTag, m_pLayer });

	shared_ptr<CGameObject> m_pTerrain = make_shared<CTerrain>(m_pGraphicDev, L"Com_Weald_FloorTexture");
	m_pLayer->CreateGameObject(L"OBJ_Terrain", m_pTerrain);

	shared_ptr<CGameObject> m_pDoor;
	shared_ptr<CGameObject> m_pWall;
	shared_ptr<CGameObject> m_pBackWall;

	// 문
	m_pDoor = make_shared<CWealdDungeonDoor>(m_pGraphicDev, EWealdDungeonDoorType::ROOM1);
	m_pDoor->SetPos(_vec3(WEALD_WALLSIZEX * 2.f + WEALD_WALLSIZEX / 2.f + WEALD_WALLSIZEX * 1.f, WEALD_WALLSIZEUPY + 2.f, WEALD_WALLSIZEX * 2.f));
	m_pDoor->SetAngle(_vec3(0.f, 0.f, 0.f));
	m_pDoor->SetScale(_vec3(WEALD_PATHSIZEX / 2.f + 1.5f, WEALD_PATHSIZEX / 2.f + 1.5f, 1.f));
	m_pLayer->CreateGameObject(L"OBJ_Door", m_pDoor);

	m_pDoor = make_shared<CWealdDungeonDoor>(m_pGraphicDev, EWealdDungeonDoorType::ROOM2);
	m_pDoor->SetPos(_vec3(WEALD_WALLSIZEX * 2.f + WEALD_WALLSIZEX / 2.f + WEALD_WALLSIZEX * 1.f, WEALD_WALLSIZEUPY + 2.f, WEALD_WALLSIZEX * 12.f));
	m_pDoor->SetAngle(_vec3(0.f, 0.f, 0.f));
	m_pDoor->SetScale(_vec3(WEALD_PATHSIZEX / 2.f + 1.5f, WEALD_PATHSIZEX / 2.f + 1.5f, 1.f));
	m_pLayer->CreateGameObject(L"OBJ_Door", m_pDoor);

	m_pDoor = make_shared<CWealdDungeonDoor>(m_pGraphicDev, EWealdDungeonDoorType::ROOM3);
	m_pDoor->SetPos(_vec3(WEALD_WALLSIZEX * 2.f + WEALD_WALLSIZEX / 2.f + WEALD_WALLSIZEX * 1.f, WEALD_WALLSIZEUPY + 2.f, WEALD_WALLSIZEX * 16.f));
	m_pDoor->SetAngle(_vec3(0.f, 0.f, 0.f));
	m_pDoor->SetScale(_vec3(WEALD_PATHSIZEX / 2.f + 1.5f, WEALD_PATHSIZEX / 2.f + 1.5f, 1.f));
	m_pLayer->CreateGameObject(L"OBJ_Door", m_pDoor);
	
	m_pDoor = make_shared<CWealdDungeonDoor>(m_pGraphicDev, EWealdDungeonDoorType::ROOM4);
	m_pDoor->SetPos(_vec3(WEALD_WALLSIZEX * 2.f + WEALD_WALLSIZEX / 2.f + WEALD_WALLSIZEX * 1.f, WEALD_WALLSIZEUPY + 2.f, WEALD_WALLSIZEX * 26.f));
	m_pDoor->SetAngle(_vec3(0.f, 0.f, 0.f));
	m_pDoor->SetScale(_vec3(WEALD_PATHSIZEX / 2.f + 1.5f, WEALD_PATHSIZEX / 2.f + 1.5f, 1.f));
	m_pLayer->CreateGameObject(L"OBJ_Door", m_pDoor);

	// 진입방 벽
	for (int i = 0; i < 2; i++)
	{
		m_pWall = make_shared<CWall>(m_pGraphicDev, L"Com_Weald_WallTexture", 9, true);	
		m_pWall->SetPos(_vec3(WEALD_WALLSIZEX * 2.f, WEALD_WALLSIZEUPY, WEALD_WALLSIZEX / 2.f + WEALD_WALLSIZEX * i));
		m_pWall->SetAngle(_vec3(0.f, 0.f, 0.f));
		m_pWall->SetScale(_vec3(WEALD_WALLSIZEX / 2.f, WEALD_WALLSIZEX / 2.f, 1.f));
		m_pLayer->CreateGameObject(L"OBJ_Wall", m_pWall);

		m_pBackWall = make_shared<CWall>(m_pGraphicDev, L"Com_Weald_BackWallTexture", 1, false);
		m_pBackWall->SetPos(_vec3(WEALD_WALLSIZEX * 2.f - 0.1f, WEALD_WALLSIZEUPY + 2.f, WEALD_WALLSIZEX / 2.f + WEALD_WALLSIZEX * i));
		m_pBackWall->SetAngle(_vec3(0.f, 0.f, 0.f));
		m_pBackWall->SetScale(_vec3(WEALD_WALLSIZEX / 2.f, WEALD_WALLSIZEX * 7.f / 10.f, 1));
		m_pLayer->CreateGameObject(L"OBJ_Back", m_pBackWall);
	}
	
	for (int i = 0; i < 2; i++)
	{
		m_pWall = make_shared<CWall>(m_pGraphicDev, L"Com_Weald_WallTexture", 9, true);
		m_pWall->SetPos(_vec3(WEALD_WALLSIZEX * 4.f + WEALD_PATHSIZEX + WEALD_WALLSIZEX * 2.f, WEALD_WALLSIZEUPY, WEALD_WALLSIZEX / 2.f + WEALD_WALLSIZEX * i));
		m_pWall->SetAngle(_vec3(0.f, 0.f, 0.f));
		m_pWall->SetScale(_vec3(WEALD_WALLSIZEX / 2.f, WEALD_WALLSIZEX / 2.f, 1));
		m_pLayer->CreateGameObject(L"OBJ_Wall", m_pWall);

		m_pBackWall = make_shared<CWall>(m_pGraphicDev, L"Com_Weald_BackWallTexture", 1, false);
		m_pBackWall->SetPos(_vec3(WEALD_WALLSIZEX * 4.f + WEALD_PATHSIZEX + WEALD_WALLSIZEX * 2.f + 0.1f, WEALD_WALLSIZEUPY + 2.f, WEALD_WALLSIZEX / 2.f + WEALD_WALLSIZEX * i));
		m_pBackWall->SetAngle(_vec3(0.f, 0.f, 0.f));
		m_pBackWall->SetScale(_vec3(WEALD_WALLSIZEX / 2.f, WEALD_WALLSIZEX * 7.f / 10.f, 1));
		m_pLayer->CreateGameObject(L"OBJ_Back", m_pBackWall);
	}

	for (int i = 0; i < 1; i++)
	{
		m_pWall = make_shared<CWall>(m_pGraphicDev, L"Com_Weald_WallTexture", 9, true);
		m_pWall->SetPos(_vec3(WEALD_WALLSIZEX * 2.f + WEALD_WALLSIZEX / 2.f + WEALD_WALLSIZEX * i, WEALD_WALLSIZEUPY, WEALD_WALLSIZEX * 2.f));
		m_pWall->SetAngle(_vec3(0.f, PI / 2.f, 0.f));
		m_pWall->SetScale(_vec3(WEALD_WALLSIZEX / 2.f, WEALD_WALLSIZEX / 2.f, 1.f));
		m_pLayer->CreateGameObject(L"OBJ_Wall", m_pWall);

		m_pBackWall = make_shared<CWall>(m_pGraphicDev, L"Com_Weald_BackWallTexture", 1, false);
		m_pBackWall->SetPos(_vec3(WEALD_WALLSIZEX * 2.f + WEALD_WALLSIZEX / 2.f + WEALD_WALLSIZEX * i, WEALD_WALLSIZEUPY + 2.f, WEALD_WALLSIZEX * 2.f + 0.1f));
		m_pBackWall->SetAngle(_vec3(0.f, PI / 2.f, 0.f));
		m_pBackWall->SetScale(_vec3(WEALD_WALLSIZEX / 2.f, WEALD_WALLSIZEX * 7.f / 10.f, 1));
		m_pLayer->CreateGameObject(L"OBJ_Back", m_pBackWall);
	}

	for (int i = 0; i < 3; i++)
	{
		m_pWall = make_shared<CWall>(m_pGraphicDev, L"Com_Weald_WallTexture", 9, true);
		m_pWall->SetPos(_vec3(WEALD_WALLSIZEX * 2.f + WEALD_PATHSIZEX + WEALD_WALLSIZEX * 1.5f + WEALD_WALLSIZEX * i, WEALD_WALLSIZEUPY, WEALD_WALLSIZEX * 2.f));
		m_pWall->SetAngle(_vec3(0.f, PI / 2.f, 0.f));
		m_pWall->SetScale(_vec3(WEALD_WALLSIZEX / 2.f, WEALD_WALLSIZEX / 2.f, 1.f));
		m_pLayer->CreateGameObject(L"OBJ_Wall", m_pWall);

		m_pBackWall = make_shared<CWall>(m_pGraphicDev, L"Com_Weald_BackWallTexture", 1, false);
		m_pBackWall->SetPos(_vec3(WEALD_WALLSIZEX * 2.f + WEALD_PATHSIZEX + WEALD_WALLSIZEX * 1.5f + WEALD_WALLSIZEX * i, WEALD_WALLSIZEUPY + 2.f, WEALD_WALLSIZEX * 2.f + 0.1f));
		m_pBackWall->SetAngle(_vec3(0.f, PI / 2.f, 0.f));
		m_pBackWall->SetScale(_vec3(WEALD_WALLSIZEX / 2.f, WEALD_WALLSIZEX * 7.f / 10.f, 1));
		m_pLayer->CreateGameObject(L"OBJ_Back", m_pBackWall);
	}

	// 통로 1 벽
	for (int i = 0; i < 10; i++)
	{
		m_pWall = make_shared<CWall>(m_pGraphicDev, L"Com_Weald_WallTexture", 9, true);
		m_pWall->SetPos(_vec3(WEALD_WALLSIZEX * 2.f + WEALD_WALLSIZEX * 1.f, WEALD_WALLSIZEUPY, WEALD_WALLSIZEX * 2.f + WEALD_WALLSIZEX / 2.f + WEALD_WALLSIZEX * i));
		m_pWall->SetAngle(_vec3(0.f, 0.f, 0.f));
		m_pWall->SetScale(_vec3(WEALD_WALLSIZEX / 2.f, WEALD_WALLSIZEX / 2.f, 1.f));
		m_pLayer->CreateGameObject(L"OBJ_Wall", m_pWall);

		m_pBackWall = make_shared<CWall>(m_pGraphicDev, L"Com_Weald_BackWallTexture", 1, false);
		m_pBackWall->SetPos(_vec3(WEALD_WALLSIZEX * 2.f + WEALD_WALLSIZEX * 1.f - 0.1f, WEALD_WALLSIZEUPY + 2.f, WEALD_WALLSIZEX * 2.f + WEALD_WALLSIZEX / 2.f + WEALD_WALLSIZEX * i));
		m_pBackWall->SetAngle(_vec3(0.f, 0.f, 0.f));
		m_pBackWall->SetScale(_vec3(WEALD_WALLSIZEX / 2.f, WEALD_WALLSIZEX * 7.f / 10.f, 1));
		m_pLayer->CreateGameObject(L"OBJ_Back", m_pBackWall);
	}

	for (int i = 0; i < 10; i++)
	{
		m_pWall = make_shared<CWall>(m_pGraphicDev, L"Com_Weald_WallTexture", 9, true);
		m_pWall->SetPos(_vec3(WEALD_WALLSIZEX * 2.f + WEALD_PATHSIZEX + WEALD_WALLSIZEX * 1.f, WEALD_WALLSIZEUPY, WEALD_WALLSIZEX * 2.f + WEALD_WALLSIZEX / 2.f + WEALD_WALLSIZEX * i));
		m_pWall->SetAngle(_vec3(0.f, 0.f, 0.f));
		m_pWall->SetScale(_vec3(WEALD_WALLSIZEX / 2.f, WEALD_WALLSIZEX / 2.f, 1.f));
		m_pLayer->CreateGameObject(L"OBJ_Wall", m_pWall);

		m_pBackWall = make_shared<CWall>(m_pGraphicDev, L"Com_Weald_BackWallTexture", 1, false);
		m_pBackWall->SetPos(_vec3(WEALD_WALLSIZEX * 2.f + WEALD_PATHSIZEX + WEALD_WALLSIZEX * 1.f + 0.1f, WEALD_WALLSIZEUPY + 2.f, WEALD_WALLSIZEX * 2.f + WEALD_WALLSIZEX / 2.f + WEALD_WALLSIZEX * i));
		m_pBackWall->SetAngle(_vec3(0.f, 0.f, 0.f));
		m_pBackWall->SetScale(_vec3(WEALD_WALLSIZEX / 2.f, WEALD_WALLSIZEX * 7.f / 10.f, 1));
		m_pLayer->CreateGameObject(L"OBJ_Back", m_pBackWall);
	}

	// 방 2 벽
	for (int i = 0; i < 4; i++)
	{
		m_pWall = make_shared<CWall>(m_pGraphicDev, L"Com_Weald_WallTexture", 9, true);
		m_pWall->SetPos(_vec3(WEALD_WALLSIZEX * 1.f, WEALD_WALLSIZEUPY, WEALD_WALLSIZEX / 2.f + WEALD_WALLSIZEX * 12.f + WEALD_WALLSIZEX * i));
		m_pWall->SetAngle(_vec3(0.f, 0.f, 0.f));
		m_pWall->SetScale(_vec3(WEALD_WALLSIZEX / 2.f, WEALD_WALLSIZEX / 2.f, 1.f));
		m_pLayer->CreateGameObject(L"OBJ_Wall", m_pWall);

		m_pBackWall = make_shared<CWall>(m_pGraphicDev, L"Com_Weald_BackWallTexture", 1, false);
		m_pBackWall->SetPos(_vec3(WEALD_WALLSIZEX * 1.f - 0.1f, WEALD_WALLSIZEUPY + 2.f, WEALD_WALLSIZEX / 2.f + WEALD_WALLSIZEX * 12.f + WEALD_WALLSIZEX * i));
		m_pBackWall->SetAngle(_vec3(0.f, 0.f, 0.f));
		m_pBackWall->SetScale(_vec3(WEALD_WALLSIZEX / 2.f, WEALD_WALLSIZEX * 7.f / 10.f, 1));
		m_pLayer->CreateGameObject(L"OBJ_Back", m_pBackWall);
	}

	for (int i = 0; i < 4; i++)
	{
		m_pWall = make_shared<CWall>(m_pGraphicDev, L"Com_Weald_WallTexture", 9, true);
		m_pWall->SetPos(_vec3(WEALD_WALLSIZEX * 3.f + WEALD_PATHSIZEX + WEALD_WALLSIZEX * 2.f, WEALD_WALLSIZEUPY, WEALD_WALLSIZEX / 2.f + WEALD_WALLSIZEX * 12.f + WEALD_WALLSIZEX * i));
		m_pWall->SetAngle(_vec3(0.f, 0.f, 0.f));
		m_pWall->SetScale(_vec3(WEALD_WALLSIZEX / 2.f, WEALD_WALLSIZEX / 2.f, 1.f));
		m_pLayer->CreateGameObject(L"OBJ_Wall", m_pWall);

		m_pBackWall = make_shared<CWall>(m_pGraphicDev, L"Com_Weald_BackWallTexture", 1, false);
		m_pBackWall->SetPos(_vec3(WEALD_WALLSIZEX * 3.f + WEALD_PATHSIZEX + WEALD_WALLSIZEX * 2.f + 0.1f, WEALD_WALLSIZEUPY + 2.f, WEALD_WALLSIZEX / 2.f + WEALD_WALLSIZEX * 12.f + WEALD_WALLSIZEX * i));
		m_pBackWall->SetAngle(_vec3(0.f, 0.f, 0.f));
		m_pBackWall->SetScale(_vec3(WEALD_WALLSIZEX / 2.f, WEALD_WALLSIZEX * 7.f / 10.f, 1));
		m_pLayer->CreateGameObject(L"OBJ_Back", m_pBackWall);
	}

	for (int i = 0; i < 2; i++)
	{
		m_pWall = make_shared<CWall>(m_pGraphicDev, L"Com_Weald_WallTexture", 9, true);
		m_pWall->SetPos(_vec3(WEALD_WALLSIZEX * 1.5f + WEALD_WALLSIZEX * i, WEALD_WALLSIZEUPY, WEALD_WALLSIZEX * 12.f));
		m_pWall->SetAngle(_vec3(0.f, PI / 2.f, 0.f));
		m_pWall->SetScale(_vec3(WEALD_WALLSIZEX / 2.f, WEALD_WALLSIZEX / 2.f, 1.f));
		m_pLayer->CreateGameObject(L"OBJ_Wall", m_pWall);

		m_pBackWall = make_shared<CWall>(m_pGraphicDev, L"Com_Weald_BackWallTexture", 1, false);
		m_pBackWall->SetPos(_vec3(WEALD_WALLSIZEX * 1.5f + WEALD_WALLSIZEX * i, WEALD_WALLSIZEUPY + 2.f, WEALD_WALLSIZEX * 12.f - 0.1f));
		m_pBackWall->SetAngle(_vec3(0.f, PI / 2.f, 0.f));
		m_pBackWall->SetScale(_vec3(WEALD_WALLSIZEX / 2.f, WEALD_WALLSIZEX * 7.f / 10.f, 1));
		m_pLayer->CreateGameObject(L"OBJ_Back", m_pBackWall);
	}

	for (int i = 0; i < 2; i++)
	{
		m_pWall = make_shared<CWall>(m_pGraphicDev, L"Com_Weald_WallTexture", 9, true);
		m_pWall->SetPos(_vec3(WEALD_WALLSIZEX * 3.5f + WEALD_PATHSIZEX + WEALD_WALLSIZEX * i, WEALD_WALLSIZEUPY, WEALD_WALLSIZEX * 12.f));
		m_pWall->SetAngle(_vec3(0.f, PI / 2.f, 0.f));
		m_pWall->SetScale(_vec3(WEALD_WALLSIZEX / 2.f, WEALD_WALLSIZEX / 2.f, 1.f));
		m_pLayer->CreateGameObject(L"OBJ_Wall", m_pWall);

		m_pBackWall = make_shared<CWall>(m_pGraphicDev, L"Com_Weald_BackWallTexture", 1, false);
		m_pBackWall->SetPos(_vec3(WEALD_WALLSIZEX * 3.5f + WEALD_PATHSIZEX + WEALD_WALLSIZEX * i, WEALD_WALLSIZEUPY + 2.f, WEALD_WALLSIZEX * 12.f - 0.1f));
		m_pBackWall->SetAngle(_vec3(0.f, PI / 2.f, 0.f));
		m_pBackWall->SetScale(_vec3(WEALD_WALLSIZEX / 2.f, WEALD_WALLSIZEX * 7.f / 10.f, 1));
		m_pLayer->CreateGameObject(L"OBJ_Back", m_pBackWall);
	}

	for (int i = 0; i < 2; i++)
	{
		m_pWall = make_shared<CWall>(m_pGraphicDev, L"Com_Weald_WallTexture", 9, true);
		m_pWall->SetPos(_vec3(WEALD_WALLSIZEX * 1.5f + WEALD_WALLSIZEX * i, WEALD_WALLSIZEUPY, WEALD_WALLSIZEX * 16.f));
		m_pWall->SetAngle(_vec3(0.f, PI / 2.f, 0.f));
		m_pWall->SetScale(_vec3(WEALD_WALLSIZEX / 2.f, WEALD_WALLSIZEX / 2.f, 1.f));
		m_pLayer->CreateGameObject(L"OBJ_Wall", m_pWall);

		m_pBackWall = make_shared<CWall>(m_pGraphicDev, L"Com_Weald_BackWallTexture", 1, false);
		m_pBackWall->SetPos(_vec3(WEALD_WALLSIZEX * 1.5f + WEALD_WALLSIZEX * i, WEALD_WALLSIZEUPY + 2.f, WEALD_WALLSIZEX * 16.f + 0.1f));
		m_pBackWall->SetAngle(_vec3(0.f, PI / 2.f, 0.f));
		m_pBackWall->SetScale(_vec3(WEALD_WALLSIZEX / 2.f, WEALD_WALLSIZEX * 7.f / 10.f, 1));
		m_pLayer->CreateGameObject(L"OBJ_Back", m_pBackWall);
	}

	for (int i = 0; i < 2; i++)
	{
		m_pWall = make_shared<CWall>(m_pGraphicDev, L"Com_Weald_WallTexture", 9, true);
		m_pWall->SetPos(_vec3(WEALD_WALLSIZEX * 3.5f + WEALD_PATHSIZEX + WEALD_WALLSIZEX * i, WEALD_WALLSIZEUPY, WEALD_WALLSIZEX * 16.f));
		m_pWall->SetAngle(_vec3(0.f, PI / 2.f, 0.f));
		m_pWall->SetScale(_vec3(WEALD_WALLSIZEX / 2.f, WEALD_WALLSIZEX / 2.f, 1.f));
		m_pLayer->CreateGameObject(L"OBJ_Wall", m_pWall);

		m_pBackWall = make_shared<CWall>(m_pGraphicDev, L"Com_Weald_BackWallTexture", 1, false);
		m_pBackWall->SetPos(_vec3(WEALD_WALLSIZEX * 3.5f + WEALD_PATHSIZEX + WEALD_WALLSIZEX * i, WEALD_WALLSIZEUPY + 2.f, WEALD_WALLSIZEX * 16.f + 0.1f));
		m_pBackWall->SetAngle(_vec3(0.f, PI / 2.f, 0.f));
		m_pBackWall->SetScale(_vec3(WEALD_WALLSIZEX / 2.f, WEALD_WALLSIZEX * 7.f / 10.f, 1));
		m_pLayer->CreateGameObject(L"OBJ_Back", m_pBackWall);
	}

	// 통로 2 벽
	for (int i = 0; i < 10; i++)
	{
		m_pWall = make_shared<CWall>(m_pGraphicDev, L"Com_Weald_WallTexture", 9, true);
		m_pWall->SetPos(_vec3(WEALD_WALLSIZEX * 2.f + WEALD_WALLSIZEX * 1.f, WEALD_WALLSIZEUPY, WEALD_WALLSIZEX * 16.f + WEALD_WALLSIZEX / 2.f + WEALD_WALLSIZEX * i));
		m_pWall->SetAngle(_vec3(0.f, 0.f, 0.f));
		m_pWall->SetScale(_vec3(WEALD_WALLSIZEX / 2.f, WEALD_WALLSIZEX / 2.f, 1.f));
		m_pLayer->CreateGameObject(L"OBJ_Wall", m_pWall);

		m_pBackWall = make_shared<CWall>(m_pGraphicDev, L"Com_Weald_BackWallTexture", 1, false);
		m_pBackWall->SetPos(_vec3(WEALD_WALLSIZEX * 2.f + WEALD_WALLSIZEX * 1.f - 0.1f, WEALD_WALLSIZEUPY + 2.f, WEALD_WALLSIZEX * 16.f + WEALD_WALLSIZEX / 2.f + WEALD_WALLSIZEX * i));
		m_pBackWall->SetAngle(_vec3(0.f, 0.f, 0.f));
		m_pBackWall->SetScale(_vec3(WEALD_WALLSIZEX / 2.f, WEALD_WALLSIZEX * 7.f / 10.f, 1));
		m_pLayer->CreateGameObject(L"OBJ_Back", m_pBackWall);
	}

	for (int i = 0; i < 5; i++)
	{
		m_pWall = make_shared<CWall>(m_pGraphicDev, L"Com_Weald_WallTexture", 9, true);
		m_pWall->SetPos(_vec3(WEALD_WALLSIZEX * 2.f + WEALD_PATHSIZEX + WEALD_WALLSIZEX * 1.f, WEALD_WALLSIZEUPY, WEALD_WALLSIZEX * 16.f + WEALD_WALLSIZEX / 2.f + WEALD_WALLSIZEX * i));
		m_pWall->SetAngle(_vec3(0.f, 0.f, 0.f));
		m_pWall->SetScale(_vec3(WEALD_WALLSIZEX / 2.f, WEALD_WALLSIZEX / 2.f, 1.f));
		m_pLayer->CreateGameObject(L"OBJ_Wall", m_pWall);

		m_pBackWall = make_shared<CWall>(m_pGraphicDev, L"Com_Weald_BackWallTexture", 1, false);
		m_pBackWall->SetPos(_vec3(WEALD_WALLSIZEX * 2.f + WEALD_PATHSIZEX + WEALD_WALLSIZEX * 1.f + 0.1f, WEALD_WALLSIZEUPY + 2.f, WEALD_WALLSIZEX * 16.f + WEALD_WALLSIZEX / 2.f + WEALD_WALLSIZEX * i));
		m_pBackWall->SetAngle(_vec3(0.f, 0.f, 0.f));
		m_pBackWall->SetScale(_vec3(WEALD_WALLSIZEX / 2.f, WEALD_WALLSIZEX * 7.f / 10.f, 1));
		m_pLayer->CreateGameObject(L"OBJ_Back", m_pBackWall);
	}

	for (int i = 6; i < 10; i++)
	{
		m_pWall = make_shared<CWall>(m_pGraphicDev, L"Com_Weald_WallTexture", 9, true);
		m_pWall->SetPos(_vec3(WEALD_WALLSIZEX * 2.f + WEALD_PATHSIZEX + WEALD_WALLSIZEX * 1.f, WEALD_WALLSIZEUPY, WEALD_WALLSIZEX * 16.f + WEALD_WALLSIZEX / 2.f + WEALD_WALLSIZEX * i));
		m_pWall->SetAngle(_vec3(0.f, 0.f, 0.f));
		m_pWall->SetScale(_vec3(WEALD_WALLSIZEX / 2.f, WEALD_WALLSIZEX / 2.f, 1.f));
		m_pLayer->CreateGameObject(L"OBJ_Wall", m_pWall);

		m_pBackWall = make_shared<CWall>(m_pGraphicDev, L"Com_Weald_BackWallTexture", 1, false);
		m_pBackWall->SetPos(_vec3(WEALD_WALLSIZEX * 2.f + WEALD_PATHSIZEX + WEALD_WALLSIZEX * 1.f + 0.1f, WEALD_WALLSIZEUPY + 2.f, WEALD_WALLSIZEX * 16.f + WEALD_WALLSIZEX / 2.f + WEALD_WALLSIZEX * i));
		m_pBackWall->SetAngle(_vec3(0.f, 0.f, 0.f));
		m_pBackWall->SetScale(_vec3(WEALD_WALLSIZEX / 2.f, WEALD_WALLSIZEX * 7.f / 10.f, 1));
		m_pLayer->CreateGameObject(L"OBJ_Back", m_pBackWall);
	}

	// 비밀 방
	for (int i = 0; i < 4; i++)
	{
		m_pWall = make_shared<CWall>(m_pGraphicDev, L"Com_Weald_WallTexture", 9, true);
		m_pWall->SetPos(_vec3(WEALD_WALLSIZEX * 3.5f + WEALD_PATHSIZEX + WEALD_WALLSIZEX * i, WEALD_WALLSIZEUPY, WEALD_WALLSIZEX * 22.f));
		m_pWall->SetAngle(_vec3(0.f, PI / 2.f, 0.f));
		m_pWall->SetScale(_vec3(WEALD_WALLSIZEX / 2.f, WEALD_WALLSIZEX / 2.f, 1.f));
		m_pLayer->CreateGameObject(L"OBJ_Wall", m_pWall);

		m_pBackWall = make_shared<CWall>(m_pGraphicDev, L"Com_Weald_BackWallTexture", 1, false);
		m_pBackWall->SetPos(_vec3(WEALD_WALLSIZEX * 3.5f + WEALD_PATHSIZEX + WEALD_WALLSIZEX * i, WEALD_WALLSIZEUPY + 2.f, WEALD_WALLSIZEX * 22.f + 0.1f));
		m_pBackWall->SetAngle(_vec3(0.f, PI / 2.f, 0.f));
		m_pBackWall->SetScale(_vec3(WEALD_WALLSIZEX / 2.f, WEALD_WALLSIZEX * 7.f / 10.f, 1));
		m_pLayer->CreateGameObject(L"OBJ_Back", m_pBackWall);
	}

	for (int i = 0; i < 4; i++)
	{
		m_pWall = make_shared<CWall>(m_pGraphicDev, L"Com_Weald_WallTexture", 9, true);
		m_pWall->SetPos(_vec3(WEALD_WALLSIZEX * 3.5f + WEALD_PATHSIZEX + WEALD_WALLSIZEX * i, WEALD_WALLSIZEUPY, WEALD_WALLSIZEX * 19.f));
		m_pWall->SetAngle(_vec3(0.f, PI / 2.f, 0.f));
		m_pWall->SetScale(_vec3(WEALD_WALLSIZEX / 2.f, WEALD_WALLSIZEX / 2.f, 1.f));
		m_pLayer->CreateGameObject(L"OBJ_Wall", m_pWall);

		m_pBackWall = make_shared<CWall>(m_pGraphicDev, L"Com_Weald_BackWallTexture", 1, false);
		m_pBackWall->SetPos(_vec3(WEALD_WALLSIZEX * 3.5f + WEALD_PATHSIZEX + WEALD_WALLSIZEX * i, WEALD_WALLSIZEUPY + 2.f, WEALD_WALLSIZEX * 19.f - 0.1f));
		m_pBackWall->SetAngle(_vec3(0.f, PI / 2.f, 0.f));
		m_pBackWall->SetScale(_vec3(WEALD_WALLSIZEX / 2.f, WEALD_WALLSIZEX * 7.f / 10.f, 1));
		m_pLayer->CreateGameObject(L"OBJ_Back", m_pBackWall);
	}

	for (int i = 0; i < 4; i++)
	{
		m_pWall = make_shared<CWall>(m_pGraphicDev, L"Com_Weald_WallTexture", 9, true);
		m_pWall->SetPos(_vec3(WEALD_WALLSIZEX * 1.f + WEALD_PATHSIZEX + WEALD_WALLSIZEX * 6.f, WEALD_WALLSIZEUPY, WEALD_WALLSIZEX / 2.f + WEALD_WALLSIZEX * 19.f + WEALD_WALLSIZEX * i));
		m_pWall->SetAngle(_vec3(0.f, 0.f, 0.f));
		m_pWall->SetScale(_vec3(WEALD_WALLSIZEX / 2.f, WEALD_WALLSIZEX / 2.f, 1.f));
		m_pLayer->CreateGameObject(L"OBJ_Wall", m_pWall);

		m_pBackWall = make_shared<CWall>(m_pGraphicDev, L"Com_Weald_BackWallTexture", 1, false);
		m_pBackWall->SetPos(_vec3(WEALD_WALLSIZEX * 1.f + WEALD_PATHSIZEX + WEALD_WALLSIZEX * 6.f + 0.1f, WEALD_WALLSIZEUPY + 2.f, WEALD_WALLSIZEX / 2.f + WEALD_WALLSIZEX * 19.f + WEALD_WALLSIZEX * i));
		m_pBackWall->SetAngle(_vec3(0.f, 0.f, 0.f));
		m_pBackWall->SetScale(_vec3(WEALD_WALLSIZEX / 2.f, WEALD_WALLSIZEX * 7.f / 10.f, 1));
		m_pLayer->CreateGameObject(L"OBJ_Back", m_pBackWall);
	}

	// 방 3 벽
	for (int i = 0; i < 4; i++)
	{
		m_pWall = make_shared<CWall>(m_pGraphicDev, L"Com_Weald_WallTexture", 9, true);
		m_pWall->SetPos(_vec3(WEALD_WALLSIZEX * 1.f, WEALD_WALLSIZEUPY, WEALD_WALLSIZEX / 2.f + WEALD_WALLSIZEX * 26.f + WEALD_WALLSIZEX * i));
		m_pWall->SetAngle(_vec3(0.f, 0.f, 0.f));
		m_pWall->SetScale(_vec3(WEALD_WALLSIZEX / 2.f, WEALD_WALLSIZEX / 2.f, 1.f));
		m_pLayer->CreateGameObject(L"OBJ_Wall", m_pWall);

		m_pBackWall = make_shared<CWall>(m_pGraphicDev, L"Com_Weald_BackWallTexture", 1, false);
		m_pBackWall->SetPos(_vec3(WEALD_WALLSIZEX * 1.f - 0.1f, WEALD_WALLSIZEUPY + 2.f, WEALD_WALLSIZEX / 2.f + WEALD_WALLSIZEX * 26.f + WEALD_WALLSIZEX * i));
		m_pBackWall->SetAngle(_vec3(0.f, 0.f, 0.f));
		m_pBackWall->SetScale(_vec3(WEALD_WALLSIZEX / 2.f, WEALD_WALLSIZEX * 7.f / 10.f, 1));
		m_pLayer->CreateGameObject(L"OBJ_Back", m_pBackWall);
	}

	for (int i = 0; i < 4; i++)
	{
		m_pWall = make_shared<CWall>(m_pGraphicDev, L"Com_Weald_WallTexture", 9, true);
		m_pWall->SetPos(_vec3(WEALD_WALLSIZEX * 3.f + WEALD_PATHSIZEX + WEALD_WALLSIZEX * 2.f, WEALD_WALLSIZEUPY, WEALD_WALLSIZEX / 2.f + WEALD_WALLSIZEX * 26.f + WEALD_WALLSIZEX * i));
		m_pWall->SetAngle(_vec3(0.f, 0.f, 0.f));
		m_pWall->SetScale(_vec3(WEALD_WALLSIZEX / 2.f, WEALD_WALLSIZEX / 2.f, 1.f));
		m_pLayer->CreateGameObject(L"OBJ_Wall", m_pWall);

		m_pBackWall = make_shared<CWall>(m_pGraphicDev, L"Com_Weald_BackWallTexture", 1, false);
		m_pBackWall->SetPos(_vec3(WEALD_WALLSIZEX * 3.f + WEALD_PATHSIZEX + WEALD_WALLSIZEX * 2.f + 0.1f, WEALD_WALLSIZEUPY + 2.f, WEALD_WALLSIZEX / 2.f + WEALD_WALLSIZEX * 26.f + WEALD_WALLSIZEX * i));
		m_pBackWall->SetAngle(_vec3(0.f, 0.f, 0.f));
		m_pBackWall->SetScale(_vec3(WEALD_WALLSIZEX / 2.f, WEALD_WALLSIZEX * 7.f / 10.f, 1));
		m_pLayer->CreateGameObject(L"OBJ_Back", m_pBackWall);
	}

	for (int i = 0; i < 2; i++)
	{
		m_pWall = make_shared<CWall>(m_pGraphicDev, L"Com_Weald_WallTexture", 9, true);
		m_pWall->SetPos(_vec3(WEALD_WALLSIZEX * 1.5f + WEALD_WALLSIZEX * i, WEALD_WALLSIZEUPY, WEALD_WALLSIZEX * 26.f));
		m_pWall->SetAngle(_vec3(0.f, PI / 2.f, 0.f));
		m_pWall->SetScale(_vec3(WEALD_WALLSIZEX / 2.f, WEALD_WALLSIZEX / 2.f, 1.f));
		m_pLayer->CreateGameObject(L"OBJ_Wall", m_pWall);

		m_pBackWall = make_shared<CWall>(m_pGraphicDev, L"Com_Weald_BackWallTexture", 1, false);
		m_pBackWall->SetPos(_vec3(WEALD_WALLSIZEX * 1.5f + WEALD_WALLSIZEX * i, WEALD_WALLSIZEUPY + 2.f, WEALD_WALLSIZEX * 26.f - 0.1f));
		m_pBackWall->SetAngle(_vec3(0.f, PI / 2.f, 0.f));
		m_pBackWall->SetScale(_vec3(WEALD_WALLSIZEX / 2.f, WEALD_WALLSIZEX * 7.f / 10.f, 1));
		m_pLayer->CreateGameObject(L"OBJ_Back", m_pBackWall);
	}

	for (int i = 0; i < 2; i++)
	{
		m_pWall = make_shared<CWall>(m_pGraphicDev, L"Com_Weald_WallTexture", 9, true);
		m_pWall->SetPos(_vec3(WEALD_WALLSIZEX * 3.5f + WEALD_PATHSIZEX + WEALD_WALLSIZEX * i, WEALD_WALLSIZEUPY, WEALD_WALLSIZEX * 26.f));
		m_pWall->SetAngle(_vec3(0.f, PI / 2.f, 0.f));
		m_pWall->SetScale(_vec3(WEALD_WALLSIZEX / 2.f, WEALD_WALLSIZEX / 2.f, 1.f));
		m_pLayer->CreateGameObject(L"OBJ_Wall", m_pWall);

		m_pBackWall = make_shared<CWall>(m_pGraphicDev, L"Com_Weald_BackWallTexture", 1, false);
		m_pBackWall->SetPos(_vec3(WEALD_WALLSIZEX * 3.5f + WEALD_PATHSIZEX + WEALD_WALLSIZEX * i, WEALD_WALLSIZEUPY + 2.f, WEALD_WALLSIZEX * 26.f - 0.1f));
		m_pBackWall->SetAngle(_vec3(0.f, PI / 2.f, 0.f));
		m_pBackWall->SetScale(_vec3(WEALD_WALLSIZEX / 2.f, WEALD_WALLSIZEX * 7.f / 10.f, 1));
		m_pLayer->CreateGameObject(L"OBJ_Back", m_pBackWall);
	}

	for (int i = 0; i < 2; i++)
	{
		m_pWall = make_shared<CWall>(m_pGraphicDev, L"Com_Weald_WallTexture", 9, true);
		m_pWall->SetPos(_vec3(WEALD_WALLSIZEX * 1.5f + WEALD_WALLSIZEX * i, WEALD_WALLSIZEUPY, WEALD_WALLSIZEX * 30.f));
		m_pWall->SetAngle(_vec3(0.f, PI / 2.f, 0.f));
		m_pWall->SetScale(_vec3(WEALD_WALLSIZEX / 2.f, WEALD_WALLSIZEX / 2.f, 1.f));
		m_pLayer->CreateGameObject(L"OBJ_Wall", m_pWall);

		m_pBackWall = make_shared<CWall>(m_pGraphicDev, L"Com_Weald_BackWallTexture", 1, false);
		m_pBackWall->SetPos(_vec3(WEALD_WALLSIZEX * 1.5f + WEALD_WALLSIZEX * i, WEALD_WALLSIZEUPY + 2.f, WEALD_WALLSIZEX * 30.f + 0.1f));
		m_pBackWall->SetAngle(_vec3(0.f, PI / 2.f, 0.f));
		m_pBackWall->SetScale(_vec3(WEALD_WALLSIZEX / 2.f, WEALD_WALLSIZEX * 7.f / 10.f, 1));
		m_pLayer->CreateGameObject(L"OBJ_Back", m_pBackWall);
	}

	for (int i = 0; i < 2; i++)
	{
		m_pWall = make_shared<CWall>(m_pGraphicDev, L"Com_Weald_WallTexture", 9, true);
		m_pWall->SetPos(_vec3(WEALD_WALLSIZEX * 3.5f + WEALD_PATHSIZEX + WEALD_WALLSIZEX * i, WEALD_WALLSIZEUPY, WEALD_WALLSIZEX * 30.f));
		m_pWall->SetAngle(_vec3(0.f, PI / 2.f, 0.f));
		m_pWall->SetScale(_vec3(WEALD_WALLSIZEX / 2.f, WEALD_WALLSIZEX / 2.f, 1.f));
		m_pLayer->CreateGameObject(L"OBJ_Wall", m_pWall);

		m_pBackWall = make_shared<CWall>(m_pGraphicDev, L"Com_Weald_BackWallTexture", 1, false);
		m_pBackWall->SetPos(_vec3(WEALD_WALLSIZEX * 3.5f + WEALD_PATHSIZEX + WEALD_WALLSIZEX * i, WEALD_WALLSIZEUPY + 2.f, WEALD_WALLSIZEX * 30.f + 0.1f));
		m_pBackWall->SetAngle(_vec3(0.f, PI / 2.f, 0.f));
		m_pBackWall->SetScale(_vec3(WEALD_WALLSIZEX / 2.f, WEALD_WALLSIZEX * 7.f / 10.f, 1));
		m_pLayer->CreateGameObject(L"OBJ_Back", m_pBackWall);
	}



	//가장 최하위 순서에 돌려줄 것
	dynamic_pointer_cast<CLayer>(m_pLayer)->AwakeLayer();


	return S_OK;
}

HRESULT CWeald_Dungeon::Ready_Layer_SkyBox(tstring pLayerTag)
{
	shared_ptr<CLayer> m_pLayer = make_shared<CLayer>();
	m_mapLayer.insert({ pLayerTag, m_pLayer });


	shared_ptr<CGameObject> m_pSkyBox = make_shared<CSkyBox>(m_pGraphicDev, L"Com_Weald_SkyBoxTexture");
	m_pLayer->CreateGameObject(L"OBJ_Weald_SkyBox", m_pSkyBox);

	dynamic_pointer_cast<CLayer>(m_pLayer)->AwakeLayer();
	
	return S_OK;
}

HRESULT CWeald_Dungeon::Ready_Layer_Camera(tstring pLayerTag)
{
	shared_ptr<CLayer> m_pLayer = make_shared<CLayer>();
	m_mapLayer.insert({ pLayerTag, m_pLayer });
	
	// Camera
	shared_ptr<CGameObject> m_pCamera = make_shared<CStaticCamera>(m_pGraphicDev);
	m_pLayer->CreateGameObject(L"OBJ_Camera", m_pCamera);

	CCameraMgr::GetInstance()->SetMainCamera(dynamic_pointer_cast<CStaticCamera>(m_pCamera));

	dynamic_pointer_cast<CLayer>(m_pLayer)->AwakeLayer();

	return S_OK;
}

HRESULT CWeald_Dungeon::Ready_Layer_GameObject(tstring pLayerTag)
{
	shared_ptr<CLayer> m_pLayer = make_shared<CLayer>();
	m_mapLayer.insert({ pLayerTag, m_pLayer });

	//Player
	shared_ptr<CGameObject> m_pPlayer = make_shared<CPlayer>(m_pGraphicDev);
	m_pPlayer->SetPos({ 4.f, 0.f, 0.f });
	m_pLayer->CreateGameObject(L"Obj_Player", m_pPlayer);

	CGameMgr::GetInstance()->SetPlayer(m_pPlayer);

	// GameObject
	// monsters
	shared_ptr<CGameObject> m_pBrigandCutthroat_1 = make_shared<CBrigandCutthroat>(m_pGraphicDev);
	shared_ptr<CGameObject> m_pBrigandCutthroat_2 = make_shared<CBrigandCutthroat>(m_pGraphicDev);
	shared_ptr<CGameObject> m_pBoneDefender1 = make_shared<CBoneDefender>(m_pGraphicDev);
	shared_ptr<CGameObject> m_pBrigandBloodletter1 = make_shared<CBrigandBloodletter>(m_pGraphicDev);
	shared_ptr<CGameObject> m_pBrigandFusilier_1 = make_shared<CBrigandFusilier>(m_pGraphicDev);
	shared_ptr<CGameObject> m_pBoneSoldier_1 = make_shared<CBoneSoldier>(m_pGraphicDev);
	shared_ptr<CGameObject> m_pBoneCourtier_1 = make_shared<CBoneCourtier>(m_pGraphicDev);
	shared_ptr<CGameObject> m_pBoneSoldier_2 = make_shared<CBoneSoldier>(m_pGraphicDev);

	// heroes
	shared_ptr<CGameObject> m_pSheldBreaker1 = make_shared<CShieldBreaker>(m_pGraphicDev);
	//shared_ptr<CGameObject> m_pSheldBreaker2 = make_shared<CShieldBreaker>(m_pGraphicDev);
	//shared_ptr<CGameObject> m_pSheldBreaker3 = make_shared<CShieldBreaker>(m_pGraphicDev);
	//shared_ptr<CGameObject> m_pSheldBreaker4 = make_shared<CShieldBreaker>(m_pGraphicDev);
	shared_ptr<CGameObject> m_pHighwayman = make_shared<CHighwayman>(m_pGraphicDev);
	shared_ptr<CGameObject> m_pJester = make_shared<CJester>(m_pGraphicDev);
	shared_ptr<CGameObject> m_pVestal = make_shared<CVestal>(m_pGraphicDev);

	// 방에 GameObject 넣기
	// creatures
	vector<shared_ptr<CGameObject>> Room3_v1;
	Room3_v1.push_back(m_pBrigandCutthroat_1);
	Room3_v1.push_back(m_pBrigandBloodletter1);
	Room3_v1.push_back(m_pBrigandCutthroat_2);
	Room3_v1.push_back(m_pBrigandFusilier_1);

	Room3_v1.push_back(m_pSheldBreaker1);
	//Room3_v1.push_back(m_pSheldBreaker2);
	//Room3_v1.push_back(m_pSheldBreaker3);
	//Room3_v1.push_back(m_pSheldBreaker4);

	Room3_v1.push_back(m_pHighwayman);
	Room3_v1.push_back(m_pJester);
	Room3_v1.push_back(m_pVestal);

	//Room3_v1.push_back(m_pBoneDefender1);
	//Room3_v1.push_back(m_pBoneSoldier_1);
	//Room3_v1.push_back(m_pBoneCourtier_1);
	//Room3_v1.push_back(m_pBoneSoldier_2);
	m_pRoom3->PushGameObjectVector(Room3_v1);

	// heroes
	vector<shared_ptr<CGameObject>> Room3_v2;
	Room3_v2.push_back(m_pSheldBreaker1);

	//Room3_v2.push_back(m_pSheldBreaker2);
	//Room3_v2.push_back(m_pSheldBreaker3);
	//Room3_v2.push_back(m_pSheldBreaker4);
	Room3_v2.push_back(m_pHighwayman);
	Room3_v2.push_back(m_pVestal);
	Room3_v2.push_back(m_pJester);
	//Room3_v2.push_back(m_pBoneDefender1);
	//Room3_v2.push_back(m_pBoneSoldier_1);
	//Room3_v2.push_back(m_pBoneSoldier_2);
	//Room3_v2.push_back(m_pBoneCourtier_1);
	m_pRoom3->PushHeroesVector(Room3_v2);

	// monsters
	vector<shared_ptr<CGameObject>> Room3_v3;
	Room3_v3.push_back(m_pBrigandBloodletter1);
	Room3_v3.push_back(m_pBrigandCutthroat_1);
	Room3_v3.push_back(m_pBrigandCutthroat_2);
	Room3_v3.push_back(m_pBrigandFusilier_1);
	m_pRoom3->PushMonstersVector(Room3_v3);

	// 배틀시스템 넣기
	shared_ptr<CBattleSystem> pRoom3_Battle = make_shared<CBattleSystem>();
	//pRoom3_Battle->Ready();
	pRoom3_Battle->PushCreaturesVector(Room3_v1);
	pRoom3_Battle->PushHeroesVector(Room3_v2);
	pRoom3_Battle->PushMonstersVector(Room3_v3);
	m_pRoom3->SetBattleSystem(pRoom3_Battle);

	// 던전에 방 넣기
	vector<shared_ptr<CDungeonRoom>> Dungeon1_v;
	Dungeon1_v.push_back(m_pRoom1);
	Dungeon1_v.push_back(m_pRoom2);
	Dungeon1_v.push_back(m_pRoom3);
	Dungeon1_v.push_back(m_pRoom4);
	m_pWealdDungeon->PushDungeonRoomVector(Dungeon1_v);

	// 던전 object들 위치 잡아놓기
	// 3번 방
	pRoom3_Battle->FormBattlePosition(Room3_v2, Room3_v3,
		-PI / 2.f, -PI / 2.f, _vec3(WEALD_WALLSIZEX + WEALD_PATHSIZEX + 10.f, 3.f, WEALD_WALLSIZEX * 14.f + 4.f));

	m_pWealdDungeon->CurrentRoom(1);

	// Layer에 GameObject 넣기
	m_pLayer->CreateGameObject(L"Obj_BrigandCutthroat", m_pBrigandCutthroat_1);
	m_pLayer->CreateGameObject(L"Obj_BrigandCutthroat", m_pBrigandCutthroat_2);
	m_pLayer->CreateGameObject(L"Obj_BrigandBloodletter", m_pBrigandBloodletter1);
	m_pLayer->CreateGameObject(L"Obj_BrigandFusilier", m_pBrigandFusilier_1);
	//m_pLayer->CreateGameObject(L"Obj_BoneDefender", m_pBoneDefender1);
	//m_pLayer->CreateGameObject(L"Obj_BoneCourtier", m_pBoneCourtier_1);
	//m_pLayer->CreateGameObject(L"Obj_BoneSoldier", m_pBoneSoldier_1);
	//m_pLayer->CreateGameObject(L"Obj_BoneSoldier", m_pBoneSoldier_2);
	m_pLayer->CreateGameObject(L"Obj_ShieldBreaker", m_pSheldBreaker1);
	//m_pLayer->CreateGameObject(L"Obj_ShieldBreaker", m_pSheldBreaker2);
	//m_pLayer->CreateGameObject(L"Obj_ShieldBreaker", m_pSheldBreaker3);
	//m_pLayer->CreateGameObject(L"Obj_ShieldBreaker", m_pSheldBreaker4);
	m_pLayer->CreateGameObject(L"Obj_Highwayman", m_pHighwayman);
	m_pLayer->CreateGameObject(L"Obj_Jester", m_pJester);
	m_pLayer->CreateGameObject(L"Obj_Vestal", m_pVestal);


//PlayerObj
	shared_ptr<CGameObject> m_pPlayerHand = make_shared<CPlayerHand>(m_pGraphicDev);
	m_pLayer->CreateGameObject(L"Obj_PlayerHand", m_pPlayerHand);
	(dynamic_pointer_cast<CPlayer>(m_pPlayer))->SetPlayerHand(dynamic_pointer_cast<CPlayerHand>(m_pPlayerHand));

	shared_ptr<CGameObject> m_pItem = make_shared<CItem>(m_pGraphicDev);
	m_pItem->SetColliding(true);
	m_pLayer->CreateGameObject(L"Obj_TestItem", m_pItem);
	dynamic_pointer_cast<CItem>(m_pItem)->SetDropItemInfo({ 4.f, 1.f, 6.f }, L"Player_Item_Antivenom");


	dynamic_pointer_cast<CLayer>(m_pLayer)->AwakeLayer();

	return S_OK;
}

HRESULT CWeald_Dungeon::Ready_Layer_UI(tstring pLayerTag)
{
	shared_ptr<CLayer> m_pLayer = make_shared<CLayer>();
	m_mapLayer.insert({ pLayerTag, m_pLayer });

	shared_ptr<CGameObject> m_pInventory = make_shared<CInventory>(m_pGraphicDev);
	m_pLayer->CreateGameObject(L"Obj_UI", m_pInventory);
	
	dynamic_pointer_cast<CPlayer>(CGameMgr::GetInstance()->GetPlayer())->SetInventory(dynamic_pointer_cast<CInventory>(m_pInventory));
	dynamic_pointer_cast<CLayer>(m_pLayer)->AwakeLayer();

	return S_OK;
}
