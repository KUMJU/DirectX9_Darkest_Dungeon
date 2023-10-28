#include "pch.h"
#include "Highwayman.h"
#include"Export_Utility.h"

CHighwayman::CHighwayman(LPDIRECT3DDEVICE9 pGraphicDev) : CHero(pGraphicDev)
{
}

CHighwayman::CHighwayman(const CHighwayman& rhs) : CHero(rhs)
{
}

CHighwayman::~CHighwayman()
{
}

HRESULT CHighwayman::ReadyGameObject()
{
	__super::ReadyGameObject();

	m_bIsHero = true;

	// ��ų ����
	{
		_int  DotDamZero[2] = { 0, 0 };

		// ����� Į��
		_bool arrActivatePos1[4] = { 1, 1, 1, 0 };
		_bool arrTargetPos1[4] = { 1, 1, 0, 0 };
		_bool arrAttack1[6] = { 1, 0, 0, 0, 0, 0 };
		_bool arrToEnemy1[6] = { 1, 1, 1, 1, 1, 1 };

		/*
		���� ��ų ��
		(��ų �̸�, ��ų �ִϸ��̼�, ��ų �̹���, ����ƮŰ, Ȱ�� ��ġ, Ÿ�� ��ġ, ��������(�ܼ� ����, �ߵ�, ����, ����, �̵�, ��), 
		�� ���� Ÿ���� ������ �ƴ���, ��Ʈ��[������][����], ���� Ȯ��, ���ݷ� ���, ġ��Ÿ Ȯ��, ���� �� ��� ���� Ƚ��, �̵���, ����, Ÿ�� ����� ��� �����ϴ���)
		*/

		shared_ptr<CSkill> pSkill1 = make_shared<CSkill>
			(L"WickedSlice", L"Highwayman_WickedSlice", L"WickedSlice_Img", L"WickedSlice_Effect",
				arrActivatePos1, arrTargetPos1, arrAttack1, arrToEnemy1, DotDamZero, 1.f, 1.15f, 1.05f, -1, 0, 0, 0, 1, 0);

		// ���� ���
		_bool arrActivatePos2[4] = { 0, 1, 1, 1 };
		_bool arrTargetPos2[4] = { 0, 1, 1, 1 };
		_bool arrAttack2[6] = { 1, 0, 0, 0, 0, 0 };
		_bool arrToEnemy2[6] = { 1, 1, 1, 1, 1, 1 };

		shared_ptr<CSkill> pSkill2 = make_shared<CSkill>
			(L"PistolShot", L"Highwayman_PistolShot", L"PistolShot_Img", L"PistolShot_Effect",
				arrActivatePos2, arrTargetPos2, arrAttack2, arrToEnemy2, DotDamZero, 1.f, 0.75f, 1.075f, -1, 0, 0, 0, 0, 0);

		// ���Ÿ� ���
		_bool arrActivatePos3[4] = { 1, 0, 0, 0 };
		_bool arrTargetPos3[4] = { 1, 0, 0, 0 };
		_bool arrAttack3[6] = { 1, 0, 0, 0, 0, 0 };
		_bool arrToEnemy3[6] = { 1, 1, 1, 1, 1, 1 };

		shared_ptr<CSkill> pSkill3 = make_shared<CSkill>
			(L"PointBlankShot", L"Highwayman_PointBlankShot", L"PointBlankShot_Img", L"PointBlankShot_Effect",
				arrActivatePos3, arrTargetPos3, arrAttack3, arrToEnemy3, DotDamZero, 1.f, 1.5f, 1.05f, -1, 1, 0, 0, 1, 0);

		// �������� ����
		_bool arrActivatePos4[4] = { 0, 1, 1, 1 };
		_bool arrTargetPos4[4] = { 1, 1, 1, 0 };
		_bool arrAttack4[6] = { 1, 0, 0, 0, 0, 0 };
		_bool arrToEnemy4[6] = { 1, 1, 1, 1, 0, 1 };

		shared_ptr<CSkill> pSkill4 = make_shared<CSkill>
			(L"DuelistsAdvance", L"Highwayman_DuelistsAdvance", L"DuelistsAdvance_Img", L"DuelistsAdvance_Effect",
				arrActivatePos4, arrTargetPos4, arrAttack4, arrToEnemy4, DotDamZero, 1.f, 0.8f, 1.05f, -1, 0, 0, 0, 1, 0);

		m_pVecSkill.push_back(pSkill1);
		m_pVecSkill.push_back(pSkill2);
		m_pVecSkill.push_back(pSkill3);
		m_pVecSkill.push_back(pSkill4);
	}

	// ���� ����
	{
		m_tCommonStat.iHp = 30;
		m_tCommonStat.iDodge = 30;
		m_tCommonStat.iSpeed = 5;
		m_tCommonStat.iAttackPower = 8;
		m_tCommonStat.iOrder = 0;
	}

	// �׽�Ʈ��
	{
		m_eAnimState = EAnimState::IDLE;
		//m_vAngle = { 0.f, PI / 2, 0.f };

		m_pTransformCom->SetPosition(m_vPos.x, m_vPos.y, m_vPos.z);
		m_pTransformCom->SetScale(2.f, 3.f, 1.f);

		m_pTransformCom->SetAngle(m_vAngle);
		m_pTransformCom->Rotation(ROT_Y, PI / 2.f);

		m_pTextureCom->SetAnimKey(L"Highwayman_Idle", 0.04f);
	}

	return S_OK;
}

_int CHighwayman::UpdateGameObject(const _float& fTimeDelta)
{
	Engine::AddRenderGroup(RENDER_ALPHA, shared_from_this());

	_int iExit = __super::UpdateGameObject(fTimeDelta);

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

	// ��� ����
	if (m_tCommonStat.iHp <= 0)
	{
		m_bDeath = true;
		m_tCommonStat.iHp = -100;
	}

	// �ǰ� �ð�
	if (m_bHitted)
	{
		m_fHittedTime -= fTimeDelta;
		if (m_fHittedTime < 0.f)
		{
			m_bHitted = false;
			m_fHittedTime = HITTEDTIME;
		}
	}

	// ����1 �ð�
	if (GetAttacking(0))
	{
		m_fAttackTime -= fTimeDelta;
		if (m_fAttackTime < 0.f)
		{
			SetAttacking(false, 0);
			m_fAttackTime = ATTACKTIME;
		}
	}

	// ����2 �ð�
	if (GetAttacking(1))
	{
		m_fAttackTime -= fTimeDelta;
		if (m_fAttackTime < 0.f)
		{
			SetAttacking(false, 1);
			m_fAttackTime = ATTACKTIME;
		}
	}

	// ����3 �ð�
	if (GetAttacking(2))
	{
		m_fAttackTime -= fTimeDelta;
		if (m_fAttackTime < 0.f)
		{
			SetAttacking(false, 2);
			m_fAttackTime = ATTACKTIME;
		}
	}

	// ����4 �ð�
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

void CHighwayman::LateUpdateGameObject()
{
	__super::LateUpdateGameObject();
}

void CHighwayman::RenderGameObject()
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

void CHighwayman::AddComponent()
{
	__super::AddComponent();
}

void CHighwayman::ChangeAnim()
{
	// Animation
	if (m_eCurAnimState != m_ePrevAnimState)
	{
		switch (m_eCurAnimState)
		{
		case EAnimState::IDLE:
			m_pTextureCom->SetAnimKey(L"Highwayman_Idle", 0.04f);
			m_pTransformCom->SetScale(2.f, 3.f, 1.f);
			break;
		case EAnimState::WALK:
			m_pTextureCom->SetAnimKey(L"Highwayman_Walk", 0.04f);
			m_pTransformCom->SetScale(2.f, 3.f, 1.f);
			break;
		case EAnimState::COMBAT:
			m_pTextureCom->SetAnimKey(L"Highwayman_Combat", 0.04f);
			m_pTransformCom->SetScale(2.f, 3.f, 1.f);
			break;
		case EAnimState::BESHOT:
			m_pTextureCom->SetAnimKey(L"Highwayman_Defend", 0.04f);
			m_pTransformCom->SetScale(2.f, 3.f, 1.f);
			break;
		case EAnimState::SKILL1:
			m_pTextureCom->SetAnimKey(L"Highwayman_WickedSlice", 0.04f);
			m_pTransformCom->SetScale(2.f, 3.f, 1.f);
			break;
		case EAnimState::SKILL2:
			m_pTextureCom->SetAnimKey(L"Highwayman_PistolShot", 0.04f);
			m_pTransformCom->SetScale(2.f, 3.f, 1.f);
			break;
		case EAnimState::SKILL3:
			m_pTextureCom->SetAnimKey(L"Highwayman_PointBlankShot", 0.04f);
			m_pTransformCom->SetScale(2.f, 3.f, 1.f);
			break;
		case EAnimState::SKILL4:
			m_pTextureCom->SetAnimKey(L"Highwayman_DuelistsAdvance", 0.04f);
			m_pTransformCom->SetScale(2.f, 3.f, 1.f);
			break;
		case EAnimState::AFFLICTION:
			m_pTextureCom->SetAnimKey(L"Highwayman_Affliction", 0.04f);
			m_pTransformCom->SetScale(2.f, 3.f, 1.f);
			break;
		case EAnimState::VIRTUE:
			m_pTextureCom->SetAnimKey(L"Highwayman_Virtue", 0.04f);
			m_pTransformCom->SetScale(2.f, 3.f, 1.f);
			break;
		case EAnimState::DEATH:
			m_pTextureCom->SetAnimKey(L"Hero_Death", 0.02f);
			m_pTransformCom->SetScale(3.f, 3.f, 1.f);
			break;
		}
	}
}