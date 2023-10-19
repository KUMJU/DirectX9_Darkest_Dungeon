#include "Skill.h"


CSkill::CSkill(tstring _strSkillName, _bool* _arrActivatePos, _bool* _arrTargetPos, _bool _bToEnemy,
				_int _iActivateCnt, _int _iAbilityCnt, ESkillAbility* _eListAbility,
				_int _iMoveCnt, _int* _iDotDamage, _float _fStunRate, _int _iHeal,
				tstring _strAnimKey, tstring _strImgKey)
	: m_strSkillName(_strSkillName), m_bToEnemy(_bToEnemy), m_iActivateCnt(_iActivateCnt), m_iAbilityCnt(_iAbilityCnt),
	m_iMoveCnt(_iMoveCnt), m_fStunRate(_fStunRate), m_iHeal(_iHeal), m_strAnimKey(_strAnimKey), m_strImgKey(_strImgKey)
{
	memcpy(m_arrActivatePos, _arrActivatePos, sizeof(_int) * 4);
	memcpy(m_arrTargetPos, _arrTargetPos, sizeof(_int) * 4);
	memcpy(m_eListAbility, _eListAbility, sizeof(ESkillAbility) * 3);
	memcpy(m_iDotDamage, _iDotDamage, sizeof(_int) * 2);
}

CSkill::CSkill(const CSkill& rhs)
{
}

CSkill::~CSkill()
{
}

_bool CSkill::UseSkill()
{
	if (m_iActivateCnt == 0)
		return false;

	return true;
}
