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
	// 플레이어 받아오기 (거리 계산, 상태 변경)
	m_pPlayer = CGameMgr::GetInstance()->GetPlayer();
	// ----------수정 필요---------- (컴퍼넌트 받아오기)
	// m_pPlayerTransformCom = m_pPlayer->GetComponent();
	return E_NOTIMPL;
}

_int CInteractionObj::UpdateGameObject(const _float& fTimeDelta)
{
	Engine::AddRenderGroup(RENDER_NONALPHA, shared_from_this());
	_int	iExit = CGameObject::UpdateGameObject(fTimeDelta);

	if (50.f > CalcDistance() && !m_bInteracting)
	{
		GetInteractionKey(fTimeDelta);
	}

	return iExit;
}

void CInteractionObj::LateUpdateGameObject()
{
}

void CInteractionObj::RenderGameObject()
{
	//	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	//m_pTextureCom->Set_Texture(0);

	m_pBufferCom->RenderBuffer();

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

}

void CInteractionObj::AddComponent()
{
	shared_ptr<CComponent> pComponent;

	pComponent = m_pBufferCom = make_shared<CRcTex>(m_pGraphicDev);
	NULL_CHECK_MSG(pComponent, L"RcTex AddComponent Failed");
	dynamic_pointer_cast<CRcTex>(m_pBufferCom)->ReadyBuffer();

	m_mapComponent[ID_STATIC].insert({ L"Com_RcTex", pComponent });
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
	if (Engine::Get_DIKeyState(DIK_C) & 0X80)
	{
		// 상호작용 시작
		if (!m_bInteracting)
		{
			m_bInteracting = true;
			// 플레이어 행동 막기
			// ----------수정 필요------------
			//m_pPlayer->BockMove(true);
			InteractionAction();
		}
		// 상호작용 종료
		else
		{
			m_bInteracting = false;
			// ----------수정 필요------------
			//m_pPlayer->BlockMove(false);
		}
	}

	if (!m_bInteracting)
	{
		// 인터랙션 키 UI 보여주기
		// ----------수정 필요------------
	}
}

void CInteractionObj::InteractionAction()
{
}
