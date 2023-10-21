#pragma once
#include "Creature.h"
#include "Skill.h"

// �ر� (75%)
enum class EAffliction
{
	// �̱���
	// ������ ���� �� �Ʊ��鿡�� ��Ʈ����
	// �Ʊ��� ���� �� �ش� �Ʊ����� ��Ʈ����
	SELFISH,

	// ���̼���
	// ȸ�� �ź�
	// ����
	IRRATIONAL,

	ENUM_END
};

// ��� (25%)
enum class EVirtue
{
	// �游�� ���
	// ���� �� ������ ��� ��Ƽ���� ��Ʈ������ 4�� ����
	COURAGEOUS,

	// ���� �ռ�
	// �� ���� �� �ڽ��� �ִ� ü���� 10% ȸ��
	VIGOROUS,

	ENUM_END
};

class CHero : public CCreature
{
public:
	explicit CHero(LPDIRECT3DDEVICE9 pGraphicDev);

	// ���� ������ ��
	CHero(LPDIRECT3DDEVICE9 pGraphicDev, STAT _tCommonStat, _int _iPosition,
		shared_ptr<vector<shared_ptr<CSkill>>> _pVecSkill, _float _fCritical);

	// ���� ��ġ, ��ų�� ���߿� ������ ��	
	CHero(LPDIRECT3DDEVICE9 pGraphicDev, STAT _tCommonStat);

	CHero(const CHero& rhs);
	virtual ~CHero();

public:
	virtual HRESULT ReadyGameObject() override;
	virtual _int UpdateGameObject(const _float& fTimeDelta) override;
	virtual void LateUpdateGameObject() override;
	virtual void RenderGameObject() override;

public:
	shared_ptr<CSkill>		SelectSkill(_int _iSkillID);

	virtual	void			StressAction();


protected:
	virtual void		AddComponent();
	//virtual void		SetAnimKey(tstring strAnimKey) { m_strAnimKey = strAnimKey; }

public:
	_int		GetStress() { return m_iStress; }
	void		SetStress(_int _iValue) { m_iStress = _iValue; }
	void		IncreaseStress(_int _iValue) { m_iStress += _iValue; }
	void		DecreaseStress(_int _iValue) { m_iStress -= _iValue; }

	_float		GetCritical() { return m_fCritical; }
	void		SetCritical(_float _fValue) { m_fCritical = _fValue; }
	void		ChangeCritical(_float _fValue) { m_fCritical *= _fValue; }

	_bool		IsDeathDoor() { return m_bDeathDoor; }
	void		SetDeathDoor(_bool _bDeathDoor) { m_bDeathDoor = _bDeathDoor; }

	_bool		IsAffliction() { return m_bAffliction; }
	void		SetAffliction(_bool _bAffliction) { m_bAffliction = _bAffliction; }

	_bool		IsVirtue() { return m_bVirtue; }
	void		SetVirtue(_bool _bVirtue) { m_bVirtue = _bVirtue; }

protected:

	_int				m_iStress = 0;		// ��Ʈ����
	_float				m_fCritical;		// ġ��Ÿ Ȯ��

	_bool				m_bDeathDoor = false;		// ������ ���� ����

	_bool				m_bAffliction = false;		// �ر� ����
	EAffliction			m_eAffliction = EAffliction::ENUM_END;

	_bool				m_bVirtue = false;			// ��� ����
	EVirtue				m_eVirtue = EVirtue::ENUM_END;

	//shared_ptr<vector<shared_ptr<tstring>>>	m_pVecAnimKey;

private:
	virtual void	Free();
};