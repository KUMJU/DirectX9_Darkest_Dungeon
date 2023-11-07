#include "pch.h"
#include "Hero.h"
#include "Export_Utility.h"
#include "StatView.h"
#include "HeroStat.h"
#include "UIMgr.h"
#include "Player.h"

CHero::CHero(LPDIRECT3DDEVICE9 pGraphicDev)
	: CCreature(pGraphicDev)
{
	ZeroMemory(m_szString4, sizeof(m_szString4));
}

CHero::CHero(LPDIRECT3DDEVICE9 pGraphicDev, STAT _tCommonStat, _int _iPosition,
	vector<shared_ptr<CSkill>>& _pVecSkill, _float _fCritical)
	: CCreature(pGraphicDev, _tCommonStat, _iPosition, _pVecSkill), m_fCritical(_fCritical)
{
}

CHero::CHero(LPDIRECT3DDEVICE9 pGraphicDev, STAT _tCommonStat)
	: CCreature(pGraphicDev, _tCommonStat)
{
}

CHero::CHero(const CHero& rhs)
	: CCreature(rhs)
{
}

CHero::~CHero()
{

}

HRESULT CHero::ReadyGameObject()
{
	__super::ReadyGameObject();
	m_bIsHero = true;
	m_eCollideID = ECollideID::HERO;
	//m_pStatInfo->SetIsHero(true);

	return S_OK;
}

_int CHero::UpdateGameObject(const _float& fTimeDelta)
{
	_int	iExit = __super::UpdateGameObject(fTimeDelta);

	// 스탯갱신
	m_pStatInfo->SetStress(m_iStress);

	ChangeAnimState();
	SetAnimDelay(fTimeDelta);

	// 테스트용
	{
		if (GetAsyncKeyState(VK_HOME) & 0x8000)
		{
			m_eCurAnimState = EAnimState::COMBAT;
		}

		else if (GetAsyncKeyState(VK_PRIOR) & 0x8000)
		{
			m_eCurAnimState = EAnimState::WALK;
		}

		else if (GetAsyncKeyState(VK_NEXT) & 0x8000)
		{
			m_eCurAnimState = EAnimState::SKILL1;
		}

		else if (GetAsyncKeyState(VK_END) & 0x8000)
		{
			m_eCurAnimState = EAnimState::SKILL2;
		}
	}

	if (m_pStatUI) {
		m_pStatUI->UpdateGameObject(fTimeDelta);
	}

	return iExit;
}

void CHero::LateUpdateGameObject()
{
	__super::LateUpdateGameObject();
}

void CHero::RenderGameObject()
{
	__super::RenderGameObject();

	TCHAR szBuff[32] = { };
	_stprintf_s(szBuff, TEXT("%d"), GetStress());
	lstrcpy(m_szString4, szBuff);
	m_pColliderCom->RenderCollider();
}

void CHero::PickingObj()
{
	if (dynamic_pointer_cast<CPlayer>(CGameMgr::GetInstance()->GetPlayer())->GetInBattle())
	{
		m_bPicked = true;
	}
	else
	{
		shared_ptr<CPlayer> pPlayer = dynamic_pointer_cast<CPlayer>(CGameMgr::GetInstance()->GetPlayer());
		_vec3* pPlayerTransform = dynamic_pointer_cast<CTransform>(pPlayer->GetComponent(L"Com_Transform", ID_DYNAMIC))->GetPos();

		if ((*pPlayerTransform).y > -10)
		{
			m_pStatUI->SetForHire(true);
			m_pStatUI->SetVisible(true);
		}

		else if ((*pPlayerTransform).y < -50 && (*pPlayerTransform).y > -150)
		{
			m_eCurAnimState = EAnimState::COMBAT;
			pPlayer->ShowTavernUI(dynamic_pointer_cast<CHero>(shared_from_this()));
			m_bSelected = true;
		}

		CGameMgr::GetInstance()->SetGameState(EGameState::LOCK);
	}
}

shared_ptr<CSkill> CHero::SelectSkill(_int _iSkillID)
{
	shared_ptr<CSkill> m_pSelectedSkill = (m_pVecSkill)[_iSkillID];

	// 장착하지 않았거나 배운 적 없는 스킬이면 선택 불가
	if (!m_pSelectedSkill->IsEquipped() || !m_pSelectedSkill->IsUnlocked())
	{
		return nullptr;
	}

	// 현재 위치에서 사용할 수 없는 스킬이면 선택 불가
	if (!m_pSelectedSkill->IsActivatePos(m_tCommonStat.iOrder))
	{
		return nullptr;
	}

	// 한 전투 당 사용 횟수를 다 쓴 스킬이면 선택 불가
	if (!m_pSelectedSkill->CanActivateCnt())
	{
		return nullptr;
	}

	return m_pSelectedSkill;
}

void CHero::StressUp()
{
	srand(unsigned(time(NULL)));

	if (m_iStress >= 100)
	{
		// 랜덤 숫자 0~1 생성
		_int iRand = rand() % 2;

		// 랜덤 숫자 0~3 생성
		_int iAorV = rand() % 4;

		// 3 나오면 기상
		if (3 == iAorV)
		{
			m_bVirtue = true;
			m_eVirtue = (EVirtue)iRand;
		}
		// 아니면 붕괴
		else
		{
			m_bAffliction = true;
			m_eAffliction = (EAffliction)iRand;
		}
	}

	else if (m_iStress >= 200)
		m_bDeathDoor = true;
}

void CHero::AddComponent()
{
	if (m_bReady)
		return;

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
	m_pTextureCom->SetAnimKey(m_strAnimKey, 0.05f);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Animator", pComponent });

	pComponent = m_pColliderCom = make_shared<CCollider>(m_pGraphicDev);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Collider",pComponent });
	m_pColliderCom->SetScale({3.f, 3.f, 1.f});
	m_pColliderCom->SetPos(m_pTransformCom->GetPos());
}

void CHero::SwapSkill(_int _iIdx1, _int _iIdx2)
{
	swap(m_pVecSkill[_iIdx1], m_pVecSkill[_iIdx2]);
}

void CHero::ChangeAnimState()
{

	//// 임시
	//if ((m_bAttacking1 || m_bAttacking2 || m_bAttacking3 || m_bAttacking4) && m_tCommonStat.iHp >= 0)
	//{
	//	if (m_bAttacking1)
	//		m_eCurAnimState = EAnimState::SKILL1;
	//	else if (m_bAttacking2)
	//		m_eCurAnimState = EAnimState::SKILL2;
	//	else if (m_bAttacking3)
	//		m_eCurAnimState = EAnimState::SKILL3;
	//	else if (m_bAttacking4)
	//		m_eCurAnimState = EAnimState::SKILL4;
	//}
	//else if (m_bHitted == true && m_tCommonStat.iHp > 0)
	//{
	//	m_ePrevAnimState = m_eCurAnimState;
	//	m_eCurAnimState = EAnimState::BESHOT;
	//}
	//else
	//{
	//	m_ePrevAnimState = m_eCurAnimState;
	//	m_eCurAnimState = EAnimState::COMBAT;
	//}	
}

void CHero::SetAnimDelay(const _float& fTimeDelta)
{
	//// 피격 시간
	//if (m_bHitted)
	//{
	//	m_fHittedTime -= fTimeDelta;
	//	if (m_fHittedTime < 0.f)
	//	{
	//		m_bHitted = false;
	//		m_fHittedTime = HITTEDTIME;
	//	}
	//}
	//
	//// 공격 시간
	//if (m_bAttacking1 || m_bAttacking2 || m_bAttacking3 || m_bAttacking4 )
	//{
	//	m_fAttackTime -= fTimeDelta;
	//	if (m_fAttackTime < 0.f)
	//	{
	//		m_bAttacking1 = false;
	//		m_bAttacking2 = false;
	//		m_bAttacking3 = false;
	//		m_bAttacking4 = false;
	//
	//		m_fAttackTime = ATTACKTIME;
	//	}
	//}

	// 붕괴 or 기상 변경 시간 (UI로 띄우기)
	if ((m_bVirtue || m_bAffliction) && !m_bStressChanged)
	{
		m_fChangeTime -= fTimeDelta;
		if (m_fChangeTime < 0.f)
		{
			m_bStressChanged = true;
		}
	}

	// 기타 딜레이 타임
	if (m_bDelay)
	{
		m_fDelayTime -= fTimeDelta;
		if (m_fDelayTime < 0.f)
		{
			m_bDelay = false;
		}
	}
}

void CHero::StressEvent()
{
	if (m_bAffliction)
	{
		switch (m_eAffliction)
		{
		// 스트레스
		case EAffliction::SELFISH:
			m_iStress += 5;
			m_bDelay = true;
			// 이펙트 넣어주기
			break;
		// 자해
		case EAffliction::IRRATIONAL:
			m_tCommonStat.iHp -= 3;
			m_bHitted = true;
			// 이펙트 넣어주기
			break;
		case EAffliction::ENUM_END:
			break;
		}
	}
	else if (m_bVirtue)
	{
		switch (m_eVirtue)
		{
		case EVirtue::COURAGEOUS:
			m_tCommonStat.iHp += 3;
			m_bHitted = true;
			// 이펙트 넣어주기
			break;
		case EVirtue::VIGOROUS:
			m_iStress -= 4;
			m_bHitted = true;
			// 이펙트 넣어주기
			break;
		case EVirtue::ENUM_END:
			break;
		}
	}
}

void CHero::Free()
{
	__super::Free();
}
