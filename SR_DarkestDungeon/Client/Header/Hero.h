#pragma once
#include "Creature.h"
#include "Skill.h"

enum class EHeroState
{
	// ������
	IDLE,
	WALK,

	// ����
	COMBAT,			// ���� �� IDLE
	DEATH_DOOR,		// ������ ����
	AFFLICTION,		// �ر�
	VIRTUE,			// ���
	STUN,			// ����
	BLEED,			// ����
	BLIGHT,			// �ߵ�
	DEATH,			// ���
	ENUM_END
};

typedef struct tagHeroStat
{
	_int		iStress;		// ��Ʈ����
	_float		fCritical;		// ġ��Ÿ Ȯ��
} HEROSTAT;

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
	CHero(const CHero& rhs);
	virtual ~CHero();

public:
	virtual HRESULT ReadyGameObject() override;
	virtual _int UpdateGameObject(const _float& fTimeDelta) override;
	virtual void LateUpdateGameObject() override;
	virtual void RenderGameObject() override;

public:
	void		SelectSkill(_int _iSkillID);

	void		AttackCreature(_int _iEnemyPos, shared_ptr<CCreature> _pCreature);

protected:
	virtual void		AddComponent();
	virtual void		SetAnimKey(tstring strAnimKey) { m_strAnimKey = strAnimKey; }

public:
	HEROSTAT	GetHeroStat() { return m_tHeroStat; }
	void		SetHeroStat(HEROSTAT _tStat) { m_tHeroStat = _tStat; }

	_bool		IsDeathDoor() { return m_bDeathDoor; }
	void		SetDeathDoor(_bool _bDeathDoor) { m_bDeathDoor = _bDeathDoor; }

	_bool		IsAffliction() { return m_bAffliction; }
	void		SetAffliction(_bool _bAffliction) { m_bAffliction = _bAffliction; }

	_bool		IsVirtue() { return m_bVirtue; }
	void		SetVirtue(_bool _bVirtue) { m_bVirtue = _bVirtue; }

protected:

	shared_ptr<vector<shared_ptr<CSkill>>> m_pVecSkill;
	HEROSTAT			m_tHeroStat;

	shared_ptr<vector<shared_ptr<EHeroState>>>	m_pVecHeroState;

	_bool				m_bDeathDoor = false;		// ������ ���� ����
	_bool				m_bAffliction = false;		// �ر� ����
	_bool				m_bVirtue = false;			// ��� ����

	shared_ptr<vector<shared_ptr<tstring>>>	m_pVecAnimKey;

	shared_ptr<CSkill>	m_pSelectedSkill;

private:
	virtual void	Free();
};