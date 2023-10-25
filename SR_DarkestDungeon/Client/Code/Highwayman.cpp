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

	// 스킬 세팅
	{
		_int  DotDamZero[2] = { 0, 0 };

		// 사약한 칼질
		_bool arrActivatePos1[4] = { 1, 1, 1, 0 };
		_bool arrTargetPos1[4] = { 1, 1, 0, 0 };
		_bool arrAttack1[6] = { 1, 0, 0, 0, 0, 0 };
		_bool arrToEnemy1[6] = { 1, 1, 1, 1, 1, 1 };

		/*
		영웅 스킬 용
		(스킬 이름, 스킬 애니메이션, 스킬 이미지, 이펙트키, 활성 위치, 타겟 위치, 공격종류(단순 공격, 중독, 출혈, 기절, 이동, 힐), 
		각 공격 타겟이 적인지 아닌지, 도트뎀[데미지][라운드], 스턴 확률, 공격력 계수, 치명타 확률, 라운드 당 사용 가능 횟수, 이동량, 힐량, 타겟 대상을 모두 공격하는지)
		*/

		shared_ptr<CSkill> pSkill1 = make_shared<CSkill>
			(L"WickedSlice", L"Highwayman_WickedSlice", L"WickedSlice_Img", L"WickedSlice_Effect",
				arrActivatePos1, arrTargetPos1, arrAttack1, arrToEnemy1, DotDamZero, 1.f, 1.15f, 1.05f, -1, 0);

		// 권총 사격
		_bool arrActivatePos2[4] = { 0, 1, 1, 1 };
		_bool arrTargetPos2[4] = { 0, 1, 1, 1 };
		_bool arrAttack2[6] = { 1, 0, 0, 0, 0, 0 };
		_bool arrToEnemy2[6] = { 1, 1, 1, 1, 1, 1 };

		shared_ptr<CSkill> pSkill2 = make_shared<CSkill>
			(L"PistolShot", L"Highwayman_PistolShot", L"PistolShot_Img", L"PistolShot_Effect",
				arrActivatePos2, arrTargetPos2, arrAttack2, arrToEnemy2, DotDamZero, 1.f, 0.75f, 1.075f, -1);

		// 영거리 사격
		_bool arrActivatePos3[4] = { 1, 0, 0, 0 };
		_bool arrTargetPos3[4] = { 1, 0, 0, 0 };
		_bool arrAttack3[6] = { 1, 0, 0, 0, 1, 0 };
		_bool arrToEnemy3[6] = { 1, 1, 1, 1, 1, 1 };

		shared_ptr<CSkill> pSkill3 = make_shared<CSkill>
			(L"PointBlankShot", L"Highwayman_PointBlankShot", L"PointBlankShot_Img", L"PointBlankShot_Effect",
				arrActivatePos3, arrTargetPos3, arrAttack3, arrToEnemy3, DotDamZero, 1.f, 1.5f, 1.05f, -1, 1);

		// 결투가의 진격
		_bool arrActivatePos4[4] = { 0, 1, 1, 1 };
		_bool arrTargetPos4[4] = { 1, 1, 1, 0 };
		_bool arrAttack4[6] = { 1, 0, 0, 0, 1, 0 };
		_bool arrToEnemy4[6] = { 1, 1, 1, 1, 0, 1 };

		shared_ptr<CSkill> pSkill4 = make_shared<CSkill>
			(L"DuelistsAdvance", L"Highwayman_DuelistsAdvance", L"DuelistsAdvance_Img", L"DuelistsAdvance_Effect",
				arrActivatePos4, arrTargetPos4, arrAttack4, arrToEnemy4, DotDamZero, 1.f, 0.8f, 1.05f, -1, -1);

		m_pVecSkill.push_back(pSkill1);
		m_pVecSkill.push_back(pSkill2);
		m_pVecSkill.push_back(pSkill3);
		m_pVecSkill.push_back(pSkill4);
	}

	// 영웅 스탯
	{
		m_tCommonStat.iHp = 23;
		m_tCommonStat.iDodge = 10;
		m_tCommonStat.iSpeed = 5;
		m_tCommonStat.iAttackPower = 8;
		m_tCommonStat.iOrder = 0;
	}

	// 테스트용
	{
		m_vPos = { 30.f, 3.f, 20.f };
		m_eAnimState = EAnimState::IDLE;
		m_vAngle = { 0.f, PI / 2, 0.f };

		m_pTransformCom->SetPosition(m_vPos.x, m_vPos.y, m_vPos.z);
		m_pTransformCom->SetScale(2.f, 3.f, 1.f);

		m_pTransformCom->SetAngle(m_vAngle);
		m_pTransformCom->Rotation(ROT_Y, PI / 2.f);

		m_pTextureCom->SetAnimKey(L"Highwayman_Idle", 0.02f);
	}

	return S_OK;
}

_int CHighwayman::UpdateGameObject(const _float& fTimeDelta)
{
	Engine::AddRenderGroup(RENDER_ALPHA, shared_from_this());

	_int iExit = __super::UpdateGameObject(fTimeDelta);

	ChangeAnim();

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
	// 영웅 애니메이션
	{
		Engine::CreateNewTexture(L"Highwayman_Combat", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/Highwayman/Combat/armatureName_combat_%d.png", 29);
		Engine::CreateNewTexture(L"Highwayman_Idle", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/Highwayman/Idle/armatureName_idle_%d.png", 29);
		Engine::CreateNewTexture(L"Highwayman_Walk", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/Highwayman/Walk/armatureName_walk_%d.png", 33);

		Engine::CreateNewTexture(L"Highwayman_WickedSlice", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/Highwayman/WickedSlice.png", 1);
		Engine::CreateNewTexture(L"Highwayman_PistolShot", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/Highwayman/PistolShot.png", 1);
		Engine::CreateNewTexture(L"Highwayman_PointBlankShot", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/Highwayman/PointBlankShot.png", 1);
		Engine::CreateNewTexture(L"Highwayman_DuelistsAdvance", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/Highwayman/DuelistsAdvance.png", 1);

		Engine::CreateNewTexture(L"Highwayman_Defend", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/Highwayman/Defend.png", 1);
		Engine::CreateNewTexture(L"Highwayman_Affliction", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/Highwayman/Afflicted.png", 1);
		Engine::CreateNewTexture(L"Highwayman_Virtue", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/Highwayman/Virtue.png", 1);
	}

	// 스킬 이미지
	{
		Engine::CreateNewTexture(L"WickedSlice_Img", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/Highwayman/Skill/WickedSlice_Img.png", 1);
		Engine::CreateNewTexture(L"PistolShot_Img", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/Highwayman/Skill/PistolShot_Img.png", 1);
		Engine::CreateNewTexture(L"PointBlankShot_Img", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/Highwayman/Skill/PointBlankShot_Img.png", 1);
		Engine::CreateNewTexture(L"DuelistsAdvance_Img", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/Highwayman/Skill/DuelistsAdvance_Img.png", 1);
	}

	/*
	// 스킬 이펙트 (본인에게)
	{
		Engine::CreateNewTexture(L"PointBlankShot_Effect", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/Highwayman/fx/point_blank_shot/armatureName_point_blank_shot_%d.png", 57);
	}

	// 스킬 이펙트 (타겟)
	{
		Engine::CreateNewTexture(L"PistolShot_Target_Effect", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/Highwayman/fx/pistol_shot_target/armatureName_pistol_shot_target_%d.png", 57);
		Engine::CreateNewTexture(L"PointBlankShot_Target_Effect", TEX_NORMAL,
			L"../Bin/Resource/Image/Creatures/Heroes/Highwayman/fx/point_blank_shot_target/armatureName_point_blank_shot_target_%d.png", 57);
	}
	*/

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
			m_pTextureCom->SetAnimKey(L"Highwayman_Idle", 0.02f);
			m_pTransformCom->SetScale(2.f, 3.f, 1.f);
			break;
		case EAnimState::WALK:
			m_pTextureCom->SetAnimKey(L"Highwayman_Walk", 0.02f);
			m_pTransformCom->SetScale(2.f, 3.f, 1.f);
			break;
		case EAnimState::COMBAT:
			m_pTextureCom->SetAnimKey(L"Highwayman_Combat", 0.02f);
			m_pTransformCom->SetScale(2.f, 3.f, 1.f);
			break;
		case EAnimState::BESHOT:
			m_pTextureCom->SetAnimKey(L"Highwayman_Defend", 0.02f);
			m_pTransformCom->SetScale(2.f, 3.f, 1.f);
			break;
		case EAnimState::SKILL1:
			m_pTextureCom->SetAnimKey(L"Highwayman_WickedSlice", 0.02f);
			m_pTransformCom->SetScale(2.f, 3.f, 1.f);
			break;
		case EAnimState::SKILL2:
			m_pTextureCom->SetAnimKey(L"Highwayman_PistolShot", 0.02f);
			m_pTransformCom->SetScale(2.f, 3.f, 1.f);
			break;
		case EAnimState::SKILL3:
			m_pTextureCom->SetAnimKey(L"Highwayman_PointBlankShot", 0.02f);
			m_pTransformCom->SetScale(2.f, 3.f, 1.f);
			break;
		case EAnimState::SKILL4:
			m_pTextureCom->SetAnimKey(L"Highwayman_DuelistsAdvance", 0.02f);
			m_pTransformCom->SetScale(2.f, 3.f, 1.f);
			break;
		case EAnimState::AFFLICTION:
			m_pTextureCom->SetAnimKey(L"Highwayman_Affliction", 0.02f);
			m_pTransformCom->SetScale(2.f, 3.f, 1.f);
			break;
		case EAnimState::VIRTUE:
			m_pTextureCom->SetAnimKey(L"Highwayman_Virtue", 0.02f);
			m_pTransformCom->SetScale(2.f, 3.f, 1.f);
			break;
		}
	}
}