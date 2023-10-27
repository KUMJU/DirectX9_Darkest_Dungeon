#include "pch.h"
#include "Jester.h"
#include"Export_Utility.h"

CJester::CJester(LPDIRECT3DDEVICE9 pGraphicDev)
	: CHero(pGraphicDev)
{
}

CJester::CJester(const CJester& rhs)
	: CHero(rhs)
{
}

CJester::~CJester()
{
}

HRESULT CJester::ReadyGameObject()
{
	__super::ReadyGameObject();

	// 스킬 세팅
	{
		_int  DotDamZero[2] = { 0, 0 };

		// 단검 찌르기
		_bool arrActivatePos1[4] = { 1, 1, 1, 1 };
		_bool arrTargetPos1[4] = { 1, 1, 1, 0 };
		_bool arrAttack1[6] = { 1, 0, 0, 0, 1, 0 };
		_bool arrToEnemy1[6] = { 1, 1, 1, 1, 0, 1 };

		shared_ptr<CSkill> pSkill1 = make_shared<CSkill>
			(L"DirkStab", L"Jester_DirkStab", L"DirkStab_Img", L"DirkStab_Effect",
				arrActivatePos1, arrTargetPos1, arrAttack1, arrToEnemy1, DotDamZero, 1.f, 1.f, 1.05f, -1, -1);

		// 수확의 낫질
		_bool arrActivatePos2[4] = { 0, 1, 1, 0 };
		_bool arrTargetPos2[4] = { 0, 1, 1, 0 };
		_bool arrAttack2[6] = { 1, 0, 1, 0, 0, 0 };
		_bool arrToEnemy2[6] = { 1, 1, 1, 1, 1, 1 };

		_int  DotDam2[2] = { 2, 3 };

		shared_ptr<CSkill> pSkill2 = make_shared<CSkill>
			(L"Harvest", L"Jester_Harvest", L"Harvest_Img", L"Harvest_Effect",
				arrActivatePos2, arrTargetPos2, arrAttack2, arrToEnemy2, DotDam2, 1.f, 0.5f, 1.f, -1, 0, 0, true);

		// 대단원
		_bool arrActivatePos3[4] = { 1, 1, 0, 0 };
		_bool arrTargetPos3[4] = { 1, 1, 1, 1 };
		_bool arrAttack3[6] = { 1, 0, 0, 0, 1, 0 };
		_bool arrToEnemy3[6] = { 1, 1, 1, 1, 0, 1 };

		shared_ptr<CSkill> pSkill3 = make_shared<CSkill>
			(L"Finale", L"Jester_Final", L"Finale_Img", L"Finale_Effect",
				arrActivatePos3, arrTargetPos3, arrAttack3, arrToEnemy3, DotDamZero, 1.f, 1.5f, 1.05f, 1, 3);

		// 독주
		_bool arrActivatePos4[4] = { 0, 0, 1, 1 };
		_bool arrTargetPos4[4] = { 1, 1, 1, 1 };
		_bool arrAttack4[6] = { 0, 0, 0, 0, 1, 0 };
		_bool arrToEnemy4[6] = { 1, 1, 1, 1, 0, 1 };

		shared_ptr<CSkill> pSkill4 = make_shared<CSkill>
			(L"Solo", L"Jester_Solo", L"Solo_Img", L"Solo_Effect",
				arrActivatePos4, arrTargetPos4, arrAttack4, arrToEnemy4, DotDamZero, 1.f, 1.f, 1.f, 2, -3, 0);

		// 저며내기
		_bool arrActivatePos5[4] = { 0, 1, 1, 0 };
		_bool arrTargetPos5[4] = { 0, 1, 1, 0 };
		_bool arrAttack5[6] = { 1, 0, 1, 0, 0, 0 };
		_bool arrToEnemy5[6] = { 1, 1, 1, 1, 1, 1 };

		_int  DotDam5[2] = { 3, 3 };

		shared_ptr<CSkill> pSkill5 = make_shared<CSkill>
			(L"SliceOff", L"Jester_SliceOff", L"SliceOff_Img", L"SliceOff_Effect",
				arrActivatePos5, arrTargetPos5, arrAttack5, arrToEnemy5, DotDam5, 1.f, 0.66f, 1.08f, -1, 0, 0);

		m_pVecSkill.push_back(pSkill1);
		m_pVecSkill.push_back(pSkill2);
		m_pVecSkill.push_back(pSkill3);
		m_pVecSkill.push_back(pSkill4);
		m_pVecSkill.push_back(pSkill5);
	}

	// 영웅 스탯
	{
		m_tCommonStat.iHp = 19;
		m_tCommonStat.iDodge = 15;
		m_tCommonStat.iSpeed = 7;
		m_tCommonStat.iAttackPower = 6;
		m_tCommonStat.iOrder = 0;
	}

	// 테스트용
	{
		m_eAnimState = EAnimState::IDLE;
		m_vAngle = { 0.f, PI / 2, 0.f };

		m_pTransformCom->SetPosition(m_vPos.x, m_vPos.y, m_vPos.z);
		m_pTransformCom->SetScale(2.f, 3.f, 1.f);

		m_pTransformCom->SetAngle(m_vAngle);
		m_pTransformCom->Rotation(ROT_Y, PI / 2.f);

		m_pTextureCom->SetAnimKey(L"Jester_Idle", 0.04f);
	}

	return S_OK;
}

_int CJester::UpdateGameObject(const _float& fTimeDelta)
{
	Engine::AddRenderGroup(RENDER_ALPHA, shared_from_this());

	_int iExit = __super::UpdateGameObject(fTimeDelta);

	ChangeAnim();

	return iExit;
}

void CJester::LateUpdateGameObject()
{
	__super::LateUpdateGameObject();
}

void CJester::RenderGameObject()
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

void CJester::AddComponent()
{
	__super::AddComponent();
}

void CJester::ChangeAnim()
{
	// Animation
	if (m_eCurAnimState != m_ePrevAnimState)
	{
		switch (m_eCurAnimState)
		{
		case EAnimState::IDLE:
			m_pTextureCom->SetAnimKey(L"Jester_Idle", 0.04f);
			m_pTransformCom->SetScale(2.f, 3.f, 1.f);
			break;
		case EAnimState::WALK:
			m_pTextureCom->SetAnimKey(L"Jester_Walk", 0.04f);
			m_pTransformCom->SetScale(2.f, 3.f, 1.f);
			break;
		case EAnimState::COMBAT:
			m_pTextureCom->SetAnimKey(L"Jester_Combat", 0.04f);
			m_pTransformCom->SetScale(2.f, 3.f, 1.f);
			break;
		case EAnimState::BESHOT:
			m_pTextureCom->SetAnimKey(L"Jester_Defend", 0.04f);
			m_pTransformCom->SetScale(2.f, 3.f, 1.f);
			break;
		case EAnimState::SKILL1:
			m_pTextureCom->SetAnimKey(L"Jester_DirkStab", 0.04f);
			m_pTransformCom->SetScale(2.f, 3.f, 1.f);
			break;
		case EAnimState::SKILL2:
			m_pTextureCom->SetAnimKey(L"Jester_Harvest", 0.04f);
			m_pTransformCom->SetScale(2.f, 3.f, 1.f);
			break;
		case EAnimState::SKILL3:
			m_pTextureCom->SetAnimKey(L"Jester_Finale", 0.04f);
			m_pTransformCom->SetScale(2.f, 3.f, 1.f);
			break;
		case EAnimState::SKILL4:
			m_pTextureCom->SetAnimKey(L"Jester_Solo", 0.04f);
			m_pTransformCom->SetScale(2.f, 3.f, 1.f);
			break;
		case EAnimState::AFFLICTION:
			m_pTextureCom->SetAnimKey(L"Jester_Affliction", 0.04f);
			m_pTransformCom->SetScale(2.f, 3.f, 1.f);
			break;
		case EAnimState::VIRTUE:
			m_pTextureCom->SetAnimKey(L"Jester_Virtue", 0.04f);
			m_pTransformCom->SetScale(2.f, 3.f, 1.f);
			break;
		}
	}
}