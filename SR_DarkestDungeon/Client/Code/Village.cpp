#include"pch.h"
#include "Village.h"

#include "Terrain.h"
#include "Wall.h"
#include "SkyBox.h"

#include "Player.h"
#include "Outside.h"
#include "Inside.h"

#include "Layer.h"
#include "DynamicCamera.h"
#include "StaticCamera.h"
#include "InteractionObj.h"

#include "Vestal.h"
#include "Jester.h"
#include "Highwayman.h"
#include "ShieldBreaker.h"
#include"ResourceMgr.h"

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
	CResourceMgr::GetInstance()->VillageTextureLoad();

	Ready_Layer_Environment(L"Layer_3_Environment");
	Ready_Layer_SkyBox(L"Layer_1_SkyBox");
	Ready_Layer_GameObject(L"Layer_4_GameObj");

	for (auto& iter : m_mapLayer) {
		//GameComponenet Setting
		iter.second->ReadyLayer();
	}

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
	shared_ptr<CGameObject> m_pCamera = make_shared<CStaticCamera>(m_pGraphicDev);
	m_pLayer->CreateGameObject(L"Obj_Camera", m_pCamera);

	shared_ptr<CGameObject> m_pTerrain = make_shared<CTerrain>(m_pGraphicDev, L"Village_FloorTexture");
	m_pLayer->CreateGameObject(L"Obj_Terrain", m_pTerrain);

	shared_ptr<CGameObject> m_pWall;

	// 가장자리 나무
	/*for (int i = 0; i < 9; i++)
	{
		m_pWall = make_shared<CWall>(m_pGraphicDev, L"Com_Village_Wall_Texture");
		m_pWall->SetPos(_vec3(VILLAGE_TILESIZE * i + VILLAGE_TILESIZE, VILLAGE_WALLSIZEUPY * 2 + 1, (VILLAGE_TILESIZE - 1) * (VILLAGE_TILESIZE - 1) - 2));
		m_pWall->SetAngle(_vec3(0.f, PI / 2.f, 0.f));
		m_pWall->SetScale(_vec3(VILLAGE_WALLSIZE * 2, VILLAGE_WALLSIZE * 2, 1));
		m_pLayer->CreateGameObject(L"OBJ_Wall", m_pWall);
	}*/


	// 외곽 건물 그림자
	for (int i = 0; i < 6; i++)
	{
		m_pWall = make_shared<CWall>(m_pGraphicDev, L"Com_Village_Back_Texture");
		m_pWall->SetScale(_vec3(VILLAGE_WALLSIZE * 2, VILLAGE_WALLSIZE * 2 * (rand() % 10 + 6) / 10, 1));
		m_pWall->SetPos(_vec3(VILLAGE_TILESIZE * i + VILLAGE_TILESIZE + (VILLAGE_TILESIZE - 5) * i, m_pWall->GetScale().y / 2  + 1, (VILLAGE_TILESIZE - 1) * (VILLAGE_TILESIZE - 1) - 2));
		m_pWall->SetAngle(_vec3(0.f, PI / 2.f, 0.f));
		m_pLayer->CreateGameObject(L"OBJ_Back", m_pWall);
	}

	// 마을 입구


	//for (int i = 0; i < 2; i++)
	//{
	//	m_pWall = make_shared<CWall>(m_pGraphicDev, L"Com_Village_Wall_Texture");
	//	m_pWall->SetPos(_vec3(WEALD_WALLSIZEX * 2.f + WEALD_PATHSIZEX + WEALD_WALLSIZEX * 2.f, WEALD_WALLSIZEUPY, WEALD_WALLSIZEX / 2.f + WEALD_WALLSIZEX * i));
	//	m_pWall->SetAngle(_vec3(0.f, 0.f, 0.f));
	//	m_pLayer->CreateGameObject(L"OBJ_Wall", m_pWall);
	//}

	//for (int i = 0; i < 1; i++)
	//{
	//	m_pWall = make_shared<CWall>(m_pGraphicDev, L"Com_Village_Wall_Texture");
	//	m_pWall->SetPos(_vec3(WEALD_WALLSIZEX * 2.f + WEALD_WALLSIZEX / 2.f + WEALD_WALLSIZEX * i, WEALD_WALLSIZEUPY, WEALD_WALLSIZEX * 2.f));
	//	m_pWall->SetAngle(_vec3(0.f, PI / 2.f, 0.f));
	//	m_pLayer->CreateGameObject(L"OBJ_Wall", m_pWall);
	//}

	//for (int i = 0; i < 1; i++)
	//{
	//	m_pWall = make_shared<CWall>(m_pGraphicDev, L"Com_Village_Wall_Texture");
	//	m_pWall->SetPos(_vec3(WEALD_WALLSIZEX * 2.f + WEALD_PATHSIZEX + WEALD_WALLSIZEX * 1.5f + WEALD_WALLSIZEX * i, WEALD_WALLSIZEUPY, WEALD_WALLSIZEX * 2.f));
	//	m_pWall->SetAngle(_vec3(0.f, PI / 2.f, 0.f));
	//	m_pLayer->CreateGameObject(L"OBJ_Wall", m_pWall);
	//}

	//// 통로 1 벽
	//for (int i = 0; i < 10; i++)
	//{
	//	m_pWall = make_shared<CWall>(m_pGraphicDev, L"Com_Village_Wall_Texture");
	//	m_pWall->SetPos(_vec3(WEALD_WALLSIZEX * 2.f + WEALD_WALLSIZEX * 1.f, WEALD_WALLSIZEUPY, WEALD_WALLSIZEX * 2.f + WEALD_WALLSIZEX / 2.f + WEALD_WALLSIZEX * i));
	//	m_pWall->SetAngle(_vec3(0.f, 0.f, 0.f));
	//	m_pLayer->CreateGameObject(L"OBJ_Wall", m_pWall);
	//}

	//for (int i = 0; i < 10; i++)
	//{
	//	m_pWall = make_shared<CWall>(m_pGraphicDev, L"Com_Village_Wall_Texture");
	//	m_pWall->SetPos(_vec3(WEALD_WALLSIZEX * 2.f + WEALD_PATHSIZEX + WEALD_WALLSIZEX * 1.f, WEALD_WALLSIZEUPY, WEALD_WALLSIZEX * 2.f + WEALD_WALLSIZEX / 2.f + WEALD_WALLSIZEX * i));
	//	m_pWall->SetAngle(_vec3(0.f, 0.f, 0.f));
	//	m_pLayer->CreateGameObject(L"OBJ_Wall", m_pWall);
	//}

	//// 방 2 벽
	//for (int i = 0; i < 4; i++)
	//{
	//	m_pWall = make_shared<CWall>(m_pGraphicDev, L"Com_Village_Wall_Texture");
	//	m_pWall->SetPos(_vec3(WEALD_WALLSIZEX * 1.f, WEALD_WALLSIZEUPY, WEALD_WALLSIZEX / 2.f + WEALD_WALLSIZEX * 12.f + WEALD_WALLSIZEX * i));
	//	m_pWall->SetAngle(_vec3(0.f, 0.f, 0.f));
	//	m_pLayer->CreateGameObject(L"OBJ_Wall", m_pWall);
	//}

	//for (int i = 0; i < 4; i++)
	//{
	//	m_pWall = make_shared<CWall>(m_pGraphicDev, L"Com_Village_Wall_Texture");
	//	m_pWall->SetPos(_vec3(WEALD_WALLSIZEX * 3.f + WEALD_PATHSIZEX + WEALD_WALLSIZEX * 2.f, WEALD_WALLSIZEUPY, WEALD_WALLSIZEX / 2.f + WEALD_WALLSIZEX * 12.f + WEALD_WALLSIZEX * i));
	//	m_pWall->SetAngle(_vec3(0.f, 0.f, 0.f));
	//	m_pLayer->CreateGameObject(L"OBJ_Wall", m_pWall);
	//}

	//for (int i = 0; i < 2; i++)
	//{
	//	m_pWall = make_shared<CWall>(m_pGraphicDev, L"Com_Village_Wall_Texture");
	//	m_pWall->SetPos(_vec3(WEALD_WALLSIZEX * 1.5f + WEALD_WALLSIZEX * i, WEALD_WALLSIZEUPY, WEALD_WALLSIZEX * 12.f));
	//	m_pWall->SetAngle(_vec3(0.f, PI / 2.f, 0.f));
	//	m_pLayer->CreateGameObject(L"OBJ_Wall", m_pWall);
	//}

	//for (int i = 0; i < 2; i++)
	//{
	//	m_pWall = make_shared<CWall>(m_pGraphicDev, L"Com_Village_Wall_Texture");
	//	m_pWall->SetPos(_vec3(WEALD_WALLSIZEX * 3.5f + WEALD_PATHSIZEX + WEALD_WALLSIZEX * i, WEALD_WALLSIZEUPY, WEALD_WALLSIZEX * 12.f));
	//	m_pWall->SetAngle(_vec3(0.f, PI / 2.f, 0.f));
	//	m_pLayer->CreateGameObject(L"OBJ_Wall", m_pWall);
	//}

	//for (int i = 0; i < 6; i++)
	//{
	//	m_pWall = make_shared<CWall>(m_pGraphicDev, L"Com_Village_Wall_Texture");
	//	m_pWall->SetPos(_vec3(WEALD_WALLSIZEX * 1.5f + WEALD_WALLSIZEX * i, WEALD_WALLSIZEUPY, WEALD_WALLSIZEX * 16.f));
	//	m_pWall->SetAngle(_vec3(0.f, PI / 2.f, 0.f));
	//	m_pLayer->CreateGameObject(L"OBJ_Wall", m_pWall);
	//}

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

HRESULT CVillage::Ready_Layer_GameObject(tstring pLayerTag)
{
	shared_ptr<CLayer> m_pLayer = make_shared<CLayer>();
	m_mapLayer.insert({ pLayerTag, m_pLayer });

	//플레이어
	shared_ptr<CGameObject> m_pPlayer = make_shared<CPlayer>(m_pGraphicDev);
	m_pLayer->CreateGameObject(L"Obj_Player", m_pPlayer);

	CGameMgr::GetInstance()->SetPlayer(m_pPlayer);
	shared_ptr<CGameObject> m_pPlayerHand = make_shared<CPlayerHand>(m_pGraphicDev);
	m_pLayer->CreateGameObject(L"Obj_PlayerHand", m_pPlayerHand);
	(dynamic_pointer_cast<CPlayer>(m_pPlayer))->SetPlayerHand(dynamic_pointer_cast<CPlayerHand>(m_pPlayerHand));

	// 영웅 테스트
	/*shared_ptr<CGameObject> m_pVestal = make_shared<CVestal>(m_pGraphicDev);
	m_pLayer->CreateGameObject(L"Obj_Vestal", m_pVestal);
	m_pVestal->SetPos({ 10.f, 3.f, 20.f });

	shared_ptr<CGameObject> m_pJester = make_shared<CJester>(m_pGraphicDev);
	m_pLayer->CreateGameObject(L"Obj_Jestal", m_pJester);
	m_pJester->SetPos({ 15.f, 3.f, 20.f });

	shared_ptr<CGameObject> m_pHighwayman = make_shared<CHighwayman>(m_pGraphicDev);
	m_pLayer->CreateGameObject(L"Obj_Highwayman", m_pHighwayman);
	m_pHighwayman->SetPos({ 20.f, 3.f, 20.f });

	shared_ptr<CGameObject> m_pShieldBreaker = make_shared<CShieldBreaker>(m_pGraphicDev);
	m_pLayer->CreateGameObject(L"Obj_ShieldBreaker", m_pShieldBreaker);
	m_pShieldBreaker->SetPos({ 25.f, 3.f, 20.f });*/

	// 여관
	{

		// 여관 외부
		shared_ptr<CGameObject> m_pTervarnOutside = make_shared<COutside>(m_pGraphicDev, EFacilityType::TERVARN);
		m_pTervarnOutside->SetPos({ 30.f, 0.f, 40.f });
		m_pTervarnOutside->SetScale({ 20.f, 30.f, 1.f });
		m_pLayer->CreateGameObject(L"Obj_TervarnOutside", m_pTervarnOutside);

		// 여관 내부
		/*shared_ptr<CGameObject> m_pTervarnInside = make_shared<CInside>(m_pGraphicDev, L"Tervarn_Inside");
		m_pTervarnInside->SetPos({ 10000.f, 0.f, 0.f });
		m_pTervarnInside->SetScale({ 20.f, 30.f, 1.f });
		m_pLayer->CreateGameObject(L"Obj_TervarnInside", m_pTervarnInside);*/
	}

	dynamic_pointer_cast<CLayer>(m_pLayer)->AwakeLayer();

	return S_OK;
}

HRESULT CVillage::Ready_Layer_UI(tstring pLayerTag)
{
	return S_OK;
}
