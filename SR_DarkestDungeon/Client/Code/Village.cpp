#include"pch.h"
#include "Village.h"

#include "Terrain.h"
#include "Wall.h"
#include "SkyBox.h"

#include "Player.h"
#include "Outside.h"

#include "Inside.h"
#include "Tervarn.h"
#include "Guild.h"

#include "Layer.h"
#include "DynamicCamera.h"
#include "StaticCamera.h"
#include "InteractionObj.h"
#include "EnvironmentObj.h"
#include "Item.h"
#include "Inventory.h"
#include "UIMgr.h"
#include"GoodsUI.h"
#include "TavernUI.h"

#include "Vestal.h"
#include "Jester.h"
#include "Highwayman.h"
#include "ShieldBreaker.h"
#include"ResourceMgr.h"
#include"CameraMgr.h"

#include"Export_Utility.h"
#include"SoundMgr.h"

CVillage::CVillage(LPDIRECT3DDEVICE9 pGraphicDev)
	:CScene(pGraphicDev)
{
}

CVillage::~CVillage()
{
}

HRESULT CVillage::ReadyScene()
{
	CResourceMgr::GetInstance()->VillageTextureLoad();
	CCameraMgr::GetInstance()->RemoveMainCamera();

	Ready_Layer_Environment(L"Layer_3_Environment");
	Ready_Layer_SkyBox(L"Layer_1_SkyBox");
	Ready_Layer_GameObject(L"Layer_4_GameObj");
	Ready_Layer_UI(L"Layer_2_UI");
	Ready_Layer_Camera(L"Layer_5_Camera");

	for (auto& iter : m_mapLayer) {
		//GameComponenet Setting
		iter.second->ReadyLayer();
	}

	CSoundMgr::GetInstance()->PlayBGM(L"Mus_Town_Base.wav", 1.f);

	return __super::ReadyScene();
}

_int CVillage::UpdateScene(const _float& fTimeDelta)
{
	int iExit;
	iExit = __super::UpdateScene(fTimeDelta);

	KeyInput();

	return iExit;
}

void CVillage::LateUpdateScene()
{
	__super::LateUpdateScene();

}

void CVillage::RenderScene()
{
}

void CVillage::KeyInput()
{
	if (GetAsyncKeyState('V') & 0x8000) {
		dynamic_pointer_cast<CTransform>(CGameMgr::GetInstance()->GetPlayer()->GetComponent(L"Com_Transform", ID_DYNAMIC))->SetPosition(VILLAGE_TILESIZE * (VILLAGE_TILECNT - 1) / 2.f + 3.5f, 0.f, 0.f);
	}
}

HRESULT CVillage::Ready_Layer_Environment(tstring pLayerTag)
{
	shared_ptr<CLayer> m_pLayer = make_shared<CLayer>();
	m_mapLayer.insert({ pLayerTag, m_pLayer });

	shared_ptr<CGameObject> m_pTerrain = make_shared<CTerrain>(m_pGraphicDev, L"Village_FloorTexture", ETerrainType::VILLAGE);
	m_pLayer->CreateGameObject(L"Obj_Terrain", m_pTerrain);

	shared_ptr<CGameObject> m_pWall;

	// 입구 지점 양 옆 건물
	{
		m_pWall = make_shared<CEnvironmentObj>(m_pGraphicDev, L"Village_Wall1_Texture", true, false, 1, false);
		m_pWall->SetScale(_vec3(VILLAGE_WALLSIZE * 2, VILLAGE_WALLSIZE * 1.5f, 1));
		m_pWall->SetPos(_vec3(50.f, m_pWall->GetScale().y / 6.f * 5.f, m_pWall->GetScale().x));
		m_pWall->SetAngle(_vec3(0.f, PI / 2, 0.f));
		m_pLayer->CreateGameObject(L"OBJ_Back", m_pWall);

		m_pWall = make_shared<CEnvironmentObj>(m_pGraphicDev, L"Village_Wall1_Texture", true, false, 1, false);
		m_pWall->SetScale(_vec3(VILLAGE_WALLSIZE * 2, VILLAGE_WALLSIZE * 1.5f, 1));
		m_pWall->SetPos(_vec3(65.f, m_pWall->GetScale().y / 6.f * 5.f, m_pWall->GetScale().x));
		m_pWall->SetAngle(_vec3(0.f, PI / 2, 0.f));
		m_pLayer->CreateGameObject(L"OBJ_Back", m_pWall);
	}

	// 외곽 건물 그림자
	for (int i = 0; i < 6; i++)
	{
		m_pWall = make_shared<CWall>(m_pGraphicDev, L"Com_Village_Back_Texture", 3, true);
		m_pWall->SetScale(_vec3(VILLAGE_WALLSIZE * 2, VILLAGE_WALLSIZE * 5 * (rand() % 10 + 6) / 10, 1));
		m_pWall->SetPos(_vec3(VILLAGE_TILESIZE * i + VILLAGE_TILESIZE + (VILLAGE_TILESIZE - 5) * i, m_pWall->GetScale().y / 2 + 3, (VILLAGE_TILESIZE - 1) * (VILLAGE_TILESIZE - 1) - 2));
		m_pWall->SetAngle(_vec3(0.f, PI / 2.f, 0.f));
		m_pLayer->CreateGameObject(L"OBJ_Back", m_pWall);
	}

	// 성벽
	{
		// 오른쪽
		{
			_vec3 prevPos = { VILLAGE_TILESIZE * (VILLAGE_TILECNT - 1) / 2.f + VILLAGE_TILESIZE, VILLAGE_WALLSIZE * 5.f / 3.f, VILLAGE_TILESIZE * 2.f };

			for (int i = 0; i < 12; ++i)
			{
				m_pWall = make_shared<CEnvironmentObj>(m_pGraphicDev, L"Village_Wall1_Texture", false, false, 1, false);
				m_pWall->SetScale(_vec3(VILLAGE_WALLSIZE * 2.f, VILLAGE_WALLSIZE * 2.f, 1));
				m_pWall->SetAngle(_vec3(0.f, -PI / 11.f * i, 0.f));

				_float IncreaseX = VILLAGE_WALLSIZE * 2.f * cos(PI / 11.f * i);
				_float IncreaseY = VILLAGE_WALLSIZE * 2.f * sin(PI / 11.f * i);
				m_pWall->SetPos({ prevPos.x + IncreaseX, m_pWall->GetScale().y / 6.f * 5.f, prevPos.z + IncreaseY });

				prevPos += {IncreaseX, 0.f, IncreaseY};

				m_pLayer->CreateGameObject(L"OBJ_CastleWall", m_pWall);
			}
		}


		// 왼쪽
		{
			_vec3 prevPos = { VILLAGE_TILESIZE * (VILLAGE_TILECNT - 1) / 2.f - 3.f, VILLAGE_WALLSIZE * 5.f / 3.f, VILLAGE_TILESIZE * 2.f };

			for (int i = 0; i < 12; ++i)
			{
				m_pWall = make_shared<CEnvironmentObj>(m_pGraphicDev, L"Village_Wall1_Texture", false, false, 1, false);
				m_pWall->SetScale(_vec3(VILLAGE_WALLSIZE * 2.f, VILLAGE_WALLSIZE * 2.f, 1));
				m_pWall->SetAngle(_vec3(0.f, PI / 11.f * i, 0.f));

				_float IncreaseX = VILLAGE_WALLSIZE * 2.f * cos(PI / 11.f * i);
				_float IncreaseY = VILLAGE_WALLSIZE * 2.f * sin(PI / 11.f * i);
				m_pWall->SetPos({ prevPos.x - IncreaseX, m_pWall->GetScale().y / 6.f * 5.f, prevPos.z + IncreaseY });

				prevPos += {-IncreaseX, 0.f, IncreaseY};

				m_pLayer->CreateGameObject(L"OBJ_CastleWall", m_pWall);
			}

			_vec3 temp = m_pWall->GetPos();
			m_pWall->SetPos({ temp.x, temp.y, temp.z - 0.1f });
		}
	}

	// 마을 입구
	{
		m_pWall = make_shared<CEnvironmentObj>(m_pGraphicDev, L"Village_Entrance_Texture", false, false, 1, false);
		m_pWall->SetScale(_vec3(VILLAGE_WALLSIZE * 1.5f, VILLAGE_WALLSIZE * 1.5f, 1.f));
		m_pWall->SetPos({ VILLAGE_TILESIZE * (VILLAGE_TILECNT - 1) / 2.f + 3.5f, m_pWall->GetScale().y / 2 + m_pWall->GetScale().y / 3, VILLAGE_TILESIZE * 2.f - 0.1f });
		m_pLayer->CreateGameObject(L"OBJ_Entrance", m_pWall);
	}


	// 던전 입구
	{
		m_pWall = make_shared<CEnvironmentObj>(m_pGraphicDev, L"Village_Portal_Texture", false, false, 1, false);
		m_pWall->SetScale(_vec3(VILLAGE_WALLSIZE, VILLAGE_WALLSIZE * 1.2f, 1.f));
		m_pWall->SetPos({ VILLAGE_TILESIZE * (VILLAGE_TILECNT - 1) / 2.f + 3.5f, m_pWall->GetScale().y / 2 + m_pWall->GetScale().y / 3, 0.f });
		m_pLayer->CreateGameObject(L"OBJ_Portal", m_pWall);
	}

	// 마을 건물 내부
	{
		//여관 내부
		shared_ptr<CGameObject> m_pTervarnInside = make_shared<CTervarn>(m_pGraphicDev);
		m_pTervarnInside->SetPos({ 50.f - VILLAGE_TILESIZE * 3 / 2, -100.f, 0.f });
		//m_pTervarnInside->SetScale({ 20.f, 30.f, 1.f });
		m_pLayer->CreateGameObject(L"Obj_TervarnInside", m_pTervarnInside);


		//훈련소 내부
		shared_ptr<CGameObject> m_pGuildInside = make_shared<CGuild>(m_pGraphicDev);
		m_pGuildInside->SetPos({ 0.f - VILLAGE_TILESIZE * 3 / 2, -200.f, 100.f });
		//m_pTervarnInside->SetScale({ 20.f, 30.f, 1.f });
		m_pLayer->CreateGameObject(L"Obj_GuildInside", m_pGuildInside);
	}

	//가장 최하위 순서에 돌려줄 것
	dynamic_pointer_cast<CLayer>(m_pLayer)->AwakeLayer();


	return S_OK;
}

HRESULT CVillage::Ready_Layer_SkyBox(tstring pLayerTag)
{
	shared_ptr<CLayer> m_pLayer = make_shared<CLayer>();
	m_mapLayer.insert({ pLayerTag, m_pLayer });


	shared_ptr<CGameObject> m_pSkyBox = make_shared<CSkyBox>(m_pGraphicDev, L"Village_SkyBoxTexture");
	m_pLayer->CreateGameObject(L"Obj_Village_SkyBox", m_pSkyBox);

	dynamic_pointer_cast<CLayer>(m_pLayer)->AwakeLayer();

	return S_OK;
}

HRESULT CVillage::Ready_Layer_Camera(tstring pLayerTag)
{
	shared_ptr<CLayer> m_pLayer = make_shared<CLayer>();
	m_mapLayer.insert({ pLayerTag, m_pLayer });

	// Camera
	shared_ptr<CGameObject> m_pCamera = make_shared<CStaticCamera>(m_pGraphicDev);
	m_pLayer->CreateGameObject(L"OBJ_Camera", m_pCamera);

	CCameraMgr::GetInstance()->SetMainCamera(dynamic_pointer_cast<CStaticCamera>(m_pCamera));
	CCameraMgr::GetInstance()->SetVillageMode();

	dynamic_pointer_cast<CLayer>(m_pLayer)->AwakeLayer();

	return S_OK;
}

HRESULT CVillage::Ready_Layer_GameObject(tstring pLayerTag)
{
	shared_ptr<CLayer> m_pLayer = make_shared<CLayer>();
	m_mapLayer.insert({ pLayerTag, m_pLayer });

	//플레이어
	shared_ptr<CGameObject> m_pPlayer;

	if (m_pPlayer = dynamic_pointer_cast<CPlayer>(CGameMgr::GetInstance()->GetPlayer()))
	{
		// 던전에서 시작해서 넘어온 경우
		m_pPlayer->SetPos({ VILLAGE_TILESIZE * (VILLAGE_TILECNT - 1) / 2.f + 3.5f, 0.f, 0.f });
		dynamic_pointer_cast<CTransform>(m_pPlayer->GetComponent(L"Com_Transform", ID_DYNAMIC))->SetPosition(m_pPlayer->GetPos().x, m_pPlayer->GetPos().y, m_pPlayer->GetPos().z);

		for (auto& iter : *dynamic_pointer_cast<CPlayer>(m_pPlayer)->GetHeroVec())
		{
			m_pLayer->CreateGameObject(dynamic_pointer_cast<CHero>(iter)->GetObjKey(), iter);
		}
	}

	else
	{
		// 마을에서부터 시작한 경우
		m_pPlayer = make_shared<CPlayer>(m_pGraphicDev);
		CGameMgr::GetInstance()->SetPlayer(m_pPlayer);
		m_pPlayer->SetPos({ VILLAGE_TILESIZE * (VILLAGE_TILECNT - 1) / 2.f + 3.5f, 0.f, 0.f });


		//// 테스트 (돈, 가보 초기 세팅)
		//{
		//	dynamic_pointer_cast<CPlayer>(m_pPlayer)->InitGold(2000);
		//	dynamic_pointer_cast<CPlayer>(m_pPlayer)->InitHeirloom(3);
		//}
	}

		// 테스트 (돈, 가보 초기 세팅)
		{
			dynamic_pointer_cast<CPlayer>(m_pPlayer)->InitGold(2000);
			dynamic_pointer_cast<CPlayer>(m_pPlayer)->InitHeirloom(3);
		}

		shared_ptr<CGameObject> m_pPlayerHand = make_shared<CPlayerHand>(m_pGraphicDev);
		m_pLayer->CreateGameObject(L"Obj_PlayerHand", m_pPlayerHand);
		(dynamic_pointer_cast<CPlayer>(m_pPlayer))->SetPlayerHand(dynamic_pointer_cast<CPlayerHand>(m_pPlayerHand));

		m_pLayer->CreateGameObject(L"Obj_Player", m_pPlayer);
		dynamic_pointer_cast<CPlayer>(m_pPlayer)->SetInDungeon(false);

	// 영웅 테스트
	 {
		//shared_ptr<CGameObject> m_pVestal = make_shared<CVestal>(m_pGraphicDev);
		//m_pLayer->CreateGameObject(L"Obj_Vestal", m_pVestal);
		//m_pVestal->SetPos({ 10.f, 3.f, 20.f });

		//dynamic_pointer_cast<CPlayer>(m_pPlayer)->AddHero(m_pVestal);

		//shared_ptr<CGameObject> m_pHighwayman = make_shared<CHighwayman>(m_pGraphicDev);
		//m_pLayer->CreateGameObject(L"Obj_Highwayman", m_pHighwayman);
		//m_pHighwayman->SetPos({ 20.f, 3.f, 20.f });

		//dynamic_pointer_cast<CPlayer>(m_pPlayer)->AddHero(m_pHighwayman);


		//shared_ptr<CGameObject> m_pJester = make_shared<CJester>(m_pGraphicDev);
		//m_pLayer->CreateGameObject(L"Obj_Jestal", m_pJester);
		//m_pJester->SetPos({ 15.f, 3.f, 20.f });
		//dynamic_pointer_cast<CPlayer>(m_pPlayer)->AddHero(m_pJester);


		//shared_ptr<CGameObject> m_pShieldBreaker = make_shared<CShieldBreaker>(m_pGraphicDev);
		//m_pLayer->CreateGameObject(L"Obj_ShieldBreaker", m_pShieldBreaker);
		//m_pShieldBreaker->SetPos({ 25.f, 3.f, 20.f });
		//dynamic_pointer_cast<CPlayer>(m_pPlayer)->AddHero(m_pShieldBreaker);
	}

	 for (auto& iter : *dynamic_pointer_cast<CPlayer>(m_pPlayer)->GetHeroVec())
	 {
		 dynamic_pointer_cast<CHero>(iter)->SetInDungeon(false);
	 }

	// 여관
	{

		// 여관 외부
		shared_ptr<CGameObject> m_pTervarnOutside = make_shared<COutside>(m_pGraphicDev, EFacilityType::TERVARN);
		m_pTervarnOutside->SetPos({ 45.f, 0.f, 70.f });
		m_pTervarnOutside->SetScale({ 20.f, 30.f, 1.f });
		m_pLayer->CreateGameObject(L"Obj_TervarnOutside", m_pTervarnOutside);

		// 훈련소 외부
		shared_ptr<CGameObject> m_pGuildOutside = make_shared<COutside>(m_pGraphicDev, EFacilityType::GUILD);
		m_pGuildOutside->SetPos({ 90.f, 0.f, 85.f });
		m_pGuildOutside->SetScale({ 20.f, 30.f, 1.f });
		m_pGuildOutside->SetAngle({ 0.f, 10.f, 0.f });
		m_pLayer->CreateGameObject(L"Obj_GuildOutside", m_pGuildOutside);

		// 역마차 외부
		shared_ptr<CGameObject> m_pStageCoachOutside = make_shared<COutside>(m_pGraphicDev, EFacilityType::STAGECOACH);
		m_pStageCoachOutside->SetPos({ 60.f, 0.f, 55.f });
		m_pStageCoachOutside->SetScale({ 7.f, 5.f, 1.f });
		m_pLayer->CreateGameObject(L"Obj_StageCoachOutside", m_pStageCoachOutside);

		// 상점 외부
		shared_ptr<CGameObject> m_pStoreOutside = make_shared<COutside>(m_pGraphicDev, EFacilityType::STORE);
		m_pStoreOutside->SetPos({ 75.f, 0.f, 33.f });
		m_pStoreOutside->SetScale({ 7.f, 7.f, 1.f });
		m_pStoreOutside->SetAngle({ 0.f, 50.f, 0.f });
		m_pLayer->CreateGameObject(L"Obj_StoreOutside", m_pStoreOutside);
	}

	// 훈련소
	{

	}

	// 상점
	{

	}

	// 역마차
	{

	}

	dynamic_pointer_cast<CLayer>(m_pLayer)->AwakeLayer();

	return S_OK;
}



HRESULT CVillage::Ready_Layer_UI(tstring pLayerTag)
{
	shared_ptr<CLayer> m_pLayer = make_shared<CLayer>();
	m_mapLayer.insert({ pLayerTag, m_pLayer });

	shared_ptr<CGameObject> m_pInventory = make_shared<CInventory>(m_pGraphicDev);
	m_pLayer->CreateGameObject(L"Obj_UI", m_pInventory);

	CUIMgr::GetInstance()->AddUIObject(L"UI_Inventory", dynamic_pointer_cast<CUIObj>(m_pInventory));

	shared_ptr<CGameObject> pGoods = make_shared<CGoodsUI>(m_pGraphicDev);
	CUIMgr::GetInstance()->AddUIObject(L"Obj_GoodsUI", dynamic_pointer_cast<CUIObj>(pGoods));

	dynamic_pointer_cast<CGoodsUI>(pGoods)->SetInitGold(dynamic_pointer_cast<CPlayer>(CGameMgr::GetInstance()->GetPlayer())->GetGold());
	dynamic_pointer_cast<CGoodsUI>(pGoods)->SetInitHeirloom(dynamic_pointer_cast<CPlayer>(CGameMgr::GetInstance()->GetPlayer())->GetHeirloom());

	m_pLayer->CreateGameObject(L"Obj_GoodsUI", pGoods);

	shared_ptr<CGameObject> pTavernUI = make_shared<CTavernUI>(m_pGraphicDev);

	// 영웅 주점 UI
	pTavernUI = make_shared<CTavernUI>(m_pGraphicDev);
	m_pLayer->CreateGameObject(L"Obj_TavernUI", pTavernUI);

	CUIMgr::GetInstance()->AddUIObject(L"Obj_TavernUI", dynamic_pointer_cast<CUIObj>(pTavernUI));



	dynamic_pointer_cast<CPlayer>(CGameMgr::GetInstance()->GetPlayer())->SetInventory(dynamic_pointer_cast<CInventory>(m_pInventory));
	dynamic_pointer_cast<CPlayer>(CGameMgr::GetInstance()->GetPlayer())->SetTavernUI(dynamic_pointer_cast<CTavernUI>(pTavernUI));

	dynamic_pointer_cast<CLayer>(m_pLayer)->AwakeLayer();

	return S_OK;
}
