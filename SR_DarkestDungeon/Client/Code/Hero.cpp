#include "pch.h"
#include "Hero.h"
#include"Export_Utility.h"

CHero::CHero(LPDIRECT3DDEVICE9 pGraphicDev)
	: CCreature(pGraphicDev)
{
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

	return S_OK;
}

_int CHero::UpdateGameObject(const _float& fTimeDelta)
{
	_int	iExit = __super::UpdateGameObject(fTimeDelta);

	return iExit;
}

void CHero::LateUpdateGameObject()
{
	__super::LateUpdateGameObject();
}

void CHero::RenderGameObject()
{
	__super::RenderGameObject();
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

void CHero::StressAction()
{
	srand(unsigned(time(NULL)));

	if (m_iStress >= 100)
	{
		// 랜덤 숫자 0~1 생성
		int iRand = rand() % 2;
		// 랜덤 숫자 0~3 생성
		int iAorV = rand() % 4;
		// 3 나오면 기상
		if (3 == iAorV)
		{
			m_bVirtue = true;
			m_eVirtue = (EVirtue)iRand;
			// 애니메이션 변경

		}
		// 아니면 붕괴
		else
		{
			m_bAffliction = true;
			m_eAffliction = (EAffliction)iRand;
			// 애니메이션 변경
		}
	}

	else if (m_iStress >= 200)
		m_bDeathDoor = true;
}

void CHero::AddComponent()
{
	__super::AddComponent();
	//shared_ptr<CComponent> pComponent;

	//pComponent = m_pBufCom = make_shared <CRcTex>(m_pGraphicDev);
	//m_pBufCom->ReadyBuffer();
	//m_mapComponent[ID_STATIC].insert({ L"Com_RCTex",pComponent });

	//pComponent = m_pEffectBufCom = make_shared<CRcTex>(m_pGraphicDev);
	//m_pEffectBufCom->ReadyBuffer();
	//m_mapComponent[ID_STATIC].insert({ L"Com_EffectRCTex",pComponent });

	//pComponent = m_pTextureCom = make_shared<CAnimator>(m_pGraphicDev);
	//m_pTextureCom->SetAnimKey(m_strAnimKey, 0.05f);
	//m_mapComponent[ID_DYNAMIC].insert({ L"Com_Animator", pComponent });
}

void CHero::ChangeAnim()
{
}

void CHero::Free()
{
	__super::Free();
}
