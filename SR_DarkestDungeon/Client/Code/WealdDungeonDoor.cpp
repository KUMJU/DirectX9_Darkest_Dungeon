#include "pch.h"
#include "WealdDungeonDoor.h"
#include "Player.h"
#include "Export_System.h"
#include "Export_Utility.h"

CWealdDungeonDoor::CWealdDungeonDoor(LPDIRECT3DDEVICE9 pGraphicDev, EWealdDungeonDoorType _eDoorType)
	: CInteractionObj(pGraphicDev), m_eDoorType(_eDoorType)
{
}

CWealdDungeonDoor::CWealdDungeonDoor(const CWealdDungeonDoor& rhs)
	: CInteractionObj(rhs)
{
}

CWealdDungeonDoor::~CWealdDungeonDoor()
{
}

HRESULT CWealdDungeonDoor::ReadyGameObject()
{
	__super::ReadyGameObject();

	return S_OK;
}

_int CWealdDungeonDoor::UpdateGameObject(const _float& fTimeDelta)
{
	_int	iExit = __super::UpdateGameObject(fTimeDelta);

	return iExit;
}

void CWealdDungeonDoor::LateUpdateGameObject()
{
	__super::LateUpdateGameObject();
}

void CWealdDungeonDoor::RenderGameObject()
{
	__super::RenderGameObject();
}

void CWealdDungeonDoor::AddComponent()
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
	m_pTextureCom->SetTextureKey(L"Weald_Door_Closed", TEX_NORMAL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Texture",pComponent });
}

void CWealdDungeonDoor::GetInteractionKey(const _float& fTimeDelta)
{
	// ���ͷ��� Ű UI �����ֱ�
	// ----------���� �ʿ�------------

	// Ű �Է¹ޱ�
	if (GetAsyncKeyState('C') & 0x8000)
	{
		m_bInteracting = true;

		// �ؽ�ó �Ǵ� �ִϸ��̼� ����
		ChangeTexture();

		switch (m_eDoorType)
		{
		case EWealdDungeonDoorType::ROOM1:
			m_iCurrent = 2;
			break;
		case EWealdDungeonDoorType::ROOM2:
			m_iCurrent = 3;
			break;
		case EWealdDungeonDoorType::ROOM3:
			m_iCurrent = 4;
			break;
		case EWealdDungeonDoorType::ROOM4:
			m_iCurrent = 5;
			break;
		}
		dynamic_pointer_cast<CPlayer>(CGameMgr::GetInstance()->GetPlayer())->SetRoomChange(true);
		dynamic_pointer_cast<CPlayer>(CGameMgr::GetInstance()->GetPlayer())->SetCurrentRoom(m_iCurrent);

		Interaction();
	}
}

void CWealdDungeonDoor::Interaction()
{
}

void CWealdDungeonDoor::FinishInteraction()
{
	m_bInteracting = false;

	m_pTextureCom->SetTextureKey(L"Weald_Door_Closed", TEX_NORMAL);
}

void CWealdDungeonDoor::ChangeTexture()
{
	m_pTextureCom->SetTextureKey(L"Weald_Door_Opened", TEX_NORMAL);
}

_bool CWealdDungeonDoor::IsFinish()
{
	// Ű �Է¹ޱ�
	if (GetAsyncKeyState('X') & 0x8000)
	{
		m_bInteracting = false;
		m_iCurrent = 10;
		m_pTextureCom->SetTextureKey(L"Weald_Door_Closed", TEX_NORMAL);

		// �÷��̾� �ൿ Ǯ��
		//CGameMgr::GetInstance()->SetGameState(EGameState::PRCESS);

		return true;
	}

	return false;
}