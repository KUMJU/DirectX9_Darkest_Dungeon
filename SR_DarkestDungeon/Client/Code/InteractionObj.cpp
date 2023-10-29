#include "pch.h"
#include "InteractionObj.h"
#include "Export_Utility.h"
#include "Export_System.h"

#include "Player.h"


CInteractionObj::CInteractionObj(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CInteractionObj::CInteractionObj(const CInteractionObj& rhs)
	: Engine::CGameObject(rhs)
{
}

CInteractionObj::~CInteractionObj()
{
}


HRESULT CInteractionObj::ReadyGameObject()
{
	// �÷��̾� �޾ƿ��� (�Ÿ� ���, ���� ����)
	m_pPlayer = CGameMgr::GetInstance()->GetPlayer();

	// ���۳�Ʈ �޾ƿ���
	m_pPlayerTransformCom = dynamic_pointer_cast<CTransform>(m_pPlayer->GetComponent(L"Com_Transform", ID_DYNAMIC));

	m_pTransformCom->SetAngle(m_vAngle);

	return S_OK;
}

_int CInteractionObj::UpdateGameObject(const _float& fTimeDelta)
{
	Engine::AddRenderGroup(RENDER_ALPHA, shared_from_this());
	_int	iExit = CGameObject::UpdateGameObject(fTimeDelta);

	// ��ȣ �ۿ� ���� �ƴϰ�, ��ȣ �ۿ� �Ÿ� �ȿ� ��� ���
	if (!m_bInteracting && 12.f > CalcDistance())
	{
		GetInteractionKey(fTimeDelta);
	}

	// ��ȣ �ۿ� ���� ���
	else if(m_bInteracting)
	{
		// ��ȣ �ۿ��� �ϴٰ� �ָ� �̵��� ���
		if (20.f < CalcDistance())
		{
			// �ڵ����� ��ȣ �ۿ� �����ϰ� �̵� ���� Ǯ��
			FinishInteraction();
		}

		// �Ÿ� ������ ��ȣ�ۿ����� ���
		else
		{
			// ��ȣ �ۿ� ���� Ʈ���� �ޱ�
			// ��ȣ�ۿ� ���� ���� ���� Ŭ�������� �����ϱ�
			if (IsFinish())
				FinishInteraction();
		}
	}

	return iExit;
}

void CInteractionObj::LateUpdateGameObject()
{
}

void CInteractionObj::RenderGameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->GetWorld());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->SetTexture(0);

	m_pBufferCom->RenderBuffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CInteractionObj::AddComponent()
{
}

_float CInteractionObj::CalcDistance()
{
	_vec3 vPos;
	m_pTransformCom->GetInfo(INFO_POS, &vPos);

	_vec3 vPosPlayer;
	m_pPlayerTransformCom->GetInfo(INFO_POS, &vPosPlayer);

	_vec3 vDist = vPosPlayer - vPos;

	return D3DXVec3Length(&vDist);
}

void CInteractionObj::GetInteractionKey(const _float& fTimeDelta)
{
	// ���ͷ��� Ű UI �����ֱ�
	// ----------���� �ʿ�------------

	// Ű �Է¹ޱ�
	if (GetAsyncKeyState('C') & 0x8000)
	{
		m_bInteracting = true;

		// �÷��̾� �ൿ ����
		CGameMgr::GetInstance()->SetGameState(EGameState::LOCK);

		// �ؽ�ó �Ǵ� �ִϸ��̼� ����
		ChangeTexture();

		Interaction();
	}
}

void CInteractionObj::FinishInteraction()
{
	m_bInteracting = false;

	// �÷��̾� �ൿ Ǯ��
	CGameMgr::GetInstance()->SetGameState(EGameState::PRCESS);
}