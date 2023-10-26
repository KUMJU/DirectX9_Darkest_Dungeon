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

	// ��ų ����
	{
		_int  DotDamZero[2] = { 0, 0 };

		// ���
		_bool arrActivatePos1[4] = { 1, 1, 1, 0 };
		_bool arrTargetPos1[4] = { 1, 1, 1, 1 };
		_bool arrAttack1[6] = { 1, 0, 0, 0, 1, 0 };
		_bool arrToEnemy1[6] = { 1, 1, 1, 1, 0, 1 };

		/*
		���� ��ų ��
		(��ų �̸�, ��ų �ִϸ��̼�, ��ų �̹���, ����ƮŰ, Ȱ�� ��ġ, Ÿ�� ��ġ, ��������(�ܼ� ����, �ߵ�, ����, ����, �̵�, ��),
		�� ���� Ÿ���� ������ �ƴ���, ��Ʈ��[������][����], ���� Ȯ��, ���ݷ� ���, ġ��Ÿ Ȯ��, ���� �� ��� ���� Ƚ��, �̵���, ����, Ÿ�� ����� ��� �����ϴ���)
		*/

		shared_ptr<CSkill> pSkill1 = make_shared<CSkill>
			(L"Pierce", L"ShieldBreaker_Pierce", L"Pierce_Img", L"Pierce_Effect",
				arrActivatePos1, arrTargetPos1, arrAttack1, arrToEnemy1, DotDamZero, 1.f, 0.9f, 1.05f, -1, -1);

		// ���� ����
		_bool arrActivatePos2[4] = { 1, 1, 1, 1 };
		_bool arrTargetPos2[4] = { 1, 1, 1, 1 };
		_bool arrAttack2[6] = { 1, 0, 0, 0, 1, 0 };
		_bool arrToEnemy2[6] = { 1, 1, 1, 1, 1, 1 };

		shared_ptr<CSkill> pSkill2 = make_shared<CSkill>
			(L"Puncture", L"ShieldBreaker_Puncture", L"Puncture_Img", L"Puncture_Effect",
				arrActivatePos2, arrTargetPos2, arrAttack2, arrToEnemy2, DotDamZero, 1.f, 0.5f, 1.f, -1, -2);

		// ������ �Ը���
		_bool arrActivatePos3[4] = { 1, 0, 0, 0 };
		_bool arrTargetPos3[4] = { 1, 1, 0, 0 };
		_bool arrAttack3[6] = { 1, 1, 0, 0, 1, 0 };
		_bool arrToEnemy3[6] = { 1, 1, 1, 1, 0, 1 };

		_int DotDam3[2] = { 3, 3 };

		shared_ptr<CSkill> pSkill3 = make_shared<CSkill>
			(L"AddersKiss", L"ShieldBreaker_AddersKiss", L"AddersKiss_Img", L"AddersKiss_Effect",
				arrActivatePos3, arrTargetPos3, arrAttack3, arrToEnemy3, DotDam3, 1.f, 1.f, 1.06f, -1, 1);

		// ��ձ�
		_bool arrActivatePos4[4] = { 1, 0, 0, 0 };
		_bool arrTargetPos4[4] = { 1, 1, 1, 1 };
		_bool arrAttack4[6] = { 1, 0, 0, 0, 1, 0 };
		_bool arrToEnemy4[6] = { 1, 1, 1, 1, 0, 1 };

		shared_ptr<CSkill> pSkill4 = make_shared<CSkill>
			(L"Impale", L"ShieldBreaker_Impale", L"Impale_Img", L"Impale_Effect",
				arrActivatePos4, arrTargetPos4, arrAttack4, arrToEnemy4, DotDamZero, 1.f, 0.4f, 0.94f, -1, 1);

		m_pVecSkill.push_back(pSkill1);
		m_pVecSkill.push_back(pSkill2);
		m_pVecSkill.push_back(pSkill3);
		m_pVecSkill.push_back(pSkill4);
	}

	// ���� ����
	{
		m_tCommonStat.iHp = 20;
		m_tCommonStat.iDodge = 8;
		m_tCommonStat.iSpeed = 5;
		m_tCommonStat.iAttackPower = 8;
		m_tCommonStat.iOrder = 0;
	}

	// �׽�Ʈ��
	{
		m_eAnimState = EAnimState::IDLE;
		m_vAngle = { 0.f, PI / 2, 0.f };

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
	Engine::AddRenderGroup(RENDER_ALPHA, shared_from_this());

	_int iExit = __super::UpdateGameObject(fTimeDelta);

	ChangeAnim();

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
	// ���� �ִϸ��̼�
	{
		Engine::CreateNewTexture(L"ShieldBreaker_Combat", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/ShieldBreaker/Combat/armatureName_combat_%d.png", 13);
		Engine::CreateNewTexture(L"ShieldBreaker_Idle", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/ShieldBreaker/Idle/armatureName_idle_%d.png", 15);
		Engine::CreateNewTexture(L"ShieldBreaker_Walk", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/ShieldBreaker/Walk/armatureName_walk_%d.png", 16);

		Engine::CreateNewTexture(L"ShieldBreaker_Pierce", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/ShieldBreaker/Pierce.png", 1);
		Engine::CreateNewTexture(L"ShieldBreaker_Puncture", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/ShieldBreaker/Puncture.png", 1);
		Engine::CreateNewTexture(L"ShieldBreaker_AddersKiss", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/ShieldBreaker/AddersKiss.png", 1);
		Engine::CreateNewTexture(L"ShieldBreaker_Impale", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/ShieldBreaker/Impale.png", 1);

		Engine::CreateNewTexture(L"ShieldBreaker_Defend", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/ShieldBreaker/Defend.png", 1);
		Engine::CreateNewTexture(L"ShieldBreaker_Affliction", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/ShieldBreaker/Afflicted.png", 1);
		Engine::CreateNewTexture(L"ShieldBreaker_Virtue", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/ShieldBreaker/Virtue.png", 1);
	}

	// ��ų �̹���
	{
		Engine::CreateNewTexture(L"Pierce_Img", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/ShieldBreaker/Skill/Pierce_Img.png", 1);
		Engine::CreateNewTexture(L"Puncture_Img", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/ShieldBreaker/Skill/Puncture_Img.png", 1);
		Engine::CreateNewTexture(L"AddersKiss_Img", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/ShieldBreaker/Skill/AddersKiss_Img.png", 1);
		Engine::CreateNewTexture(L"Impale_Img", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/ShieldBreaker/Skill/Impale_Img.png", 1);
	}

	// ��ų ����Ʈ (���ο���)
	{
		Engine::CreateNewTexture(L"AddersKiss_Effect", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/ShieldBreaker/fx/adders_kiss/armatureName_adders_kiss_%d.png", 20);
		Engine::CreateNewTexture(L"Pierce_Effect", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/ShieldBreaker/fx/pierce/armatureName_pierce_%d.png", 15);
		Engine::CreateNewTexture(L"Puncture_Effect", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/ShieldBreaker/fx/break_guard/armatureName_break_guard_%d.png", 20);
		Engine::CreateNewTexture(L"Impale_Effect", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/ShieldBreaker/fx/spearing/armatureName_spearing_%d.png", 16);
		
	}

	// ��ų ����Ʈ (Ÿ��)
	{
		Engine::CreateNewTexture(L"AddersKiss_Target_Effect", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/ShieldBreaker/fx/adders_kiss_target/armatureName_adders_kiss_target_%d.png", 20);
		Engine::CreateNewTexture(L"Impale_Target_Effect", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/ShieldBreaker/fx/spearing_target/armatureName_spearing_target_%d.png", 20);
	}
	

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
		}
	}
}