#pragma once

#include "GameObject.h"

enum class ESkillAbility {
	ADDICT,			// �ߵ�
	BLEED,			// ����
	STUN,			// ����
	MOVE,			// �̵�
	HEAL,			// ȸ��
	DAMAGE,			// ����
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
	ESkillAbility m_eListAbility[3];

	// =========��ų��==========
	// �̵���
	_int		m_iMoveCnt;
	// �ߵ�or���� ������, ����
	_int		m_iDotDamage[2];
	// ���� Ȯ��
	_float		m_fStunRate;
	// ����
	_int		m_iHeal;

	// �ִϸ��̼� Ű
	tstring		m_strAnimKey;
	// ��ų �̹��� Ű
	tstring		m_strImgKey;
};