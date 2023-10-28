#include "pch.h"
#include "ShieldBreaker.h"
#include"Export_Utility.h"

CShieldBreaker::CShieldBreaker(LPDIRECT3DDEVICE9 pGraphicDev) : CHero(pGraphicDev)
{
}

CShieldBreaker::CShieldBreaker(const CShieldBreaker& rhs) : CHero(rhs)
{
}

CShieldBreaker::~CShieldBreaker()
{
}

HRESULT CShieldBreaker::ReadyGameObject()
{
	__super::ReadyGameObject();

	m_bIsHero = true;

	// 스킬 세팅
	{
		_int  DotDamZero[2] = { 0, 0 };

		// 찌르기
		_bool arrActivatePos1[4] = { 1, 1, 1, 0 };
		_bool arrTargetPos1[4] = { 1, 1, 1, 1 };
		_bool arrAttack1[6] = { 1, 0, 0, 0, 0, 0 };
		_bool arrToEnemy1[6] = { 1, 1, 1, 1, 0, 1 };

		/*
		영웅 스킬 용
		(스킬 이름, 스킬 애니메이션, 스킬 이미지, 이펙트키, 활성 위치, 타겟 위치, 공격종류(단순 공격, 중독, 출혈, 기절, 이동, 힐),
		각 공격 타겟이 적인지 아닌지, 도트뎀[데미지][라운드], 스턴 확률, 공격력 계수, 치명타 확률, 라운드 당 사용 가능 횟수, 이동량, 힐량, 타겟 대상을 모두 공격하는지)
		*/

		shared_ptr<CSkill> pSkill1 = make_shared<CSkill>
			(L"Pierce", L"ShieldBreaker_Pierce", L"Pierce_Img", L"Pierce_Effect",
				arrActivatePos1, arrTargetPos1, arrAttack1, arrToEnemy1, DotDamZero, 1.f, 0.9f, 1.05f, -1, 0, 0, 0, 1, 5);

		// 구멍 내기
		_bool arrActivatePos2[4] = { 1, 1, 1, 1 };
		_bool arrTargetPos2[4] = { 1, 1, 0, 0 };
		_bool arrAttack2[6] = { 1, 0, 0, 0, 0, 0 };
		_bool arrToEnemy2[6] = { 1, 1, 1, 1, 1, 1 };

		shared_ptr<CSkill> pSkill2 = make_shared<CSkill>
			(L"Puncture", L"ShieldBreaker_Puncture", L"Puncture_Img", L"Puncture_Effect",
				arrActivatePos2, arrTargetPos2, arrAttack2, arrToEnemy2, DotDamZero, 1.f, 0.5f, 1.f, -1, 1,0,0,1, 5);

		// 독사의 입맞춤
		_bool arrActivatePos3[4] = { 1, 0, 0, 0 };
		_bool arrTargetPos3[4] = { 1, 1, 0, 0 };
		_bool arrAttack3[6] = { 0, 1, 0, 0, 0, 0 };
		_bool arrToEnemy3[6] = { 1, 1, 1, 1, 0, 1 };

		_int DotDam3[2] = { 3, 3 };

		shared_ptr<CSkill> pSkill3 = make_shared<CSkill>
			(L"AddersKiss", L"ShieldBreaker_AddersKiss", L"AddersKiss_Img", L"AddersKiss_Effect",
				arrActivatePos3, arrTargetPos3, arrAttack3, arrToEnemy3, DotDam3, 1.f, 1.f, 1.06f, -1, 0, 0, 0, 1, 5);

		// 꿰뚫기
		_bool arrActivatePos4[4] = { 1, 0, 0, 0 };
		_bool arrTargetPos4[4] = { 1, 1, 1, 1 };
		_bool arrAttack4[6] = { 1, 0, 0, 0, 0, 0 };
		_bool arrToEnemy4[6] = { 1, 1, 1, 1, 0, 1 };

		shared_ptr<CSkill> pSkill4 = make_shared<CSkill>
			(L"Impale", L"ShieldBreaker_Impale", L"Impale_Img", L"Impale_Effect",
				arrActivatePos4, arrTargetPos4, arrAttack4, arrToEnemy4, DotDamZero, 1.f, 0.4f, 0.94f, -1, 0, 0, 1, 0, 5);

		m_pVecSkill.push_back(pSkill1);
		m_pVecSkill.push_back(pSkill2);
		m_pVecSkill.push_back(pSkill3);
		m_pVecSkill.push_back(pSkill4);
	}

	// 영웅 스탯
	{
		m_tCommonStat.iHp = 50;
		m_tCommonStat.iDodge = 8;
		m_tCommonStat.iSpeed = 5;
		m_tCommonStat.iAttackPower = 8;
		m_tCommonStat.iOrder = 0;
	}

	// 테스트용
	{
		m_eAnimState = EAnimState::IDLE;
		//m_vAngle = { 0.f, PI / 2, 0.f };

		m_pTransformCom->SetPosition(m_vPos.x, m_vPos.y, m_vPos.z);
		m_pTransformCom->SetScale(2.f, 3.f, 1.f);

		m_pTransformCom->SetAngle(m_vAngle);
		m_pTransformCom->Rotation(ROT_Y, PI / 2.f);

		m_pTextureCom->SetAnimKey(L"ShieldBreaker_Idle", 0.04f);
	}

	return S_OK;
}

_int CShieldBreaker::UpdateGameObject(const _float& fTimeDelta)
{
	_int iExit = __super::UpdateGameObject(fTimeDelta);

	Engine::AddRenderGroup(RENDER_ALPHA, shared_from_this());

	ChangeAnim();

	if ((IsAttacking()))
	{
		for (int i = 0; i < 7; i++)
		{
			if (GetAttacking(i))
			{	
				switch (i)
				{
				case 0:
					m_eCurAnimState = EAnimState::SKILL1;
					break;
				case 1:
					m_eCurAnimState = EAnimState::SKILL2;
					break;
				case 2:
					m_eCurAnimState = EAnimState::SKILL3;
					break;
				case 3:
					m_eCurAnimState = EAnimState::SKILL4;
					break;
				case 4:
					m_eCurAnimState = EAnimState::SKILL5;
					break;
				case 5:
					m_eCurAnimState = EAnimState::SKILL6;
					break;
				case 6:
					m_eCurAnimState = EAnimState::SKILL7;
					break;
				}
				break;
			}

		}
	}
	else if (m_bDeath)
	{
		m_ePrevAnimState = m_eCurAnimState;
		m_eCurAnimState = EAnimState::DEATH;
	}
	else if (m_bHitted == true)
	{
		m_ePrevAnimState = m_eCurAnimState;
		m_eCurAnimState = EAnimState::BESHOT;
	}
	else
	{
		m_ePrevAnimState = m_eCurAnimState;
		m_eCurAnimState = EAnimState::COMBAT;
	}

	// 사망 여부
	if (m_tCommonStat.iHp <= 0)
	{
		m_bDeath = true;
		m_tCommonStat.iHp = -100;
	}

	// 피격 시간
	if (m_bHitted)
	{
		m_fHittedTime -= fTimeDelta;
		if (m_fHittedTime < 0.f)
		{
			m_bHitted = false;
			m_fHittedTime = HITTEDTIME;
		}
	}

	// 공격1 시간
	if (GetAttacking(0))
	{
		m_fAttackTime -= fTimeDelta;
		if (m_fAttackTime < 0.f)
		{
			SetAttacking(false, 0);
			m_fAttackTime = ATTACKTIME;
		}
	}

	// 공격2 시간
	if (GetAttacking(1))
	{
		m_fAttackTime -= fTimeDelta;
		if (m_fAttackTime < 0.f)
		{
			SetAttacking(false, 1);
			m_fAttackTime = ATTACKTIME;
		}
	}

	// 공격3 시간
	if (GetAttacking(2))
	{
		m_fAttackTime -= fTimeDelta;
		if (m_fAttackTime < 0.f)
		{
			SetAttacking(false, 2);
			m_fAttackTime = ATTACKTIME;
		}
	}

	// 공격4 시간
	if (GetAttacking(3))
	{
		m_fAttackTime -= fTimeDelta;
		if (m_fAttackTime < 0.f)
		{
			SetAttacking(false, 3);
			m_fAttackTime = ATTACKTIME;
		}
	}

	return iExit;
}

void CShieldBreaker::LateUpdateGameObject()
{
	__super::LateUpdateGameObject();
}

void CShieldBreaker::RenderGameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->GetWorld());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	// m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	m_pTextureCom->SetAnimTexture();
	m_pBufCom->RenderBuffer();
	m_pEffectBufCom->RenderBuffer();

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	__super::RenderGameObject();
}

void CShieldBreaker::AddComponent()
{
	__super::AddComponent();
}

void CShieldBreaker::ChangeAnim()
{
	// Animation
	if (m_eCurAnimState != m_ePrevAnimState)
	{
		switch (m_eCurAnimState)
		{
		case EAnimState::IDLE:
			m_pTextureCom->SetAnimKey(L"ShieldBreaker_Idle", 0.04f);
			m_pTransformCom->SetScale(2.f, 3.f, 1.f);
			break;
		case EAnimState::WALK:
			m_pTextureCom->SetAnimKey(L"ShieldBreaker_Walk", 0.04f);
			m_pTransformCom->SetScale(2.f, 3.f, 1.f);
			break;
		case EAnimState::COMBAT:
			m_pTextureCom->SetAnimKey(L"ShieldBreaker_Combat", 0.04f);
			m_pTransformCom->SetScale(2.f, 3.f, 1.f);
			break;
		case EAnimState::BESHOT:
			m_pTextureCom->SetAnimKey(L"ShieldBreaker_Defend", 0.04f);
			m_pTransformCom->SetScale(2.f, 3.f, 1.f);
			break;
		case EAnimState::SKILL1:
			m_pTextureCom->SetAnimKey(L"ShieldBreaker_Pierce", 0.04f);
			m_pTransformCom->SetScale(2.f, 3.f, 1.f);
			break;
		case EAnimState::SKILL2:
			m_pTextureCom->SetAnimKey(L"ShieldBreaker_Puncture", 0.04f);
			m_pTransformCom->SetScale(2.f, 3.f, 1.f);
			break;
		case EAnimState::SKILL3:
			m_pTextureCom->SetAnimKey(L"ShieldBreaker_AddersKiss", 0.04f);
			m_pTransformCom->SetScale(2.f, 3.f, 1.f);
			break;
		case EAnimState::SKILL4:
			m_pTextureCom->SetAnimKey(L"ShieldBreaker_Impale", 0.04f);
			m_pTransformCom->SetScale(2.f, 3.f, 1.f);
			break;
		case EAnimState::AFFLICTION:
			m_pTextureCom->SetAnimKey(L"ShieldBreaker_Affliction", 0.04f);
			m_pTransformCom->SetScale(2.f, 3.f, 1.f);
			break;
		case EAnimState::VIRTUE:
			m_pTextureCom->SetAnimKey(L"ShieldBreaker_Virtue", 0.04f);
			m_pTransformCom->SetScale(2.f, 3.f, 1.f);
			break;
		case EAnimState::DEATH:
			m_pTextureCom->SetAnimKey(L"Hero_Death", 0.02f);
			m_pTransformCom->SetScale(3.f, 3.f, 1.f);
			break;
		}
	}
}