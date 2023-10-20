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

	// ��ų �̸�
	tstring		m_strSkillName;

	// �ߵ� ��ġ
	_bool		m_arrActivatePos[4];
	// Ÿ�� ��ġ
	_bool		m_arrTargetPos[4];
	// Ÿ�� ��(true) or �Ʊ�(false)
	_bool		m_bToEnemy;

	// �� ���� �� ��� ���� Ƚ�� (-1�� �� ������)
	_int		m_iActivateCnt;

	// �ɷ� ����
	_int		m_iAbilityCnt;
	EAttackType m_eListAbility[3];

	// ���� ����
	_bool		m_bEquipped;
	
	// �ر� ����
	_bool		m_bUnlocked;

	// ��ų ����
	_int		m_iLevel;

	// =========��ų��==========
	// �̵���
	_int		m_iMoveCnt;
	// �ߵ�or���� ������, ����
	_int		m_iDotDamage[2];
	// ���� Ȯ��
	_float		m_fStunRate;
	// ����
	_int		m_iHeal;
	// ���ݷ� ���
	_float		m_fDamageRatio;
	// ġ��Ÿ ���
	_float		m_fCriticalRatio;

	// �ִϸ��̼� Ű
	tstring		m_strAnimKey;
	// ��ų �̹��� Ű
	tstring		m_strImgKey;
};