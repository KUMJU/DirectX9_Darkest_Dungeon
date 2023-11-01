#pragma once
#include "Creature.h"
#include "Skill.h"

class CHeroStat;

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

	// 전부 세팅할 때
	CHero(LPDIRECT3DDEVICE9 pGraphicDev, STAT _tCommonStat, _int _iPosition,
		vector<shared_ptr<CSkill>>& _pVecSkill, _float _fCritical);

	// 전투 위치, 스킬은 나중에 세팅할 때	
	CHero(LPDIRECT3DDEVICE9 pGraphicDev, STAT _tCommonStat);

	CHero(const CHero& rhs);
	virtual ~CHero();

public:
	virtual HRESULT ReadyGameObject() override;
	virtual _int UpdateGameObject(const _float& fTimeDelta) override;
	virtual void LateUpdateGameObject() override;
	virtual void RenderGameObject() override;

public:
	vector<shared_ptr<CSkill>>* GetSkillVector() { return &m_pVecSkill; }
	shared_ptr<CSkill>		SelectSkill(_int _iSkillID);
	virtual	void			StressUp();

	// 선택한 두 스킬의 순서를 스왑하는 함수
	virtual void			SwapSkill(_int _iIdx1, _int _iIdx2);


protected:
	virtual void		AddComponent();
	virtual void		SetAnimState(EAnimState _eAnimState) { m_eCurAnimState = _eAnimState; }
	virtual void		ChangeAnimState();
	virtual void		SetAnimDelay(const _float& fTimeDelta);
	virtual void		ChangeAnim() {}

	// 영웅 턴 시작시에 호출해서 동작해주기
	virtual void		StressEvent();

	virtual void		PickingObj();

public:
	const _tchar* Get_String4() const { return m_szString4; }

	_int		GetStress() { return m_iStress; }
	void		SetStress(_int _iValue) { m_iStress = _iValue; }
	void		IncreaseStress(_int _iValue) { m_iStress += _iValue; }
	void		DecreaseStress(_int _iValue)
	{
		m_iStress -= _iValue;
		if (m_iStress < 0) m_iStress = 0;
	}

	_float		GetCritical() { return m_fCritical; }
	void		SetCritical(_float _fValue) { m_fCritical = _fValue; }
	void		ChangeCritical(_float _fValue) { m_fCritical *= _fValue; }

	_bool		IsDeathDoor() { return m_bDeathDoor; }
	void		SetDeathDoor(_bool _bDeathDoor) { m_bDeathDoor = _bDeathDoor; }

	_bool		IsAffliction() { return m_bAffliction; }
	void		SetAffliction(_bool _bAffliction) { m_bAffliction = _bAffliction; }

	_bool		IsVirtue() { return m_bVirtue; }
	void		SetVirtue(_bool _bVirtue) { m_bVirtue = _bVirtue; }

	EHeroType	GetHeroType() { return m_eHeroType; }

protected:

	_int				m_iStress = 0;		// 스트레스
	_float				m_fCritical;		// 치명타 확률

	_bool				m_bDeathDoor = false;		// 죽음의 문턱 여부

	_bool				m_bAffliction = false;		// 붕괴 여부
	EAffliction			m_eAffliction = EAffliction::ENUM_END;

	_bool				m_bVirtue = false;			// 기상 여부
	EVirtue				m_eVirtue = EVirtue::ENUM_END;

	_bool				m_bDelay = false;			// UI 출력 시간 등 (애니메이션이 변하진 않지만 기다려야하는 시간)

	// 딜레이를 만들기 위한 상태값들
	_float				m_fHittedTime = HITTEDTIME;
	_float				m_fAttackTime = ATTACKTIME;
	_float				m_fChangeTime = CHANGETIME;
	_float				m_fDelayTime = DELAYTIME;

	_bool				m_bInDungeon = true;

	_bool				m_bAttacking1 = false;
	_bool				m_bAttacking2 = false;
	_bool				m_bAttacking3 = false;
	_bool				m_bAttacking4 = false;

	_bool				m_bStressChanged = false;

	EHeroType			m_eHeroType = EHeroType::ENUM_END;

	shared_ptr<CHeroStat> m_pStatUI = nullptr;

	// 스탯 Info ON/Off
	_bool		m_bShowStatUI = false;

	//shared_ptr<vector<shared_ptr<tstring>>>	m_pVecAnimKey;

	_tchar		m_szString4[128];

private:
	virtual void	Free();
};