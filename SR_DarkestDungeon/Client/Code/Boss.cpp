#include "pch.h"
#include "Boss.h"
#include "Export_Utility.h"
#include "Export_System.h"

#include "Player.h"


CBoss::CBoss(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CBoss::CBoss(const CBoss& rhs)
	: Engine::CGameObject(rhs)
{
}

CBoss::~CBoss()
{
}


HRESULT CBoss::ReadyGameObject()
{
	__super::ReadyGameObject();

	// 플레이어 받아오기 (거리 계산, 상태 변경)
	m_pPlayer = CGameMgr::GetInstance()->GetPlayer();

	// 컴퍼넌트 받아오기
	m_pPlayerTransformCom = dynamic_pointer_cast<CTransform>(m_pPlayer->GetComponent(L"Com_Transform", ID_DYNAMIC));

	m_pTransformCom->SetAngle(m_vAngle);

	{
		m_eAnimState = EBossState::P1_IDLE;
		m_vAngle = { 0.f, PI / 2, 0.f };

		m_pTransformCom->SetPosition(m_vPos.x, m_vPos.y, m_vPos.z);

		m_pTransformCom->SetAngle(m_vAngle);
		m_pTransformCom->Rotation(ROT_Y, PI / 2.f);

		m_pTextureCom->SetAnimKey(L"Boss_Phase1_Idle", 0.06f);
		m_vOriginSize = m_pTextureCom->GetTextureSize();
	}

	return S_OK;
}

_int CBoss::UpdateGameObject(const _float& fTimeDelta)
{
	_int	iExit = __super::UpdateGameObject(fTimeDelta);

	m_vPos = *m_pTransformCom->GetPos();

	Engine::AddRenderGroup(RENDER_ALPHA, shared_from_this());

	// FSM 조건
	FSM(fTimeDelta);

	// 애니메이션 변경
	ChangeAnim();
	// 시간에 따른 애니메이션 끝내기
	AnimDuration(fTimeDelta);

	// 키 입력
	KeyInput();




	//빌보드 시작
	/*_matrix matWorld;

	matWorld = *m_pTransformCom->GetWorld();
	SetBillBoard(matWorld);
	m_pTransformCom->SetWorld(&matWorld);*/

	return iExit;
}

void CBoss::LateUpdateGameObject()
{
	__super::LateUpdateGameObject();
}

void CBoss::RenderGameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->GetWorld());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->SetAnimTexture();;
	m_pBufCom->RenderBuffer();
	m_pEffectBufCom->RenderBuffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	__super::RenderGameObject();
}

void CBoss::AddComponent()
{
	shared_ptr<CComponent> pComponent;

	pComponent = m_pTransformCom = make_shared<CTransform>();
	m_pTransformCom->ReadyTransform();
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform",pComponent });

	pComponent = m_pBufCom = make_shared <CRcTex>(m_pGraphicDev);
	m_pBufCom->ReadyBuffer();
	m_mapComponent[ID_STATIC].insert({ L"Com_RCTex",pComponent });

	pComponent = m_pEffectBufCom = make_shared<CRcTex>(m_pGraphicDev);
	m_pEffectBufCom->ReadyBuffer();
	m_mapComponent[ID_STATIC].insert({ L"Com_EffectRCTex",pComponent });

	pComponent = m_pTextureCom = make_shared<CAnimator>(m_pGraphicDev);
	m_pTextureCom->SetAnimKey(m_strAnimKey, 0.1f);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Animator", pComponent });

	pComponent = m_pColliderCom = make_shared<CCollider>(m_pGraphicDev);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Collider",pComponent });
	m_pColliderCom->SetScale({ 30.f, 30.f, 1.f });
	m_pColliderCom->SetPos(m_pTransformCom->GetPos());
}

void CBoss::FSM(const _float& fTimeDelta)
{
	// Idle상태
	if (m_eCurAnimState == EBossState::P1_IDLE)
	{
		ChasePlayer(fTimeDelta, m_fP1_IdleSpeed);
	}


	// 돌진 상태
	if (m_eCurAnimState == EBossState::P1_DASH && m_fP1DashTime < 0.06f * 6)
	{
		ChasePlayer(fTimeDelta, m_fP1_DashSpeed);
	}
}

void CBoss::ChangeAnim()
{
	_float fXpos, fYpos;
	// Animation
	if (m_eCurAnimState != m_ePrevAnimState)
	{
		switch (m_eCurAnimState)
		{
		case EBossState::P1_IDLE:
			m_pTextureCom->SetAnimKey(L"Boss_Phase1_Idle", 0.06f);
			break;
		case EBossState::P1_ATTACK:
			m_pTextureCom->SetAnimKey(L"Boss_Phase1_Attack", 0.06f);
			break;
		case EBossState::P1_LASER1:
			m_pTextureCom->SetAnimKey(L"Boss_Phase1_Laser1", 0.06f);
			break;
		case EBossState::P1_LASER2:
			m_pTextureCom->SetAnimKey(L"Boss_Phase1_Laser2", 0.06f);
			break;
		case EBossState::P1_DASH:
			m_pTextureCom->SetAnimKey(L"Boss_Phase1_Dash", 0.06f);
			break;
		case EBossState::P1_CHANGE:
			m_pTextureCom->SetAnimKey(L"Boss_Phase1_Change", 0.06f);
			break;
		}
		m_ePrevAnimState = m_eCurAnimState;

	}

	_vec2 vcurPos = m_pTextureCom->GetTextureSize();

	if (m_eCurAnimState == EBossState::P1_IDLE) {
		fXpos = (vcurPos.x / m_vOriginSize.x);
		fYpos = (vcurPos.y / m_vOriginSize.y);
	}
	else {
		fXpos = (vcurPos.x / m_vOriginSize.x);
		fYpos = (vcurPos.y / m_vOriginSize.y);
	}

	m_pTransformCom->SetScale(40.f * fXpos, 35.f * fYpos, 1.f);

}

void CBoss::AnimDuration(const _float& fTimeDelta)
{
	if (m_eCurAnimState == EBossState::P1_ATTACK)
	{
		m_fP1AttackTime -= fTimeDelta;
		if (m_fP1AttackTime < 0.f)
		{
			m_fP1AttackTime = 0.06f * 12;
			m_eCurAnimState = EBossState::P1_IDLE;
		}
	}
	if (m_eCurAnimState == EBossState::P1_LASER1)
	{
		m_fP1Laser1Time -= fTimeDelta;
		if (m_fP1Laser1Time < 0.f)
		{
			m_fP1Laser1Time = 0.06f * 7;
			m_eCurAnimState = EBossState::P1_LASER2;
		}
	}
	if (m_eCurAnimState == EBossState::P1_LASER2)
	{
		m_fP1Laser2Time -= fTimeDelta;
		if (m_fP1Laser2Time < 0.f)
		{
			m_fP1Laser2Time = 0.06f * 4;
			m_eCurAnimState = EBossState::P1_IDLE;
		}
	}
	if (m_eCurAnimState == EBossState::P1_DASH)
	{
		m_fP1DashTime -= fTimeDelta;
		if (m_fP1DashTime < 0.f)
		{
			m_fP1DashTime = 0.06f * 11;
			m_eCurAnimState = EBossState::P1_IDLE;
		}
	}
	if (m_eCurAnimState == EBossState::P1_CHANGE)
	{
		m_fP1ChangeTime -= fTimeDelta;
		if (m_fP1ChangeTime < 0.f)
		{
			m_fP1ChangeTime = 0.06f * 43;
			m_eCurAnimState = EBossState::P1_IDLE;
		}
	}
}

void CBoss::KeyInput()
{
	if (GetAsyncKeyState('1') & 0x8000) {
		m_eCurAnimState = EBossState::P1_IDLE;
	}
	if (GetAsyncKeyState('2') & 0x8000) {	
		m_eCurAnimState = EBossState::P1_ATTACK;
	}
	if (GetAsyncKeyState('3') & 0x8000) {
		m_eCurAnimState = EBossState::P1_LASER1;
	}
	if (GetAsyncKeyState('4') & 0x8000) {
		m_eCurAnimState = EBossState::P1_DASH;
	}
	if (GetAsyncKeyState('5') & 0x8000) {
		m_eCurAnimState = EBossState::P1_CHANGE;
	}
}

_float CBoss::CalcDistance()
{
	_vec3 vPos;
	m_pTransformCom->GetInfo(INFO_POS, &vPos);

	_vec3 vPosPlayer;
	m_pPlayerTransformCom->GetInfo(INFO_POS, &vPosPlayer);

	_vec3 vDist = vPosPlayer - vPos;

	return D3DXVec3Length(&vDist);
}

_vec3 CBoss::CalcDirection()
{
	_vec3 vPos;
	m_pTransformCom->GetInfo(INFO_POS, &vPos);

	_vec3 vPosPlayer;
	m_pPlayerTransformCom->GetInfo(INFO_POS, &vPosPlayer);

	_vec3 vDist = vPosPlayer - vPos;

	return vDist;
}

void CBoss::ChasePlayer(const _float& fTimeDelta, float _fSpeed)
{
	_vec3		vDir;
	vDir = CalcDirection();
	D3DXVec3Normalize(&vDir, &vDir);
	vDir.y = 0.f;
	m_pTransformCom->MoveForward(&vDir, fTimeDelta, _fSpeed);
}
