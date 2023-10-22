#include "pch.h"
#include "BrigandCutthroat.h"
#include"Export_Utility.h"

CBrigandCutthroat::CBrigandCutthroat(LPDIRECT3DDEVICE9 pGraphicDev)
	: CMonster(pGraphicDev)
{
}

CBrigandCutthroat::CBrigandCutthroat(const CCreature& rhs)
	: CMonster(rhs)
{
}

CBrigandCutthroat::~CBrigandCutthroat()
{
}

HRESULT CBrigandCutthroat::ReadyGameObject()
{
	__super::ReadyGameObject();

	// 스탯 설정
	m_tCommonStat.iHp = 100;
	m_tCommonStat.iDodge = 10;
	m_tCommonStat.iSpeed = 3;
	m_tCommonStat.iAttackPower = 10;

	// 스킬 넣어주기
	vector<shared_ptr<CSkill>>	pVecSkill = {};
	int Skill1_Dot[2] = {1,3};
	_bool	m_bArrAttack1[6] = { 0, 0, 1, 0, 0, 0};
	shared_ptr<CSkill> m_pBrigandCutthroat_1 = make_shared<CSkill>
		(L"Attack1", L"Brigand Cutthroat_Attack1", Skill1_Dot, 0.f, 1.2f, 0.f,
			m_bArrAttack1, 0, 0);
	pVecSkill.push_back(m_pBrigandCutthroat_1);

	int Skill2_Dot[2] = { 2,3 };
	_bool	m_bArrAttack2[6] = { 0, 0, 1, 0, 0, 0 };
	shared_ptr<CSkill> m_pBrigandCutthroat_2 = make_shared<CSkill>
		(L"Attack2", L"Brigand Cutthroat_Attack2", Skill2_Dot, 0.f, 1.5f, 0.f,
			m_bArrAttack2, 0, 0);
	pVecSkill.push_back(m_pBrigandCutthroat_2);
	SetSkill(pVecSkill);

	m_pTransformCom->SetPosition(m_vPos.x, m_vPos.y, m_vPos.z);
	m_pTransformCom->SetScale(2.f, 2.f, 1.f);

	m_pTransformCom->SetAngle(m_vAngle);
	m_pTransformCom->Rotation(ROT_Y, PI / 2.f);

	return E_NOTIMPL;
}

_int CBrigandCutthroat::UpdateGameObject(const _float& fTimeDelta)
{
	_int	iExit = __super::UpdateGameObject(fTimeDelta);

	Engine::AddRenderGroup(RENDER_ALPHA, shared_from_this());

	// Animation
	if (m_eCurAnimState != m_ePrevAnimState)
	{
		switch (m_eCurAnimState)
		{
		case EAnimState::COMBAT:
			m_pTextureCom->SetAnimKey(L"Brigand Cutthroat_Combat", 0.02f);
			m_pTransformCom->SetScale(2.f, 2.f, 1.f);
			break;
		case EAnimState::BESHOT:
			m_pTextureCom->SetAnimKey(L"Brigand Cutthroat_Hitted", 0.02f);
			m_pTransformCom->SetScale(2.f * 225.f / 232.f, 2.f * 317.f / 311.f, 1.f);
			break;
		case EAnimState::SKILL1:
			m_pTextureCom->SetAnimKey(L"Brigand Cutthroat_Attack1", 0.02f);
			m_pTransformCom->SetScale(2.f * 427.f / 232.f, 2.f * 268.f / 311.f, 1.f);
			break;
		case EAnimState::SKILL2:
			m_pTextureCom->SetAnimKey(L"Brigand Cutthroat_Attack2", 0.02f);
			m_pTransformCom->SetScale(2.f * 232.f / 232.f, 2.f * 365.f / 311.f, 1.f);
			break;
		case EAnimState::CORPSE:
			m_pTextureCom->SetAnimKey(L"Brigand Cutthroat_Dead", 0.02f);
			m_pTransformCom->SetScale(2.f * 215.f / 232.f, 2.f * 158.f / 311.f, 1.f);
			break;
		}
	}

	// 임시
	if ((m_bAttacking1 || m_bAttacking2) && m_tCommonStat.iHp > 0)
	{
		if(m_bAttacking1)
			m_eCurAnimState = EAnimState::SKILL1;
		else if (m_bAttacking2)
			m_eCurAnimState = EAnimState::SKILL2;
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
	if (m_bAttacking1)
	{
		m_fAttack1Time -= fTimeDelta;
		if (m_fAttack1Time < 0.f)
		{
			m_bAttacking1 = false;
			m_fAttack1Time = ATTACK1TIME;
		}
	}

	// 공격2 시간
	if (m_bAttacking2)
	{
		m_fAttack2Time -= fTimeDelta;
		if (m_fAttack2Time < 0.f)
		{
			m_bAttacking2 = false;
			m_fAttack2Time = ATTACK2TIME;
		}
	}


	return iExit;
}

void CBrigandCutthroat::LateUpdateGameObject()
{
	__super::LateUpdateGameObject();
}

void CBrigandCutthroat::RenderGameObject()
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

void CBrigandCutthroat::AddComponent()
{
	Engine::CreateNewTexture(L"Brigand Cutthroat_Combat", TEX_NORMAL,
		L"../Bin/Resource/Image/Creatures/Monsters/Weald/Brigand Cutthroat/Combat/armatureName_combat_%d.png", 33);
	Engine::CreateNewTexture(L"Brigand Cutthroat_Attack1", TEX_NORMAL,
		L"../Bin/Resource/Image/Creatures/Monsters/Weald/Brigand Cutthroat/1.png", 1);
	Engine::CreateNewTexture(L"Brigand Cutthroat_Attack2", TEX_NORMAL,
		L"../Bin/Resource/Image/Creatures/Monsters/Weald/Brigand Cutthroat/2.png", 1);
	Engine::CreateNewTexture(L"Brigand Cutthroat_Dead", TEX_NORMAL,
		L"../Bin/Resource/Image/Creatures/Monsters/Weald/Brigand Cutthroat/3.png", 1);
	Engine::CreateNewTexture(L"Brigand Cutthroat_Hitted", TEX_NORMAL,
		L"../Bin/Resource/Image/Creatures/Monsters/Weald/Brigand Cutthroat/4.png", 1);

	shared_ptr<CComponent> pComponent;

	_vec3 vPosTemp = { 0.f,0.f,0.f };

	pComponent = m_pTransformCom = make_shared<CTransform>();
	NULL_CHECK_MSG(pComponent, L"Make Player TransformCom Failed");
	m_pTransformCom->ReadyTransform();
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform",pComponent });

	pComponent = m_pTextureCom = make_shared<CAnimator>(m_pGraphicDev);
	m_pTextureCom->SetAnimKey(L"Brigand Cutthroat_Combat", 0.05f);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Animator",pComponent });

	//pComponent = m_pEffectTransformCom = make_shared<CTransform>();
	//NULL_CHECK_MSG(pComponent, L"Make Player TransformCom Failed");
	//m_pEffectTransformCom->ReadyTransform();
	//m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform",pComponent });
	//
	//pComponent = m_pEffectTextureCom = make_shared<CAnimator>(m_pGraphicDev);
	//m_pEffectTextureCom->SetAnimKey(L"Brigand Cutthroat_Combat", 0.05f);
	//m_mapComponent[ID_DYNAMIC].insert({ L"Com_Animator",pComponent });

	__super::AddComponent();
}

void CBrigandCutthroat::Free()
{
	__super::Free();
}
