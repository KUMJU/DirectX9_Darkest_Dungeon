#include "pch.h"
#include "Guild.h"

#include "Export_Utility.h"
#include "Export_System.h"

#include "Terrain.h"
#include "Wall.h"
#include "EnvironmentObj.h"
#include "GuildNPC.h"
#include "Trigger.h"

CGuild::CGuild(LPDIRECT3DDEVICE9 pGraphicDev) : CInside(pGraphicDev)
{
}

CGuild::CGuild(const CGuild& rhs) : CInside(rhs)
{
}

CGuild::~CGuild()
{
}

HRESULT CGuild::ReadyGameObject()
{
	// Environment
	{
		// 바닥
		{
			shared_ptr<CGameObject> m_pTerrain = make_shared<CTerrain>(m_pGraphicDev, L"Inside_FloorTexture", ETerrainType::VILLAGE_INSIDE);
			m_pTerrain->SetPos(m_vPos);

			m_vecGameObject.push_back(m_pTerrain);
		}

		// 벽
		{
			shared_ptr<CGameObject> m_pWall;

			// 앞쪽
			for (int i = 0; i < 5; ++i)
			{
				m_pWall = make_shared<CWall>(m_pGraphicDev, L"Wood_Wall_Texture", 1, false);
				m_pWall->SetScale(_vec3(VILLAGE_WALLSIZE * 3 / 5, VILLAGE_WALLSIZE * 3 / 2, 1));
				m_pWall->SetPos(_vec3(m_vPos.x + VILLAGE_TILESIZE * i / 5 * 3 + VILLAGE_TILESIZE / 10 * 3, m_vPos.y + m_pWall->GetScale().y - 1, m_vPos.z));
				m_pWall->SetAngle(_vec3(0.f, PI / 2.f, 0.f));

				m_vecGameObject.push_back(m_pWall);
			}

			// 뒷쪽
			for (int i = 0; i < 5; ++i)
			{
				m_pWall = make_shared<CWall>(m_pGraphicDev, L"Wood_Wall_Texture", 1, false);
				m_pWall->SetScale(_vec3(VILLAGE_WALLSIZE * 3 / 5, VILLAGE_WALLSIZE * 3 / 2, 1));
				m_pWall->SetPos(_vec3(m_vPos.x + VILLAGE_TILESIZE * i / 5 * 3 + VILLAGE_TILESIZE / 10 * 3, m_vPos.y + m_pWall->GetScale().y - 1, m_vPos.z + VILLAGE_TILESIZE * 3 - 1));
				m_pWall->SetAngle(_vec3(0.f, PI / 2.f, 0.f));

				m_vecGameObject.push_back(m_pWall);
			}

			// 왼쪽
			for (int i = 0; i < 5; ++i)
			{
				m_pWall = make_shared<CWall>(m_pGraphicDev, L"Wood_Wall_Texture", 1, false);
				m_pWall->SetScale(_vec3(VILLAGE_WALLSIZE * 3 / 5, VILLAGE_WALLSIZE * 3 / 2, 1));
				m_pWall->SetPos(_vec3(m_vPos.x, m_vPos.y + m_pWall->GetScale().y - 1, m_vPos.z + VILLAGE_TILESIZE * i / 5 * 3 + VILLAGE_TILESIZE / 10 * 3));
				m_pWall->SetAngle(_vec3(0.f, 0.f, 0.f));

				m_vecGameObject.push_back(m_pWall);
			}

			// 오른쪽
			for (int i = 0; i < 5; ++i)
			{
				m_pWall = make_shared<CWall>(m_pGraphicDev, L"Wood_Wall_Texture", 1, false);
				m_pWall->SetScale(_vec3(VILLAGE_WALLSIZE * 3 / 5, VILLAGE_WALLSIZE * 3 / 2, 1));
				m_pWall->SetPos(_vec3(m_vPos.x + VILLAGE_TILESIZE * 3 - 1, m_vPos.y + m_pWall->GetScale().y - 1, m_vPos.z + VILLAGE_TILESIZE * i / 5 * 3 + VILLAGE_TILESIZE / 10 * 3));
				m_pWall->SetAngle(_vec3(0.f, 0.f, 0.f));

				m_vecGameObject.push_back(m_pWall);
			}
		}

		// 천장
		{
			shared_ptr<CGameObject> m_pTerrain = make_shared<CTerrain>(m_pGraphicDev, L"Inside_CeilingTexture", ETerrainType::VILLAGE_INSIDE);
			m_pTerrain->SetPos({ m_vPos.x, m_vPos.y + 15.f, m_vPos.z });

			m_vecGameObject.push_back(m_pTerrain);
		}
	}

	// GameObject
	{
		// NPC
		{
			shared_ptr<CGameObject> m_pNPC = make_shared<CEnvironmentObj>(m_pGraphicDev, L"Guild_NPC_Texture", true, false, 1, false);
			m_pNPC->SetScale({ 5.f, 5.f, 1.f });
			m_pNPC->SetPos({ m_vPos.x + 2 * VILLAGE_TILESIZE / 2.f - 3.f, m_vPos.y + m_pNPC->GetScale().y + 3.f, m_vPos.z + VILLAGE_TILESIZE * 2.f + 6.f });

			m_vecGameObject.push_back(m_pNPC);
		}

		// 문
		{
			shared_ptr<CGameObject> m_pDoor = make_shared<CEnvironmentObj>(m_pGraphicDev, L"Village_Door_Open", false);
			m_pDoor->SetScale({ 3.f, 7.f, 1.f });
			m_pDoor->SetPos({ m_vPos.x + VILLAGE_TILESIZE * 1.5f, m_vPos.y + m_pDoor->GetScale().y / 2.f + 1, m_vPos.z + 0.1f });

			m_vecGameObject.push_back(m_pDoor);
		}

		// 테이블
		{
			shared_ptr<CGameObject> m_pTable = make_shared<CEnvironmentObj>(m_pGraphicDev, L"Tavern_Bar_Table_Texture", true, false, 1, false);

			m_pTable->SetScale({ 10.f, 7.f, 1.f });
			m_pTable->SetPos({ m_vPos.x + 2 * VILLAGE_TILESIZE / 2.f - 3.f, m_vPos.y + m_pTable->GetScale().y / 2.f + m_pTable->GetScale().y / 3.f, m_vPos.z + VILLAGE_TILESIZE * 2.f + 5.f });

			m_vecGameObject.push_back(m_pTable);
		}

		// 수련인형
		{
			shared_ptr<CGameObject> m_pSandBack = make_shared<CGuildNPC>(m_pGraphicDev);
			m_pSandBack->SetScale({ 8.f, 7.f, 1.f });
			m_pSandBack->SetPos({ m_vPos.x + 2 * VILLAGE_TILESIZE / 2.f + 5.f, m_vPos.y + m_pSandBack->GetScale().y / 2.f + m_pSandBack->GetScale().y / 3.f, m_vPos.z + VILLAGE_TILESIZE + 3.f });

			m_vecGameObject.push_back(m_pSandBack);
		}

		// 창문
		{
			shared_ptr<CGameObject> m_pWindow = make_shared<CEnvironmentObj>(m_pGraphicDev, L"Tavern_Window_Texture", false, false, 1, false);
			m_pWindow->SetScale({ 4.f, 4.f, 1.f });
			m_pWindow->SetPos({ m_vPos.x + 3 * VILLAGE_TILESIZE - 1.1f, m_vPos.y + m_pWindow->GetScale().y + 3.f, m_vPos.z + VILLAGE_TILESIZE * 2 - 5.f });
			m_pWindow->SetAngle({ 0.f, -PI / 2, 0.f });
				
			m_vecGameObject.push_back(m_pWindow);
		}

		// 벽 장식
		{
			shared_ptr<CGameObject> m_pWallDeco = make_shared<CEnvironmentObj>(m_pGraphicDev, L"Guild_WallSword_Texture", false, false, 1, false);
			m_pWallDeco->SetScale({ 3.5f, 3.3f, 1.f });
			m_pWallDeco->SetPos({ m_vPos.x + VILLAGE_TILESIZE * 2.f + 4.5f, m_vPos.y + m_pWallDeco->GetScale().y + 5.f, m_vPos.z + VILLAGE_TILESIZE * 3.f - 1.1f });
			m_pWallDeco->SetAngle({ 0.f, 0.f, 0.f });

			m_vecGameObject.push_back(m_pWallDeco);
		}

		// 이동 트리거
		{
			shared_ptr<CGameObject> m_pTrigger = make_shared<CTrigger>(m_pGraphicDev, L"Guild", true);
			m_pTrigger->SetScale({ 3.f, 3.f, 3.f });
			m_pTrigger->SetPos({ m_vPos.x + VILLAGE_TILESIZE * 2.5f / 5.f * 3.f, m_vPos.y, m_vPos.z });

			m_vecGameObject.push_back(m_pTrigger);
		}

		//	// 창문

		//	// 술통
		//	shared_ptr<CGameObject> m_pBarrel = make_shared<CEnvironmentObj>(m_pGraphicDev, L"Tavern_Barrel_Texture", true, true, 1, false);
		//	m_pBarrel->SetScale({ 3.f, 3.f, 1.f });
		//	m_pBarrel->SetPos({ m_vPos.x + m_pBarrel->GetScale().x, m_vPos.y + m_pBarrel->GetScale().y / 2.f + m_pBarrel->GetScale().y / 3.f, m_vPos.z + VILLAGE_TILESIZE });
		//	m_pBarrel->SetAngle({ 0.f, PI / 4.f * 3.f, 0.f });

		//	m_vecGameObject.push_back(m_pBarrel);

		//	// 난로
		//	shared_ptr<CGameObject> m_pStove = make_shared<CEnvironmentObj>(m_pGraphicDev, L"Tavern_Stove_Texture", true, true, 1, false);
		//	m_pStove->SetScale({ 3.f, 8.3f, 1.f });
		//	m_pStove->SetPos({ m_vPos.x + VILLAGE_TILESIZE * 3.f - 5.f, m_vPos.y + m_pStove->GetScale().y / 2.f + m_pStove->GetScale().y / 3.f, m_vPos.z + VILLAGE_TILESIZE + 5.f });
		//	m_pStove->SetAngle({ 0.f, 0.f, 0.f });

		//	m_vecGameObject.push_back(m_pStove);

		//	// 바 테이블
		//	//shared_ptr<CGameObject> m_pTable = make_shared<CGambling>(m_pGraphicDev);
		//	shared_ptr<CGameObject> m_pTable = make_shared<CEnvironmentObj>(m_pGraphicDev, L"Tavern_Bar_Table_Texture", true, false, 1, false);

		//	m_pTable->SetScale({ 10.f, 7.f, 1.f });
		//	m_pTable->SetPos({ m_vPos.x + VILLAGE_TILESIZE / 2.f + 4.f, m_vPos.y + m_pTable->GetScale().y / 2.f + m_pTable->GetScale().y / 3.f, m_vPos.z + VILLAGE_TILESIZE * 2.f + 5.f });

		//	m_vecGameObject.push_back(m_pTable);

		//	// 바 테이블 배경
		//	shared_ptr<CGameObject> m_pBarBack = make_shared<CEnvironmentObj>(m_pGraphicDev, L"Tavern_Bar_Back_Texture", false, false, 1, false);

		//	m_pBarBack->SetScale({ 10.f, 9.f, 1.f });
		//	m_pBarBack->SetPos({ m_vPos.x + VILLAGE_TILESIZE / 2.f + 4.f, m_vPos.y + m_pBarBack->GetScale().y / 2.f + m_pBarBack->GetScale().y / 3.f, m_vPos.z + VILLAGE_TILESIZE * 3 - 1.1f });

		//	m_vecGameObject.push_back(m_pBarBack);
	}

	return __super::ReadyGameObject();
}

_int CGuild::UpdateGameObject(const _float& fTimeDelta)
{
	Engine::AddRenderGroup(RENDER_ALPHA, shared_from_this());

	_int iExit = __super::UpdateGameObject(fTimeDelta);

	return iExit;
}

void CGuild::LateUpdateGameObject()
{
	__super::LateUpdateGameObject();
}

void CGuild::RenderGameObject()
{
	__super::RenderGameObject();
}