#include "pch.h"
#include "BrigandMatchman.h"
#include"Export_Utility.h"

CBrigandMatchman::CBrigandMatchman(LPDIRECT3DDEVICE9 pGraphicDev)
	: CMonster(pGraphicDev)
{
}

CBrigandMatchman::CBrigandMatchman(const CCreature& rhs)
	: CMonster(rhs)
{
}

CBrigandMatchman::~CBrigandMatchman()
{
}

HRESULT CBrigandMatchman::ReadyGameObject()
{
	__super::ReadyGameObject();

	// 스탯 설정
	m_tCommonStat.iHp = 100;
	m_tCommonStat.iDodge = 10;
	m_tCommonStat.iSpeed = 4;
	m_tCommonStat.iAttackPower = 10;

	// 스킬 넣어주기
	vector<shared_ptr<CSkill>>	pVecSkill = {};
	int Skill1_Dot[2] = { 0,0 };
	_bool	m_bArrAttack1[6] = { 1, 0, 0, 0, 0, 0 };
	shared_ptr<CSkill> m_pBrigandMatchman_1 = make_shared<CSkill>
		(L"Attack1", L"Brigand Matchman_Attack1", Skill1_Dot, 0.f, 1.f, 0.f,
			m_bArrAttack1, 0, 0);
	pVecSkill.push_back(m_pBrigandMatchman_1);

	int Skill2_Dot[2] = { 0,0 };
	_bool	m_bArrAttack2[6] = { 1, 0, 0, 0, 0, 0 };
	shared_ptr<CSkill> m_pBrigandMatchman_2 = make_shared<CSkill>
		(L"Attack2", L"Brigand Matchman_Attack2", Skill2_Dot, 0.f, 1.1f, 0.f,
			m_bArrAttack2, 0, 0);
	pVecSkill.push_back(m_pBrigandMatchman_2);
	SetSkill(pVecSkill);

	m_pTransformCom->SetPosition(m_vPos.x, m_vPos.y, m_vPos.z);
	m_pTransformCom->SetScale(2.f, 2.f, 1.f);

	m_pTransformCom->SetAngle(m_vAngle);
	m_pTransformCom->Rotation(ROT_Y, PI / 2.f);

	return E_NOTIMPL;
}

_int CBrigandMatchman::UpdateGameObject(const _float& fTimeDelta)
{
	_int	iExit = __super::UpdateGameObject(fTimeDelta);

	Engine::AddRenderGroup(RENDER_ALPHA, shared_from_this());

	// Animation
	if (m_eCurAnimState != m_ePrevAnimState)
	{
		switch (m_eCurAnimState)
		{
		case EAnimState::COMBAT:
			m_pTextureCom->SetAnimKey(L"Brigand Matchman_Combat", 0.05f);
			m_pTransformCom->SetScale(2.f, 2.f, 1.f);
			break;
		case EAnimState::BESHOT:
			m_pTextureCom->SetAnimKey(L"Brigand Matchman_Hitted", 0.02f);
			m_pTransformCom->SetScale(2.f * 238.f / 179.f, 2.f * 371.f / 356.f, 1.f);
			break;
		case EAnimState::SKILL1:
			m_pTextureCom->SetAnimKey(L"Brigand Matchman_Attack1", 0.02f);
			m_pTransformCom->SetScale(2.f * 497.f / 179.f, 2.f * 257.f / 356.f, 1.f);
			break;
		case EAnimState::SKILL2:
			m_pTextureCom->SetAnimKey(L"Brigand Matchman_Attack2", 0.02f);
			m_pTransformCom->SetScale(2.f * 366.f / 179.f, 2.f * 442.f / 356.f, 1.f);
			break;
		case EAnimState::CORPSE:
			break;
		}
	}

	// 임시
	if ((IsAttacking()) && !m_bCorpse)
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
	else if (m_tCommonStat.iHp <= 0)
	{
		m_ePrevAnimState = m_eCurAnimState;
		m_eCurAnimState = EAnimState::CORPSE;
	}
	else if (m_bHitted == true && m_tCommonStat.iHp > 0)
	{
		m_ePrevAnimState = m_eCurAnimState;
		m_eCurAnimState = EAnimState::BESHOT;
	}
	else
	{
		m_ePrevAnimState = m_eCurAnimState;
		m_eCurAnimState = EAnimState::COMBAT;
	}

	// 시체 여부
	if (m_tCommonStat.iHp <= 0 && !m_bCorpse && !m_bDeath)
	{
		m_bCorpse = true;
		m_tCommonStat.iHp = 20;
	}

	// 사망 여부
	if (m_tCommonStat.iHp <= 0 && m_bCorpse)
	{
		m_bCorpse = false;
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
		m_fAttack1Time -= fTimeDelta;
		if (m_fAttack1Time < 0.f)
		{
			SetAttacking(false, 0);
			m_fAttack1Time = ATTACK1TIME;
		}
	}

	// 공격2 시간
	if (GetAttacking(1))
	{
		m_fAttack2Time -= fTimeDelta;
		if (m_fAttack2Time < 0.f)
		{
			SetAttacking(false, 1);
			m_fAttack2Time = ATTACK2TIME;
		}
	}


	return iExit;
}

void CBrigandMatchman::LateUpdateGameObject()
{
	__super::LateUpdateGameObject();
}

void CBrigandMatchman::RenderGameObject()
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

void CBrigandMatchman::AddComponent()
{
	Engine::CreateNewTexture(L"Brigand Matchman_Combat", TEX_NORMAL,
		L"../Bin/Resource/Image/Creatures/Monsters/Weald/Brigand Matchman/Combat/armatureName_combat_%d.png", 36);
	Engine::CreateNewTexture(L"Brigand Matchman_Attack1", TEX_NORMAL,
		L"../Bin/Resource/Image/Creatures/Monsters/Weald/Brigand Matchman/1.png", 1);
	Engine::CreateNewTexture(L"Brigand Matchman_Attack2", TEX_NORMAL,
		L"../Bin/Resource/Image/Creatures/Monsters/Weald/Brigand Matchman/2.png", 1);
	Engine::CreateNewTexture(L"Brigand Matchman_Hitted", TEX_NORMAL,
		L"../Bin/Resource/Image/Creatures/Monsters/Weald/Brigand Matchman/3.png", 1);

	shared_ptr<CComponent> pComponent;

	_vec3 vPosTemp = { 0.f,0.f,0.f };

	pComponent = m_pTransformCom = make_shared<CTransform>();
	NULL_CHECK_MSG(pComponent, L"Make Player TransformCom Failed");
	m_pTransformCom->ReadyTransform();
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform",pComponent });

	pComponent = m_pTextureCom = make_shared<CAnimator>(m_pGraphicDev);
	m_pTextureCom->SetAnimKey(L"Brigand Matchman_Combat", 0.05f);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Animator",pComponent });

	__super::AddComponent();
}

void CBrigandMatchman::Free()
{
	__super::Free();
}
