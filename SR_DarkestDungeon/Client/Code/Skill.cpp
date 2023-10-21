#include "pch.h"
#include "Skill.h"

CSkill::CSkill(tstring _strSkillName, tstring _strAnimKey, tstring _strImgKey,
	_bool* _arrActivatePos, _bool* _arrTargetPos, _int* _iDotDamage,
	_bool* _bArrAttack, _float _fStunRatio, _float _fDamageRatio, _float _fCriticalRatio,
	_bool _bToEnemy, _int _iActivateCnt, _int _iMoveCnt, _int _iHeal)
	: m_strSkillName(_strSkillName), m_bToEnemy(_bToEnemy), m_iActivateCnt(_iActivateCnt),
	m_iMoveCnt(_iMoveCnt), m_iHeal(_iHeal), m_strAnimKey(_strAnimKey), m_strImgKey(_strImgKey),
	m_fStunRatio(_fStunRatio), m_fDamageRatio(_fDamageRatio), m_fCriticalRatio(_fCriticalRatio)
{
	memcpy(m_arrActivatePos, _arrActivatePos, sizeof(_bool) * 4);
	memcpy(m_arrTargetPos, _arrTargetPos, sizeof(_bool) * 4);
	memcpy(m_bArrAttack, _bArrAttack, sizeof(_bool) * 6);
	memcpy(m_iDotDamage, _iDotDamage, sizeof(_int) * 2);
}

CSkill::CSkill(tstring _strSkillName, tstring _strAnimKey, _int* _iDotDamage,
	_float _fStunRatio, _float _fDamageRatio, _float _fCriticalRatio,
	_bool* _bArrAttack, _int _iMoveCnt, _int _iStress)
	: m_strSkillName(_strSkillName), m_strAnimKey(_strAnimKey), m_iMoveCnt(_iMoveCnt), m_iStress(_iStress),
	m_fStunRatio(_fStunRatio), m_fDamageRatio(_fDamageRatio), m_fCriticalRatio(_fCriticalRatio)
{
	memcpy(m_bArrAttack, _bArrAttack, sizeof(_bool) * 6);
	memcpy(m_iDotDamage, _iDotDamage, sizeof(_int) * 2);
}

CSkill::CSkill(const CSkill& rhs)
{
}

CSkill::~CSkill()
{
}

_bool CSkill::IsEnable()
{
	if (m_iActivateCnt == 0)
		return false;

	return true;
}