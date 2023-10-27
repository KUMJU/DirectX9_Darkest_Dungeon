#include "pch.h"
#include "Vestal.h"
#include"Export_Utility.h"

CVestal::CVestal(LPDIRECT3DDEVICE9 pGraphicDev)
	: CHero(pGraphicDev)
{
}

CVestal::CVestal(const CVestal& rhs)
	: CHero(rhs)
{
}

CVestal::~CVestal()
{
}

HRESULT CVestal::ReadyGameObject()
{
	__super::ReadyGameObject();

	// ½ºÅ³ ¼¼ÆÃ
	{
		_int  DotDamZero[2] = { 0, 0 };

		_bool arrActivatePos1[4] = { 1, 1, 0, 0 };
		_bool arrTargetPos1[4] = { 1, 1, 0, 0 };
		_bool arrAttack1[6] = { 1, 0, 0, 0, 0, 0 };
		_bool arrToEnemy1[6] = { 1, 0, 0, 0, 0, 0 };


		// Ã¶Åð ÈÄ·ÁÄ¡±â
		shared_ptr<CSkill> pSkill1 = make_shared<CSkill>
			(L"MaceBash", L"Vestal_MaceBash", L"MaceBash_Img", L"MaceBash_Effect",
				arrActivatePos1, arrTargetPos1, arrAttack1, arrToEnemy1, DotDamZero, 1.f, 1.15f);

		_bool arrActivatePos2[4] = { 0, 1, 1, 1 };
		_bool arrTargetPos2[4] = { 1, 1, 1, 0 };
		_bool arrAttack2[6] = { 1, 0, 0, 1, 0, 0 };
		_bool arrToEnemy2[6] = { 1, 0, 0, 0, 0, 0 };

		// ´«ºÎ½Å ±¤Ã¤
		shared_ptr<CSkill> pSkill2 = make_shared<CSkill>
			(L"DazzlingLight", L"Vestal_DazzlingLight", L"DazzlingLight_Img", L"DazzlingLight_Effect",
				arrActivatePos2, arrTargetPos2, arrAttack2, arrToEnemy2, DotDamZero, 1.f, 0.25f);

		_bool arrActivatePos3[4] = { 0, 0, 1, 1 };
		_bool arrTargetPos3[4] = { 1, 1, 1, 1 };
		_bool arrAttack3[6] = { 0, 0, 0, 0, 0, 1 };
		_bool arrToEnemy3[6] = { 1, 1, 1, 1, 1, 0 };

		// ½Å¼ºÇÑ ÀºÃÑ
		shared_ptr<CSkill> pSkill3 = make_shared<CSkill>
			(L"DivineGrace", L"Vestal_DivineGrace", L"DivineGrace_Img", L"DivineGrace_Effect",
				arrActivatePos3, arrTargetPos3, arrAttack3, arrToEnemy3, DotDamZero, 1.f, 1.f, 1.f, -1, 0, 4);

		_bool arrActivatePos4[4] = { 0, 1, 1, 1 };
		_bool arrTargetPos4[4] = { 1, 1, 1, 1 };
		_bool arrAttack4[6] = { 0, 0, 0, 0, 0, 1 };
		_bool arrToEnemy4[6] = { 1, 1, 1, 1, 1, 0 };

		// ½Å¼ºÇÑ À§¹«
		shared_ptr<CSkill> pSkill4 = make_shared<CSkill>
			(L"DevineComfort", L"Vestal_DevineComfort", L"DevineComfort_Img", L"DevineComfort_Effect",
				arrActivatePos3, arrTargetPos3, arrAttack3, arrToEnemy4, DotDamZero, 1.f, 1.f, 1.f, -1, 0, 4);

		m_pVecSkill.push_back(pSkill1);
		m_pVecSkill.push_back(pSkill2);
		m_pVecSkill.push_back(pSkill3);
		m_pVecSkill.push_back(pSkill4);
	}

	// ¿µ¿õ ½ºÅÈ
	{
		m_tCommonStat.iHp = 24;
		m_tCommonStat.iDodge = 0;
		m_tCommonStat.iSpeed = 4;
		m_tCommonStat.iAttackPower = 6;
		m_tCommonStat.iOrder = 0;
	}

	// Å×½ºÆ®¿ë
	{
		m_eAnimState = EAnimState::IDLE;
		m_vAngle = { 0.f, PI / 2, 0.f };

		m_pTransformCom->SetPosition(m_vPos.x, m_vPos.y, m_vPos.z);
		m_pTransformCom->SetScale(2.f, 3.f, 1.f);

		m_pTransformCom->SetAngle(m_vAngle);
		m_pTransformCom->Rotation(ROT_Y, PI / 2.f);

		m_pTextureCom->SetAnimKey(L"Vestal_Idle", 0.04f);
	}

	return S_OK;
}

_int CVestal::UpdateGameObject(const _float& fTimeDelta)
{
	Engine::AddRenderGroup(RENDER_ALPHA, shared_from_this());

	_int iExit = __super::UpdateGameObject(fTimeDelta);

	ChangeAnim();

	return iExit;
}

void CVestal::LateUpdateGameObject()
{
	__super::LateUpdateGameObject();
}

void CVestal::RenderGameObject()
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

void CVestal::AddComponent()
{

	__super::AddComponent();
}

void CVestal::ChangeAnim()
{
	// Animation
	if (m_eCurAnimState != m_ePrevAnimState)
	{
		switch (m_eCurAnimState)
		{
		case EAnimState::IDLE:
			m_pTextureCom->SetAnimKey(L"Vestal_Idle", 0.04f);
			m_pTransformCom->SetScale(2.f, 3.f, 1.f);
			break;
		case EAnimState::WALK:
			m_pTextureCom->SetAnimKey(L"Vestal_Walk", 0.04f);
			m_pTransformCom->SetScale(2.f, 3.f, 1.f);
			break;
		case EAnimState::COMBAT:
			m_pTextureCom->SetAnimKey(L"Vestal_Combat", 0.04f);
			m_pTransformCom->SetScale(2.f, 3.f, 1.f);
			break;
		case EAnimState::BESHOT:
			m_pTextureCom->SetAnimKey(L"Vestal_Defend", 0.04f);
			m_pTransformCom->SetScale(2.f, 3.f, 1.f);
			break;
		case EAnimState::SKILL1:
			m_pTextureCom->SetAnimKey(L"Vestal_MaceBash", 0.04f);
			m_pTransformCom->SetScale(2.f, 3.f, 1.f);
			break;
		case EAnimState::SKILL2:
			m_pTextureCom->SetAnimKey(L"Vestal_DazzlingLight", 0.04f);
			m_pTransformCom->SetScale(2.f, 3.f, 1.f);
			break;
		case EAnimState::SKILL3:
			m_pTextureCom->SetAnimKey(L"Vestal_DivineGrace", 0.04f);
			m_pTransformCom->SetScale(2.f, 3.f, 1.f);
			break;
		case EAnimState::SKILL4:
			m_pTextureCom->SetAnimKey(L"Vestal_DivineComfort", 0.04f);
			m_pTransformCom->SetScale(2.f, 3.f, 1.f);
			break;
		case EAnimState::AFFLICTION:
			m_pTextureCom->SetAnimKey(L"Vestal_Affliction", 0.04f);
			m_pTransformCom->SetScale(2.f, 3.f, 1.f);
			break;
		case EAnimState::VIRTUE:
			m_pTextureCom->SetAnimKey(L"Vestal_Virtue", 0.04f);
			m_pTransformCom->SetScale(2.f, 3.f, 1.f);
			break;
		}
	}
}