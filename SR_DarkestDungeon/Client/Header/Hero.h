#pragma once
#include "Creature.h"
#include "Skill.h"

enum class EHeroState
{
	// 비전투
	IDLE,
	WALK,

	// 전투
	COMBAT,			// 전투 중 IDLE
	DEATH_DOOR,		// 죽음의 문턱
	AFFLICTION,		// 붕괴
	VIRTUE,			// 기상
	STUN,			// 기절
	BLEED,			// 출혈
	BLIGHT,			// 중독
	DEATH,			// 사망
	ENUM_END
};

typedef struct tagHeroStat
{
	_int		iStress;		// 스트레스
	_float		fCritical;		// 치명타 확률
} HEROSTAT;

// 붕괴 (75%)
enum class EAffliction
{
	// 이기적
	// 본인이 맞을 시 아군들에게 스트레스
	// 아군이 공격 시 해당 아군에게 스트레스
	SELFISH,

	// 비이성적
	// 회복 거부
	// 자해
	IRRATIONAL,

	ENUM_END
};

// 기상 (25%)
enum class EVirtue
{
	// 충만한 용기
	// 턴이 올 때마다 모든 파티원의 스트레스를 4씩 감소
	COURAGEOUS,

	// 원기 왕성
	// 턴 시작 시 자신의 최대 체력의 10% 회복
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

	_bool				m_bDeathDoor = false;		// 죽음의 문턱 여부
	_bool				m_bAffliction = false;		// 붕괴 여부
	_bool				m_bVirtue = false;			// 기상 여부

	shared_ptr<vector<shared_ptr<tstring>>>	m_pVecAnimKey;

	shared_ptr<CSkill>	m_pSelectedSkill;

private:
	virtual void	Free();
};