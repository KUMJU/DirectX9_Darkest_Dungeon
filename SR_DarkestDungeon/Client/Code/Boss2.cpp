#include "pch.h"
#include "Boss2.h"
#include "Export_Utility.h"
#include "Export_System.h"

#include "Bullet1.h"
#include "Bullet2.h"
#include "Bullet3.h"
#include "Laser.h"
#include "Mob.h"
#include "Player.h"

CBoss2::CBoss2(LPDIRECT3DDEVICE9 pGraphicDev)
	:CCreature(pGraphicDev)
{
}

CBoss2::CBoss2(LPDIRECT3DDEVICE9 pGraphicDev, STAT _tCommonStat, _int _iPosition,
	vector<shared_ptr<CSkill>>& _pVecSkill)
	: CCreature(pGraphicDev, _tCommonStat, _iPosition, _pVecSkill)
{
}

CBoss2::CBoss2(LPDIRECT3DDEVICE9 pGraphicDev, STAT _tCommonStat)
	: CCreature(pGraphicDev, _tCommonStat)
{
}

CBoss2::CBoss2(const CCreature& rhs)
	: CCreature(rhs)
{
}

CBoss2::~CBoss2()
{
}

void CBoss2::PickingObj()
{
	m_bPicked = true;
}

HRESULT CBoss2::ReadyGameObject()
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

_int CBoss2::UpdateGameObject(const _float& fTimeDelta)
{
	_int	iExit = CGameObject::UpdateGameObject(fTimeDelta);

	m_vPos = *m_pTransformCom->GetPos();

	//빌보드 시작
	_matrix matWorld;

	matWorld = *m_pTransformCom->GetWorld();
	SetBillBoard(matWorld);
	m_pTransformCom->SetWorld(&matWorld);

	Engine::AddRenderGroup(RENDER_ALPHA, shared_from_this());

	// 벽 아래로 못 뚫고 내려가게 하기
	OnFloor(30.f);

	// 영역 밖으로 벗어나면 가운데로 텔레포트
	if (OutArea())
	{
		Teleport(MiddleTop);
		m_eCurAnimState = EBossState::P1_IDLE;
		m_bPattern2Dash = false;
		m_bIdle = true;
	}

	// FSM 조건
	FSM(fTimeDelta);

	// 애니메이션 변경
	ChangeAnim();
	// 시간에 따른 애니메이션 끝내기
	AnimDuration(fTimeDelta);

	// 키 입력
	KeyInput();

	return iExit;
}

void CBoss2::LateUpdateGameObject()
{
	__super::LateUpdateGameObject();
}

void CBoss2::RenderGameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->GetWorld());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->SetAnimTexture();;
	m_pBufCom->RenderBuffer();
	m_pEffectBufCom->RenderBuffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	__super::RenderGameObject();
}

void CBoss2::AddComponent()
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

void CBoss2::FSM(const _float& fTimeDelta)
{
	// Idle상태
	if (m_eCurAnimState == EBossState::P1_IDLE)
	{
		if (m_bIdle)
		{
			m_fIdleTime -= fTimeDelta;
			if (m_iHp <= 100)
			{
				m_fIdleTime = 5.f;
				m_bIdle = false;
				m_bChange = true;
				m_eCurAnimState = EBossState::P1_CHANGE;
				m_iHp = 100;

				// 위치 이동
				Teleport(_vec3(300.f, 30.f, 300.f));
			}
			if (m_fIdleTime < 0.f)
			{
				m_fIdleTime = 5.f;
				m_bIdle = false;
			}
		}
		else if (!m_bIdle)
		{
			switch (rand() % 3)
			{
			case 0:
				m_bPattern1 = true;
				m_eCurAnimState = EBossState::P1_TELEPORT;
				break;
			case 1:
				m_bPattern2 = true;
				m_eCurAnimState = EBossState::P1_TELEPORT;
				break;
			case 2:
				m_bPattern3 = true;
				m_eCurAnimState = EBossState::P1_TELEPORT;
			}
		}
	}

	// Teleport상태
	if (m_eCurAnimState == EBossState::P1_TELEPORT)
	{
		if (m_bPattern1)
		{
			m_eCurAnimState = EBossState::P1_PATTERN1;
			Teleport(MiddleTop);
			m_bPattern1Before = true;
		}
		else if (m_bPattern2)
		{
			m_eCurAnimState = EBossState::P1_PATTERN2DASHREADY;
			switch (rand() % 4)
			{
			case 0:
				Teleport(MiddleTop);
				break;
			case 1:
				Teleport(RightTop);
				break;
			case 2:
				Teleport(LeftTop);
				break;
			case 3:
				Teleport(LeftBottom);
				break;
			}
			m_bPattern2DashReady = true;
		}
		else if (m_bPattern3)
		{
			m_eCurAnimState = EBossState::P1_PATTERN3LASERREADY;
			Teleport(MiddleTop);
			m_bPattern3LaserReady = true;
		}
		else
		{
			m_eCurAnimState = EBossState::P1_IDLE;
			switch (rand() % 4)
			{
			case 0:
				Teleport(Right);
				break;
			case 1:
				Teleport(Left);
				break;
			case 2:
				Teleport(Top);
				break;
			case 3:
				Teleport(Bottom);
				break;
			}
			m_bIdle = true;
		}
	}

	// Pattern1 상태
	if (m_eCurAnimState == EBossState::P1_PATTERN1)
	{
		if (m_bPattern1Before)
		{
			m_fPattern1BeforeTime -= fTimeDelta;
			if (m_fPattern1BeforeTime < 0.f)
			{
				m_fPattern1BeforeTime = 1.f;
				m_bPattern1Before = false;
				m_bPattern1Start = true;
			}
		}
		// 구체 4개 쏘기
		if (m_bPattern1Start)
		{
			ShootBullet4();
			m_bPattern1Start = false;
		}

		if (!m_bPattern1Ready)
		{
			m_fPattern1ReadyTime -= fTimeDelta;
			if (m_fPattern1ReadyTime < 0.f)
			{
				m_fPattern1ReadyTime = 4.f;
				m_bPattern1Ready = true;
			}
		}

		// 발사 준비 완료
		if (m_bPattern1Ready)
		{
			m_eCurAnimState = EBossState::P1_PATTERN1FIRE;
			m_bPattern1Ready = false;
		}
	}

	// Pattern1 발사 상태
	if (m_eCurAnimState == EBossState::P1_PATTERN1FIRE)
	{
		m_fPattern1FireTime -= fTimeDelta;
		m_fPattern1FireIntervel -= fTimeDelta;
		if (m_fPattern1FireIntervel < 0.f)
		{
			m_fPattern1FireIntervel = 0.03f;
			ShootBullet3();
		}
		// 발사완료
		if (m_fPattern1FireTime < 0.f)
		{
			m_fPattern1FireTime = 3.f;
			m_eCurAnimState = EBossState::P1_TELEPORT;
			m_bPattern1 = false;
		}
	}

	// Pattern2 상태
	if (m_eCurAnimState == EBossState::P1_PATTERN2DASHREADY)
	{
		if (m_bPattern2DashReady)
		{
			m_fDashReadyTime -= fTimeDelta;
			if (m_fDashReadyTime < 0.f)
			{
				m_fDashReadyTime = 0.25f * 4;
				m_bPattern2DashReady = false;
				m_eCurAnimState = EBossState::P1_PATTERN2CHARGE;
				m_bPattern2DashCharge = true;
				
			}
		}
	}
	if (m_eCurAnimState == EBossState::P1_PATTERN2CHARGE)
	{
		if (m_bPattern2DashCharge)
		{
			m_fDashChargeTime -= fTimeDelta;
			if (m_fDashChargeTime < 0.f)
			{
				m_fDashChargeTime = 3.f;
				m_bPattern2DashCharge = false;
				m_eCurAnimState = EBossState::P1_PATTERN2DASH;
				m_bPattern2Dash = true;
			}
		}
	}
	if (m_eCurAnimState == EBossState::P1_PATTERN2DASH)
	{
		if (m_bPattern2Dash)
		{
			m_fDashTime -= fTimeDelta;
			ChasePlayer(fTimeDelta, m_fP1_DashSpeed);
			if (m_fDashTime < 0.f)
			{
				m_fDashTime = 0.06f * 11;
				m_bPattern2Dash = false;
				m_eCurAnimState = EBossState::P1_TELEPORT;
				m_bPattern2 = false;
			}
		}
	}

	// Pattern3 상태
	if (m_eCurAnimState == EBossState::P1_PATTERN3LASERREADY)
	{
		if (m_bPattern3LaserReady)
		{
			m_fPattern3LaserReadyTime -= fTimeDelta;
			if (m_fPattern3LaserReadyTime < 0.f)
			{
				m_fPattern3LaserReadyTime = 1.f;
				m_bPattern3LaserReady = false;
				m_eCurAnimState = EBossState::P1_PATTERN3LASER;
				m_bPattern3Laser = true;
			}
		}
	}

	if (m_eCurAnimState == EBossState::P1_PATTERN3LASER)
	{
		if (m_bPattern3Laser)
		{
			m_fPattern3LaserTime -= fTimeDelta;
			if (m_fPattern3LaserTime < 0.f)
			{
				m_fPattern3LaserTime = 0.12 * 25;
				m_bPattern3Laser = false;
				m_eCurAnimState = EBossState::P1_TELEPORT;
				m_bPattern3 = false;
			}
		}
	}

	// 페이즈2로
	if (m_eCurAnimState == EBossState::P1_CHANGE)
	{
		m_bPhase2 = true;
		if (m_bChange)
		{
			m_fP1ChangeTime -= fTimeDelta;
			if (m_fP1ChangeTime < 0.f)
			{
				m_fP1ChangeTime = 0.06f * 43;
				m_bChange = false;
				m_eCurAnimState = EBossState::P2_IDLE;
				m_bPhase2Idle = true;
			}
		}
	}

	// 페이즈2 Idle
	if (m_eCurAnimState == EBossState::P2_IDLE)
	{
		m_fP2IdleTime -= fTimeDelta;
		if (m_fP2IdleTime < 0.f)
		{
			m_fP2IdleTime = 3.f;
			m_bPhase2Idle = false;
			switch (rand() % 1)
			{
			case 0:
				m_eCurAnimState = EBossState::P2_SUMMON;
				m_bPhase2Summon = true;
				break;
			case 1:
				m_eCurAnimState = EBossState::P2_SUNKEN;
				m_bPhase2Sunken = true;
				break;
			case 2:
				m_eCurAnimState = EBossState::P2_SPIKE;
				m_bPhase2Spike = true;
				break;
			}
		}
	}

	// 페이즈2 Summon
	if (m_eCurAnimState == EBossState::P2_SUMMON)
	{
		m_fP2SummonTime -= fTimeDelta;
		if (m_fP2SummonTime < 0.f)
		{
			// 소환
			for (int i = 0; i < m_iMobTotalNum; i++)
			{
				if (!dynamic_pointer_cast<CGameObject>(m_pVecMob[i])->GetIsEnable())
				{
					dynamic_pointer_cast<CGameObject>(m_pVecMob[i])->SetEnable(true);
					break;
				}
			}
			m_fP2SummonTime = 3.f;
			m_bPhase2Summon = false;
			m_eCurAnimState = EBossState::P2_IDLE;
			m_bPhase2Idle = true;
		}
	}

	// 페이즈2 Sunken
	//if (m_eCurAnimState == EBossState::P2_SUMMON)
	//{
	//	m_fP2SummonTime -= fTimeDelta;
	//	if (m_fP2SummonTime < 0.f)
	//	{
	//		m_fP2SummonTime = 3.f;
	//		// 소환
	//		for (int i = 0; i < m_iMobTotalNum; i++)
	//		{
	//			if (!dynamic_pointer_cast<CGameObject>(m_pVecMob[i])->GetIsEnable())
	//			{
	//				dynamic_pointer_cast<CGameObject>(m_pVecMob[i])->SetEnable(true);
	//				break;
	//			}
	//		}
	//		m_bPhase2Summon = false;
	//		m_eCurAnimState = EBossState::P2_IDLE;
	//		m_bPhase2Idle = true;
	//	}
	//}


	// 원형탄
	if (m_eCurAnimState == EBossState::P1_ATTACK)
	{
		if (!m_bBullet1Fire)
		{
			m_fBullet1Intervel -= fTimeDelta;
			if (m_fBullet1Intervel < 0.f)
			{
				ShootBullet1();
				m_fBullet1Intervel = 0.5f;
				m_bBullet1Fire = true;
			}
		}

		if (!m_bBullet2Fire)
		{
			ShootBullet2();
			m_bBullet2Fire = true;
		}
		if (m_bBullet2Fire)
		{
			m_fBullet2Intervel -= fTimeDelta;
			if (m_fBullet2Intervel < 0.f)
			{
				m_fBullet2Intervel = 0.06f;
				m_bBullet2Fire = false;
			}
		}
	}

	// 레이저 발사
	if (m_eCurAnimState == EBossState::P1_LASER2)
	{
		if (!m_bLaserFire)
		{
			ShootLaser();
			m_bLaserFire = true;
		}
		//if (m_bLaserFire)
		//{
		//	m_fLaserIntervel -= fTimeDelta;
		//	if (m_fLaserIntervel < 0.f)
		//	{
		//		m_fLaserIntervel = 0.5f;
		//		m_bLaserFire = false;
		//	}
		//}
	}

	// 돌진 상태
	if (m_eCurAnimState == EBossState::P1_DASH && m_fP1DashTime < 0.06f * 6)
	{
		ChasePlayer(fTimeDelta, m_fP1_DashSpeed);
	}
}

void CBoss2::ChangeAnim()
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
		case EBossState::P1_TELEPORT:
			m_pTextureCom->SetAnimKey(L"Boss_Phase1_Idle", 0.06f);
			break;
		case EBossState::P1_PATTERN1:
			m_pTextureCom->SetAnimKey(L"Boss_Phase1_Idle", 0.06f);
			break;
		case EBossState::P1_PATTERN1FIRE:
			m_pTextureCom->SetAnimKey(L"Boss_Phase1_Attack", 0.06f);
			break;
		case EBossState::P1_PATTERN2DASHREADY:
			m_pTextureCom->SetAnimKey(L"Boss_Phase1_Dash_Ready", 0.06f);
			break;
		case EBossState::P1_PATTERN2CHARGE:
			m_pTextureCom->SetAnimKey(L"Boss_Phase1_Dash_Charge", 0.06f);
			break;
		case EBossState::P1_PATTERN2DASH:
			m_pTextureCom->SetAnimKey(L"Boss_Phase1_Dash", 0.06f);
			break;
		case EBossState::P1_PATTERN3LASERREADY:
			m_pTextureCom->SetAnimKey(L"Boss_Phase1_Idle", 0.06f);
			break;
		case EBossState::P1_PATTERN3LASER:
			m_pTextureCom->SetAnimKey(L"Boss_Phase1_Laser1", 0.12f);
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
		case EBossState::P2_IDLE:
			m_pTextureCom->SetAnimKey(L"Boss_Phase2_Idle", 0.06f);
			break;
		case EBossState::P2_DEATH:
			m_pTextureCom->SetAnimKey(L"Boss_Phase2_Death", 0.06f);
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

	m_pTransformCom->SetScale(40.f * 0.9f * fXpos, 35.f * 0.9f * fYpos, 40.f * 0.9f * fXpos);

}

void CBoss2::AnimDuration(const _float& fTimeDelta)
{
	if (m_eCurAnimState == EBossState::P1_ATTACK)
	{
		m_fP1AttackTime -= fTimeDelta;
		if (m_fP1AttackTime < 0.f)
		{
			m_fP1AttackTime = 0.06f * 12;
			m_eCurAnimState = EBossState::P1_IDLE;
			m_bBullet1Fire = false;
			m_bBullet2Fire = false;
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
			m_bLaserFire = false;
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
}

void CBoss2::KeyInput()
{
	//if (GetAsyncKeyState('2') & 0x8000) {
	//	m_eCurAnimState = EBossState::P1_ATTACK;
	//}
	//if (GetAsyncKeyState('3') & 0x8000) {
	//	m_eCurAnimState = EBossState::P1_LASER1;
	//}
	if (GetAsyncKeyState('4') & 0x8000) {
		m_iHp = 100;
	}

	if (GetAsyncKeyState('5') & 0x8000) {
		for (int i = 0; i < m_iMobTotalNum; i++)
		{
			if (!dynamic_pointer_cast<CGameObject>(m_pVecMob[i])->GetIsEnable())
			{
				dynamic_pointer_cast<CGameObject>(m_pVecMob[i])->SetEnable(true);
				break;
			}
		}
	}
}

void CBoss2::OnFloor(_float _fHeight)
{
	if (m_pTransformCom->GetPos()->y < _fHeight)
	{
		m_pTransformCom->SetPosition(m_vPos.x, _fHeight, m_vPos.z);
	}
}

void CBoss2::Teleport(_vec3 vPos)
{
	m_pTransformCom->SetPosition(vPos.x, vPos.y, vPos.z);
	m_vPos = vPos;
}

_bool CBoss2::OutArea()
{
	if (m_pTransformCom->GetPos()->x > 640.f)
	{
		return true;
	}
	if (m_pTransformCom->GetPos()->x < 0.f)
	{
		return true;
	}
	if (m_pTransformCom->GetPos()->z > 640.f)
	{
		return true;
	}
	if (m_pTransformCom->GetPos()->z < 0.f)
	{
		return true;
	}
	if (m_pTransformCom->GetPos()->y < 0.f)
	{
		return true;
	}
	if (m_pTransformCom->GetPos()->y > 640.f)
	{
		return true;
	}

	return false;
}

_float CBoss2::CalcDistance()
{
	_vec3 vPos;
	m_pTransformCom->GetInfo(INFO_POS, &vPos);

	_vec3 vPosPlayer;
	m_pPlayerTransformCom->GetInfo(INFO_POS, &vPosPlayer);

	_vec3 vDist = vPosPlayer - vPos;

	return D3DXVec3Length(&vDist);
}

_vec3 CBoss2::CalcDirection()
{
	_vec3 vPos;
	m_pTransformCom->GetInfo(INFO_POS, &vPos);

	_vec3 vPosPlayer;
	m_pPlayerTransformCom->GetInfo(INFO_POS, &vPosPlayer);

	_vec3 vDist = vPosPlayer - vPos;

	return vDist;
}

void CBoss2::ChasePlayer(const _float& fTimeDelta, float _fSpeed)
{
	_vec3		vDir;
	vDir = CalcDirection();
	D3DXVec3Normalize(&vDir, &vDir);
	//vDir.y = 0.f;
	m_pTransformCom->MoveForward(&vDir, fTimeDelta, _fSpeed);
}

void CBoss2::ShootBullet1()
{
	for (int i = 0; i < m_iBullet1TotalNum; i++)
	{
		if (!dynamic_pointer_cast<CGameObject>(m_pVecBullet1[i])->GetIsEnable())
		{
			dynamic_pointer_cast<CGameObject>(m_pVecBullet1[i])->SetEnable(true);
			shared_ptr<CTransform> pTransform = dynamic_pointer_cast<CTransform>(
				m_pVecBullet1[i]->GetComponent(L"Com_Transform", ID_DYNAMIC));
			pTransform->SetPosition(m_vPos.x, m_vPos.y, m_vPos.z);
			break;
		}
	}
}

void CBoss2::ShootBullet2()
{
	for (int i = 0; i < m_iBullet2TotalNum; i++)
	{
		if (!dynamic_pointer_cast<CGameObject>(m_pVecBullet2[i])->GetIsEnable())
		{
			dynamic_pointer_cast<CGameObject>(m_pVecBullet2[i])->SetEnable(true);
			shared_ptr<CTransform> pTransform = dynamic_pointer_cast<CTransform>(
				m_pVecBullet2[i]->GetComponent(L"Com_Transform", ID_DYNAMIC));
			pTransform->SetPosition(m_vPos.x, m_vPos.y, m_vPos.z);
			_vec3 vPosPlayer;
			m_pPlayerTransformCom->GetInfo(INFO_POS, &vPosPlayer);
			vPosPlayer = _vec3(vPosPlayer.x - 10.f + (rand() % 10) * 2.f, vPosPlayer.y - 10.f + (rand() % 10) * 2.f, vPosPlayer.z);
			m_pVecBullet2[i]->SetTarget(vPosPlayer);
			break;
		}
	}
}

void CBoss2::ShootBullet3()
{
	for (int i = 0; i < 50; i++)
	{
		if (!dynamic_pointer_cast<CGameObject>(m_pVecBullet2[i])->GetIsEnable())
		{
			dynamic_pointer_cast<CGameObject>(m_pVecBullet2[i])->SetEnable(true);
			shared_ptr<CTransform> pTransform = dynamic_pointer_cast<CTransform>(
				m_pVecBullet2[i]->GetComponent(L"Com_Transform", ID_DYNAMIC));
			pTransform->SetPosition(m_vPos.x, m_vPos.y, m_vPos.z);
			_vec3 vTarget;
			vTarget = _vec3(m_vPos.x + (-10.f + (rand() % 10) * 2.f) * 1280.f, m_vPos.y + (-10.f + (rand() % 10) * 2.f) * 1280.f, m_vPos.z + (-10.f + (rand() % 10) * 2.f) * 1280.f);
			m_pVecBullet2[i]->SetTarget(vTarget);
			break;
		}
	}
	for (int i = 50; i < 100; i++)
	{
		if (!dynamic_pointer_cast<CGameObject>(m_pVecBullet2[i])->GetIsEnable())
		{
			dynamic_pointer_cast<CGameObject>(m_pVecBullet2[i])->SetEnable(true);
			shared_ptr<CTransform> pTransform = dynamic_pointer_cast<CTransform>(
				m_pVecBullet2[i]->GetComponent(L"Com_Transform", ID_DYNAMIC));
			pTransform->SetPosition(Right.x, Right.y, Right.z);
			_vec3 vTarget;
			vTarget = Right + _vec3((-10.f + (rand() % 10) * 2.f) * 1280.f,(-10.f + (rand() % 10) * 2.f) * 1280.f,(-10.f + (rand() % 10) * 2.f) * 1280.f);
			m_pVecBullet2[i]->SetTarget(vTarget);
			break;
		}
	}
	for (int i = 100; i < 150; i++)
	{
		if (!dynamic_pointer_cast<CGameObject>(m_pVecBullet2[i])->GetIsEnable())
		{
			dynamic_pointer_cast<CGameObject>(m_pVecBullet2[i])->SetEnable(true);
			shared_ptr<CTransform> pTransform = dynamic_pointer_cast<CTransform>(
				m_pVecBullet2[i]->GetComponent(L"Com_Transform", ID_DYNAMIC));
			pTransform->SetPosition(Left.x, Left.y, Left.z);
			_vec3 vTarget;
			vTarget = Left + _vec3((-10.f + (rand() % 10) * 2.f) * 1280.f, (-10.f + (rand() % 10) * 2.f) * 1280.f, (-10.f + (rand() % 10) * 2.f) * 1280.f);
			m_pVecBullet2[i]->SetTarget(vTarget);
			break;
		}
	}
	for (int i = 150; i < 200; i++)
	{
		if (!dynamic_pointer_cast<CGameObject>(m_pVecBullet2[i])->GetIsEnable())
		{
			dynamic_pointer_cast<CGameObject>(m_pVecBullet2[i])->SetEnable(true);
			shared_ptr<CTransform> pTransform = dynamic_pointer_cast<CTransform>(
				m_pVecBullet2[i]->GetComponent(L"Com_Transform", ID_DYNAMIC));
			pTransform->SetPosition(Top.x, Top.y, Top.z);
			_vec3 vTarget;
			vTarget = Top + _vec3((-10.f + (rand() % 10) * 2.f) * 1280.f, (-10.f + (rand() % 10) * 2.f) * 1280.f, (-10.f + (rand() % 10) * 2.f) * 1280.f);
			m_pVecBullet2[i]->SetTarget(vTarget);
			break;
		}
	}
	for (int i = 200; i < 250; i++)
	{
		if (!dynamic_pointer_cast<CGameObject>(m_pVecBullet2[i])->GetIsEnable())
		{
			dynamic_pointer_cast<CGameObject>(m_pVecBullet2[i])->SetEnable(true);
			shared_ptr<CTransform> pTransform = dynamic_pointer_cast<CTransform>(
				m_pVecBullet2[i]->GetComponent(L"Com_Transform", ID_DYNAMIC));
			pTransform->SetPosition(Bottom.x, Bottom.y, Bottom.z);
			_vec3 vTarget;
			vTarget = Bottom + _vec3((-10.f + (rand() % 10) * 2.f) * 1280.f, (-10.f + (rand() % 10) * 2.f) * 1280.f, (-10.f + (rand() % 10) * 2.f) * 1280.f);
			m_pVecBullet2[i]->SetTarget(vTarget);
			break;
		}
	}
}

void CBoss2::ShootBullet4()
{
	for (int i = 0; i < 4; i++)
	{
		if (!dynamic_pointer_cast<CGameObject>(m_pVecBullet3[i])->GetIsEnable())
		{
			dynamic_pointer_cast<CGameObject>(m_pVecBullet3[i])->SetEnable(true);
			shared_ptr<CTransform> pTransform = dynamic_pointer_cast<CTransform>(
				m_pVecBullet3[i]->GetComponent(L"Com_Transform", ID_DYNAMIC));
			pTransform->SetPosition(m_vPos.x, m_vPos.y, m_vPos.z);
			_vec3 vTarget;
			switch (i)
			{
			case 0:
				vTarget = Right;
				break;
			case 1:
				vTarget = Left;
				break;
			case 2:
				vTarget = Top;
				break;
			case 3:
				vTarget = Bottom;
				break;
			}
			m_pVecBullet3[i]->SetTarget(vTarget);
		}
	}
}

void CBoss2::ShootLaser()
{
	for (int i = 0; i < m_iLaserTotalNum; i++)
	{
		if (!dynamic_pointer_cast<CGameObject>(m_pVecLaser[i])->GetIsEnable())
		{
			dynamic_pointer_cast<CGameObject>(m_pVecLaser[i])->SetEnable(true);
			shared_ptr<CTransform> pTransform = dynamic_pointer_cast<CTransform>(
				m_pVecLaser[i]->GetComponent(L"Com_Transform", ID_DYNAMIC));
			_vec3 vPosition = m_vPos;
			_vec3 vDirX;
			_vec3 vDirY;
			m_pTransformCom->GetInfo(INFO_RIGHT, &vDirX);
			m_pTransformCom->GetInfo(INFO_UP, &vDirY);
			D3DXVec3Normalize(&vDirX, &vDirX);
			D3DXVec3Normalize(&vDirY, &vDirY);
			switch (i)
			{
				// 윗 눈
			case 0:
				vPosition += vDirY * 7.f;
				pTransform->SetPosition(vPosition.x, vPosition.y, vPosition.z);
				pTransform->SetAngle(_vec3(PI * 0.3f, 0.f, 0.f));
				break;
				// 아래 왼 눈
			case 1:
				vPosition -= vDirX * 8.f;
				pTransform->SetPosition(vPosition.x, vPosition.y, vPosition.z);
				pTransform->SetAngle(_vec3(PI * 0.3f, 0.f, 0.f));
				break;
				// 아래 오른 눈
			case 2:
				vPosition += vDirX * 8.f;
				pTransform->SetPosition(vPosition.x, vPosition.y, vPosition.z);
				pTransform->SetAngle(_vec3(PI * 0.3f, 0.f, 0.f));
				break;
			}
			_vec3 vPosPlayer;
			m_pPlayerTransformCom->GetInfo(INFO_POS, &vPosPlayer);
			vPosPlayer = _vec3(vPosPlayer.x - 10.f + (rand() % 10) * 2.f, vPosPlayer.y - 10.f + (rand() % 10) * 2.f, vPosPlayer.z);
			m_pVecLaser[i]->SetTarget(vPosPlayer);
		}
	}
}
