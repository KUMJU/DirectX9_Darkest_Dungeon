#include "pch.h"
#include "GuildNPC.h"
#include "GameMgr.h"
#include "CameraMgr.h"

#include "Export_System.h"
#include "Export_Utility.h"

CGuildNPC::CGuildNPC(LPDIRECT3DDEVICE9 pGraphicDev)
	: CInteractionObj(pGraphicDev)
{
}

CGuildNPC::CGuildNPC(const CGuildNPC& rhs)
	: CInteractionObj(rhs)
{
}

CGuildNPC::~CGuildNPC()
{
}

HRESULT CGuildNPC::ReadyGameObject()
{
	__super::ReadyGameObject();

	return S_OK;
}

_int CGuildNPC::UpdateGameObject(const _float& fTimeDelta)
{
	_int	iExit = __super::UpdateGameObject(fTimeDelta);

	return iExit;
}

void CGuildNPC::LateUpdateGameObject()
{
	__super::LateUpdateGameObject();
}

void CGuildNPC::RenderGameObject()
{
	__super::RenderGameObject();
}

void CGuildNPC::AddComponent()
{
	shared_ptr<CComponent> pComponent;

	pComponent = m_pBufferCom = make_shared<CRcTex>(m_pGraphicDev);
	dynamic_pointer_cast<CRcTex>(m_pBufferCom)->ReadyBuffer();
	m_mapComponent[ID_STATIC].insert({ L"Com_RcTex", pComponent });

	pComponent = m_pTransformCom = make_shared<CTransform>(_vec3(0.f, 0.f, 0.f), _vec3(1.f, 1.f, 1.f), _vec3(0.f, 0.f, 0.f));
	m_pTransformCom->ReadyTransform();
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });
	m_pTransformCom->SetPosition(m_vPos.x, m_vPos.y, m_vPos.z);
	m_pTransformCom->SetScale(m_vScale.x, m_vScale.y, m_vScale.z);

	pComponent = m_pTextureCom = make_shared<CTexture>(m_pGraphicDev);
	m_pTextureCom->SetTextureKey(L"Guild_NPC_Texture", TEX_NORMAL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Texture",pComponent });
}

void CGuildNPC::GetInteractionKey(const _float& fTimeDelta)
{
	// 인터랙션 키 UI 보여주기
	// ----------수정 필요------------

	// 키 입력받기
	if (GetAsyncKeyState('C') & 0x8000)
	{
		m_bInteracting = true;

		// 플레이어 움직임 막기
		CGameMgr::GetInstance()->SetGameState(EGameState::LOCK);

		// 카메라 이동

		Interaction();
	}
}

void CGuildNPC::Interaction()
{
	// 영웅 선택
	{
		// 영웅 텍스처 생성

		// 피킹 받기

		// 술집 키 입력처리 (술과 함께 넘어가는 근심)
	}
}

_bool CGuildNPC::IsFinish()
{
	// 키 입력받기
	if (GetAsyncKeyState('X') & 0x8000)
	{
		m_bInteracting = false;

		// 카메라 원상복귀

		// 플레이어 행동 풀기
		CGameMgr::GetInstance()->SetGameState(EGameState::PRCESS);

		return true;
	}

	return false;
}

void CGuildNPC::FinishInteraction()
{
}

void CGuildNPC::ChangeTexture()
{
}
