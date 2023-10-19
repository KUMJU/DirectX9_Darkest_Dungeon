#pragma once

#include "GameObject.h"

enum class ESkillAbility {
	ADDICT,			// 중독
	BLEED,			// 출혈
	STUN,			// 기절
	MOVE,			// 이동
	HEAL,			// 회복
	DAMAGE,			// 피해
	ENUM_END
};

class CSkill
{
	CSkill(tstring _strSkillName, _bool* _arrActivatePos, _bool* _arrTargetPos, _bool _bToEnemy,
		_int _iActivateCnt, _int _iAbilityCnt, ESkillAbility* _eListAbility, _int _iMoveCnt,
		_int* _iDotDamage, _float _fStunRate, _int _iHeal, tstring _strAnimKey, tstring _strImgKey);

	CSkill(const CSkill& rhs);
	virtual ~CSkill();

public:
	_bool		UseSkill();
	

public:
	tstring		GetSkillName() { return m_strSkillName; }

	_bool		IsActivatePos(_int _idx) { return m_arrActivatePos[_idx]; }
	_bool*		GetTargetPos(){ return m_arrTargetPos; }
	_bool		IsToEnemy() { return m_bToEnemy; }
	_int		GetActivateCnt() { return m_iActivateCnt; }
	void		DecreaseActivateCnt() { --m_iActivateCnt; }
	_int		GetAbliltyCnt() { return m_iAbilityCnt; }
	ESkillAbility*	GetListAbility() { return m_eListAbility; }

	_int		GetMoveCnt() { return m_iMoveCnt; }
	_int*		GetDotDamage() { return m_iDotDamage; }
	_float		GetStunRate() { return m_fStunRate; }
	_int		GetHeal() { return m_iHeal; }

	tstring		GetAnimKey() { return m_strAnimKey; }
	tstring		GetImgKey() { return m_strImgKey; }

protected:

	// 스킬 이름
	tstring		m_strSkillName;

	// 발동 위치
	_bool		m_arrActivatePos[4];
	// 타겟 위치
	_bool		m_arrTargetPos[4];
	// 타겟 적(true) or 아군(false)
	_bool		m_bToEnemy;

	// 한 전투 당 사용 가능 횟수 (-1일 시 무제한)
	_int		m_iActivateCnt;

	// 능력 개수
	_int		m_iAbilityCnt;
	ESkillAbility m_eListAbility[3];

	// =========스킬값==========
	// 이동량
	_int		m_iMoveCnt;
	// 중독or출혈 데미지, 라운드
	_int		m_iDotDamage[2];
	// 기절 확률
	_float		m_fStunRate;
	// 힐량
	_int		m_iHeal;

	// 애니메이션 키
	tstring		m_strAnimKey;
	// 스킬 이미지 키
	tstring		m_strImgKey;
};