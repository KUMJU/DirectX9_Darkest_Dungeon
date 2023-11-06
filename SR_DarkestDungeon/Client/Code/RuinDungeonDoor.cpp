#include "pch.h"
#include "RuinDungeonDoor.h"
#include "Player.h"
#include "Export_System.h"
#include "Export_Utility.h"

CRuinDungeonDoor::CRuinDungeonDoor(LPDIRECT3DDEVICE9 pGraphicDev, ERuinDungeonDoorType _eDoorType)
	: CInteractionObj(pGraphicDev), m_eDoorType(_eDoorType)
{
}

CRuinDungeonDoor::CRuinDungeonDoor(const CRuinDungeonDoor& rhs)
	: CInteractionObj(rhs)
{
}

CRuinDungeonDoor::~CRuinDungeonDoor()
{
}

HRESULT CRuinDungeonDoor::ReadyGameObject()
{
	__super::ReadyGameObject();

	return S_OK;
}

_int CRuinDungeonDoor::UpdateGameObject(const _float& fTimeDelta)
{
	_int	iExit = __super::UpdateGameObject(fTimeDelta);

	return iExit;
}

void CRuinDungeonDoor::LateUpdateGameObject()
{
	__super::LateUpdateGameObject();
}

void CRuinDungeonDoor::RenderGameObject()
{
	__super::RenderGameObject();
}

void CRuinDungeonDoor::AddComponent()
{

	shared_ptr<CComponent> pComponent;

	pComponent = m_pBufferCom = make_shared<CRcTex>(m_pGraphicDev);
	dynamic_pointer_cast<CRcTex>(m_pBufferCom)->ReadyBuffer();
	m_mapComponent[ID_STATIC].insert({ L"Com_RcTex", pComponent });

	pComponent = m_pTextureCom = make_shared<CTexture>(m_pGraphicDev);
	m_mapComponent[ID_STATIC].insert({ L"Com_Texture", pComponent });

	pComponent = m_pTransformCom = make_shared<CTransform>(_vec3(0.f, 0.f, 0.f), _vec3(1.f, 1.f, 1.f), _vec3(0.f, 0.f, 0.f));
	m_pTransformCom->ReadyTransform();
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });
	m_pTransformCom->SetPosition(m_vPos.x, m_vPos.y, m_vPos.z);
	m_pTransformCom->SetScale(m_vScale.x, m_vScale.y, m_vScale.z);

	pComponent = m_pTextureCom = make_shared<CTexture>(m_pGraphicDev);
	m_pTextureCom->SetTextureKey(L"Ruin_Door_Opened", TEX_NORMAL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Texture",pComponent });
}

void CRuinDungeonDoor::GetInteractionKey(const _float& fTimeDelta)
{
	// 인터랙션 키 UI 보여주기
	// ----------수정 필요------------

	// 키 입력받기
	if (GetAsyncKeyState('C') & 0x8000)
	{
		m_bInteracting = true;

		// 플레이어 행동 막기
		//CGameMgr::GetInstance()->SetGameState(EGameState::LOCK);

		// 텍스처 또는 애니메이션 변경
		ChangeTexture();

		switch (m_eDoorType)
		{
		case ERuinDungeonDoorType::ROOM1:
			m_iCurrent = 2;
			break;
		case ERuinDungeonDoorType::ROOM2:
			m_iCurrent = 3;
			break;
		case ERuinDungeonDoorType::ROOM3:
			m_iCurrent = 4;
			break;
		case ERuinDungeonDoorType::ROOM4:
			m_iCurrent = 5;
			break;
		case ERuinDungeonDoorType::ROOM5:
			m_iCurrent = 6;
			break;
		}
		dynamic_pointer_cast<CPlayer>(CGameMgr::GetInstance()->GetPlayer())->SetRoomChange(true);
		dynamic_pointer_cast<CPlayer>(CGameMgr::GetInstance()->GetPlayer())->SetCurrentRoom(m_iCurrent);

		Interaction();
	}
}

void CRuinDungeonDoor::Interaction()
{
}

void CRuinDungeonDoor::FinishInteraction()
{
	m_bInteracting = false;

	m_pTextureCom->SetTextureKey(L"Ruin_Door_Opened", TEX_NORMAL);
}

void CRuinDungeonDoor::ChangeTexture()
{
	m_pTextureCom->SetTextureKey(L"Ruin_Door_Opened", TEX_NORMAL);
}

_bool CRuinDungeonDoor::IsFinish()
{
	// 키 입력받기
	if (GetAsyncKeyState('X') & 0x8000)
	{
		m_bInteracting = false;
		m_iCurrent = 10;
		m_pTextureCom->SetTextureKey(L"Ruin_Door_Opened", TEX_NORMAL);

		// 플레이어 행동 풀기
		//CGameMgr::GetInstance()->SetGameState(EGameState::PRCESS);

		return true;
	}

	return false;
}