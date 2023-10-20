#pragma once

#include "GameObject.h"

class CSkill
{
public:
	CSkill(tstring _strSkillName, _bool* _arrActivatePos, _bool* _arrTargetPos, _bool _bToEnemy,
		_int _iActivateCnt, _int _iAbilityCnt, EAttackType* _eListAbility, _int _iMoveCnt,
		_int* _iDotDamage, _float _fStunRate, _int _iHeal, tstring _strAnimKey, tstring _strImgKey);

	CSkill(const CSkill& rhs);
	virtual ~CSkill();

public:
	_bool		UseSkill();
	

public:
	tstring		GetSkillName() { return m_strSkillName; }

	_bool		IsActivatePos(_int _idx) { return m_arrActivatePos[_idx]; }
	_bool		IsTargetPos(_int _idx) { return m_arrTargetPos[_idx]; }
	_bool*		GetTargetPos(){ return m_arrTargetPos; }
	_bool		IsToEnemy() { return m_bToEnemy; }
	_bool		CanActivateCnt() { if (0 == m_iActivateCnt) return false; return true; }
	void		DecreaseActivateCnt() { --m_iActivateCnt; }
	_int		GetAbliltyCnt() { return m_iAbilityCnt; }
	EAttackType*	GetListAbility() { return m_eListAbility; }

	_bool		IsEquipped() { return m_bEquipped; }
	void		SetEquipped(_bool _bEquipped) { m_bEquipped = _bEquipped; }

	_bool		IsUnlocked() { return m_bUnlocked; }
	void		SetUnlocked(_bool _bUnlocekd) { m_bUnlocked = _bUnlocekd; }

	_int		GetLevel() { return m_iLevel; }
	void		UpgradeLevel() { m_iLevel++; }

	_int		GetMoveCnt() { return m_iMoveCnt; }
	_int*		GetDotDamage() { return m_iDotDamage; }
	_float		GetStunRate() { return m_fStunRate; }
	_int		GetHeal() { return m_iHeal; }
	_float		GetDamageRatio() { return m_fDamageRatio; }
	_float		GetCriticalRatio() { return m_fCriticalRatio; }

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
	EAttackType m_eListAbility[3];

	// 장착 여부
	_bool		m_bEquipped;
	
	// 해금 여부
	_bool		m_bUnlocked;

	// 스킬 레벨
	_int		m_iLevel;

	// =========스킬값==========
	// 이동량
	_int		m_iMoveCnt;
	// 중독or출혈 데미지, 라운드
	_int		m_iDotDamage[2];
	// 기절 확률
	_float		m_fStunRate;
	// 힐량
	_int		m_iHeal;
	// 공격력 계수
	_float		m_fDamageRatio;
	// 치명타 계수
	_float		m_fCriticalRatio;

	// 애니메이션 키
	tstring		m_strAnimKey;
	// 스킬 이미지 키
	tstring		m_strImgKey;
};