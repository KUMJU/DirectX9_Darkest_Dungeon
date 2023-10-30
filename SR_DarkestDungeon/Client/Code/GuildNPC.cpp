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
	// ���ͷ��� Ű UI �����ֱ�
	// ----------���� �ʿ�------------

	// Ű �Է¹ޱ�
	if (GetAsyncKeyState('C') & 0x8000)
	{
		m_bInteracting = true;

		// �÷��̾� ������ ����
		CGameMgr::GetInstance()->SetGameState(EGameState::LOCK);

		// ī�޶� �̵�

		Interaction();
	}
}

void CGuildNPC::Interaction()
{
	// ���� ����
	{
		// ���� �ؽ�ó ����

		// ��ŷ �ޱ�

		// ���� Ű �Է�ó�� (���� �Բ� �Ѿ�� �ٽ�)
	}
}

_bool CGuildNPC::IsFinish()
{
	// Ű �Է¹ޱ�
	if (GetAsyncKeyState('X') & 0x8000)
	{
		m_bInteracting = false;

		// ī�޶� ���󺹱�

		// �÷��̾� �ൿ Ǯ��
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
