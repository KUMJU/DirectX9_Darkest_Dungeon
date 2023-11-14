#include "pch.h"
#include "Sunken.h"
#include "Export_Utility.h"
#include "Export_System.h"

#include "Bullet1.h"
#include "Bullet2.h"
#include "Mob_Bullet.h"
#include "Laser.h"
#include "Player.h"

CSunken::CSunken(LPDIRECT3DDEVICE9 pGraphicDev)
	:CCreature(pGraphicDev)
{
}

CSunken::CSunken(LPDIRECT3DDEVICE9 pGraphicDev, STAT _tCommonStat, _int _iPosition,
	vector<shared_ptr<CSkill>>& _pVecSkill)
	: CCreature(pGraphicDev, _tCommonStat, _iPosition, _pVecSkill)
{
}

CSunken::CSunken(LPDIRECT3DDEVICE9 pGraphicDev, STAT _tCommonStat)
	: CCreature(pGraphicDev, _tCommonStat)
{
}

CSunken::CSunken(const CCreature& rhs)
	: CCreature(rhs)
{
}

CSunken::~CSunken()
{
}

void CSunken::PickingObj()
{
	m_bPicked = true;
}

HRESULT CSunken::ReadyGameObject()
{
	__super::ReadyGameObject();

	// 플레이어 받아오기 (거리 계산, 상태 변경)
	m_pPlayer = CGameMgr::GetInstance()->GetPlayer();

	// 컴퍼넌트 받아오기
	m_pPlayerTransformCom = dynamic_pointer_cast<CTransform>(m_pPlayer->GetComponent(L"Com_Transform", ID_DYNAMIC));

	m_pTransformCom->SetAngle(m_vAngle);

	{
		m_eAnimState = EMobState::GO_OUT;
		m_vAngle = { 0.f, PI / 2, 0.f };

		m_pTransformCom->SetPosition(m_vPos.x, m_vPos.y, m_vPos.z);

		m_pTransformCom->SetAngle(m_vAngle);
		m_pTransformCom->Rotation(ROT_Y, PI / 2.f);

		m_pTextureCom->SetAnimKey(L"Mob_Out", 0.12f);
		m_vOriginSize = m_pTextureCom->GetTextureSize();
	}

	SetEnable(false);

	return S_OK;
}

_int CSunken::UpdateGameObject(const _float& fTimeDelta)
{
	_int	iExit = CGameObject::UpdateGameObject(fTimeDelta);

	m_vPos = *m_pTransformCom->GetPos();

	//빌보드 시작
	_matrix matWorld;

	matWorld = *m_pTransformCom->GetWorld();
	SetBillBoard(matWorld);
	m_pTransformCom->SetWorld(&matWorld);

	Engine::AddRenderGroup(RENDER_ALPHA, shared_from_this());

	// FSM 조건
	FSM(fTimeDelta);

	// 애니메이션 변경
	ChangeAnim();
	// 시간에 따른 애니메이션 끝내기
	AnimDuration(fTimeDelta);

	// 키 입력
	//KeyInput();

	return iExit;
}

void CSunken::LateUpdateGameObject()
{
	__super::LateUpdateGameObject();
}

void CSunken::RenderGameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->GetWorld());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->SetAnimTexture();;
	m_pBufCom->RenderBuffer();
	m_pEffectBufCom->RenderBuffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	__super::RenderGameObject();
}

void CSunken::AddComponent()
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

void CSunken::FSM(const _float& fTimeDelta)
{
	// Out
	if (m_eCurAnimState == EMobState::GO_OUT)
	{
		m_fOutTime -= fTimeDelta;
		if (m_fOutTime < 0.f)
		{
			m_fOutTime = 0.12f * 7;
			m_eCurAnimState = EMobState::IDLE;
			m_bIdle = true;
			m_bOut = false;
		}
	}
}

void CSunken::ChangeAnim()
{
	_float fXpos, fYpos;
	// Animation
	if (m_eCurAnimState != m_ePrevAnimState)
	{
		switch (m_eCurAnimState)
		{
		case EMobState::IDLE:
			m_pTextureCom->SetAnimKey(L"Mob_Idle", 0.06f);
			break;
		case EMobState::ATTACK:
			m_pTextureCom->SetAnimKey(L"Mob_Shoot", 0.12f);
			break;
		case EMobState::GO_IN:
			m_pTextureCom->SetAnimKey(L"Mob_In", 0.12f);
			break;
		case EMobState::GO_OUT:
			m_pTextureCom->SetAnimKey(L"Mob_Out", 0.12f);
			break;
		case EMobState::DEATH:
			m_pTextureCom->SetAnimKey(L"Mob_Death", 0.12f);
			break;
		}
		m_ePrevAnimState = m_eCurAnimState;

	}

	_vec2 vcurPos = m_pTextureCom->GetTextureSize();

	if (m_eCurAnimState == EMobState::IDLE) {
		fXpos = (vcurPos.x / m_vOriginSize.x);
		fYpos = (vcurPos.y / m_vOriginSize.y);
	}
	else {
		fXpos = (vcurPos.x / m_vOriginSize.x);
		fYpos = (vcurPos.y / m_vOriginSize.y);
	}

	m_pTransformCom->SetScale(8.f * fXpos, 8.f * fYpos, 8.f * fXpos);

}

void CSunken::AnimDuration(const _float& fTimeDelta)
{
	int a;
}

void CSunken::KeyInput()
{
	if (GetAsyncKeyState('1') & 0x8000) {
		m_eCurAnimState = EMobState::IDLE;
	}
	if (GetAsyncKeyState('2') & 0x8000) {
		m_eCurAnimState = EMobState::ATTACK;
	}
	if (GetAsyncKeyState('3') & 0x8000) {
		m_eCurAnimState = EMobState::GO_IN;
	}
	if (GetAsyncKeyState('4') & 0x8000) {
		m_eCurAnimState = EMobState::GO_OUT;
	}
	if (GetAsyncKeyState('5') & 0x8000) {
		m_eCurAnimState = EMobState::DEATH;
	}
}

_float CSunken::CalcDistance()
{
	_vec3 vPos;
	m_pTransformCom->GetInfo(INFO_POS, &vPos);

	_vec3 vPosPlayer;
	m_pPlayerTransformCom->GetInfo(INFO_POS, &vPosPlayer);

	_vec3 vDist = vPosPlayer - vPos;

	return D3DXVec3Length(&vDist);
}

_vec3 CSunken::CalcDirection()
{
	_vec3 vPos;
	m_pTransformCom->GetInfo(INFO_POS, &vPos);

	_vec3 vPosPlayer;
	m_pPlayerTransformCom->GetInfo(INFO_POS, &vPosPlayer);

	_vec3 vDist = vPosPlayer - vPos;

	return vDist;
}

void CSunken::ChasePlayer(const _float& fTimeDelta, float _fSpeed)
{
	_vec3		vDir;
	vDir = CalcDirection();
	D3DXVec3Normalize(&vDir, &vDir);
	vDir.y = 0.f;
	m_pTransformCom->MoveForward(&vDir, fTimeDelta, _fSpeed);
}

void CSunken::ShootBullet1()
{
	for (int i = 0; i < m_iBullet1TotalNum; i++)
	{
		if (!dynamic_pointer_cast<CGameObject>(m_pVecBullet1[i])->GetIsEnable())
		{
			dynamic_pointer_cast<CGameObject>(m_pVecBullet1[i])->SetEnable(true);
			shared_ptr<CTransform> pTransform = dynamic_pointer_cast<CTransform>(
				m_pVecBullet1[i]->GetComponent(L"Com_Transform", ID_DYNAMIC));
			pTransform->SetPosition(m_vPos.x, m_vPos.y, m_vPos.z);
			_vec3 vPosPlayer;
			m_pPlayerTransformCom->GetInfo(INFO_POS, &vPosPlayer);
			vPosPlayer = _vec3(vPosPlayer.x, vPosPlayer.y, vPosPlayer.z);
			m_pVecBullet1[i]->SetTarget(vPosPlayer);
			break;
		}
	}
}