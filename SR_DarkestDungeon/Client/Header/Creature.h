#pragma once

#include"GameObject.h"
#include "Skill.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CAnimator;

END

// Creature 공통 스탯
typedef struct tagCreatureCommonStat
{
	_int		iHp;			// 체력
	_int		iDodge;			// 회피율
	_int		iSpeed;			// 속도
	_int		iAttackPower;	// 순수공격력(실제 데미지는 스킬에 따른 공격력 계수의 곱)
	_int		iOrder;			// 순서(중심에서부터 0~3)
}STAT;

enum class EAnimState
{
	// 비전투
	IDLE,
	WALK,

	// 전투
	COMBAT,			// 전투 중 IDLE
	BESHOT,			// 피격
	DEATH,			// 사망

	// 몬스터용
	CORPSE,			// 시체

	// 영웅용
	AFFLICTION,		// 붕괴
	VIRTUE,			// 기상
	ENUM_END
};

class CCreature : public CGameObject
{
public:
	explicit CCreature(LPDIRECT3DDEVICE9 pGraphicDev);

	// 스킬까지 세팅할 때
	CCreature(LPDIRECT3DDEVICE9 pGraphicDev, STAT _tCommonStat, _int _iPosition,
		shared_ptr<vector<shared_ptr<CSkill>>> _pVecSkill);

	// 전투 위치와 스킬은 나중에 세팅할 때	
	CCreature(LPDIRECT3DDEVICE9 pGraphicDev, STAT _tCommonStat);

	CCreature(const CCreature& rhs);
	virtual ~CCreature();

public:
	virtual HRESULT ReadyGameObject() override;
	virtual _int UpdateGameObject(const _float& fTimeDelta) override;
	virtual void LateUpdateGameObject() override;
	virtual void RenderGameObject() override;

public:
	// 인덱스로 스킬 불러오기 (생각 좀 해봐야 할듯,, 스킬을 건너뛰고 장착할 수 있어서)
	shared_ptr<CSkill> GetSkill(_int _iIdx) { return (*m_pVecSkill)[_iIdx]; }
	// 이름으로 스킬 불러오기 (일단 이거 사용)
	shared_ptr<CSkill> GetSkill(tstring _strSkillName);

	HRESULT	SetSkill(shared_ptr<vector<shared_ptr<CSkill>>> _pSkill);	// 스킬 vector로 한 번에 넣어주기

	STAT	GetCommonStat() { return m_tCommonStat; }
	void	SetCommonStat(STAT _tStat) { m_tCommonStat = _tStat; }

	_int	GetPosition() { return m_iPosition; }
	void	SetPosition(_int _iPosition) { m_iPosition += _iPosition; }

	void	SetHitted(_bool _bHitted) { m_bHitted = _bHitted; }
	void	SetEffectOn(_bool _bEffectOn) { m_bEffectOn = _bEffectOn; }

	void	SetBlight(_bool _bBlight) { m_bState[0] = true; }
	void	SetBleed(_bool _bBleed) { m_bState[1] = true; }
	void	SetStun(_bool _bStun) { m_bState[2] = true; }

	_int	GetHp() { return m_tCommonStat.iHp; }
	void	SetHp(_int _iValue) { m_tCommonStat.iHp = _iValue; }
	void	IncreaseHP(_int _iValue) { m_tCommonStat.iHp += _iValue; }
	void	DecreaseHP(_int _iValue) { m_tCommonStat.iHp -= _iValue; }

	_int	GetDodge() { return m_tCommonStat.iDodge; }
	void	SetDodge(_int _iValue) { m_tCommonStat.iDodge = _iValue; }

	_int	GetSpeed() { return m_tCommonStat.iSpeed; }
	void	SetSpeed(_int _iValue) { m_tCommonStat.iSpeed = _iValue; }

	_int	GetAttackPower() { return m_tCommonStat.iAttackPower; }
	void	SetAttackPower(_int _iValue) { m_tCommonStat.iAttackPower = _iValue; }

	_int	GetOrder() { return m_tCommonStat.iOrder; }
	void	SetOrder(_int _iValue) { m_tCommonStat.iOrder = _iValue; }




	// 쓰는지??
	_bool	GetDone() { return m_bDone; }
	_bool	GetAbleAct() { return m_bAbleAct; }
	void	SetTurn(_bool _bTurn) { m_bMyTurn = _bTurn; }
	void	SetDone(_bool _bDone) { m_bDone = _bDone; }
	void	SetAbleAct(_bool _bAbleAct) { m_bAbleAct = _bAbleAct; }

protected:
	// 턴 시작시
	virtual void	StartTurn();

	// 다른 크리처 공격시
	virtual void	AttackCreature(shared_ptr<CCreature> _pCreature, shared_ptr<CSkill> _pSkill);

	// 공격 종료시
	virtual void	EndAttack(shared_ptr<CGameObject> _pCreature);

	// 중독 공격
	virtual void	BlightAttack(_int* _iDotDam);

	// 출혈 공격
	virtual void	BleedAttack(_int* _iDotDam);

	// 중독 치료
	virtual void	BlightCure();

	// 출혈 치료
	virtual void	BleedCure();

protected:
	virtual void			AddComponent();
	virtual void			ClimbingTerrain();


protected:
	// Creature Common Component
	// 본체 관련 Component
	shared_ptr<CTransform> m_pTransformCom = nullptr;
	shared_ptr<CRcTex> m_pBufCom = nullptr;
	shared_ptr<CAnimator> m_pTextureCom = nullptr;

	// 본체 피격, 타격 이펙트 관련 Component
	shared_ptr<CTransform> m_pEffectTransformCom = nullptr;
	shared_ptr<CRcTex> m_pEffectBufCom = nullptr;
	shared_ptr<CAnimator> m_pEffectTextureCom = nullptr;

protected:
	STAT		m_tCommonStat;				// 스탯

	_int		m_iPosition;				// 위치 (0~3)

	_bool		m_bDeath = false;			// 사망 여부
	_bool		m_bState[4] = { false };	// 순서대로 중독, 출혈, 기절, 시체 여부

	_int		m_bBlightDot[4] = { 0 };	// 턴마다 중독 도트뎀
	_int		m_bBleedDot[4] = { 0 };		// 턴마다 출혈 도트뎀

	_bool		m_bHitted = false;			// 자신의 피격상태 여부
	_bool		m_bEffectOn = false;		// 이펙트 여부
	// m_bHitted && m_bEffectOn -> 피격 애니메이션 on
	// !m_bHitted && m_bEffectOn -> 타격 애니메이션 on
	// 예시) 내가 공격시 나오는 이펙트 출력, 상대는 피격 이펙트 출력

	shared_ptr<vector<shared_ptr<CSkill>>>	m_pVecSkill;	// 스킬

	EAnimState m_eAnimState = EAnimState::IDLE;	// 애니메이션 변경을 위한 상태값
	tstring		m_strAnimKey = L"";				// 애니메이션 키
	tstring		m_strEffectKey = L"";				// 이펙트 키

	// 쓰는지??
	_bool		m_bMyTurn = false;			// 자신의 턴 여부 
	_bool		m_bDone = false;			// 이번 턴에 행동 했는지 여부
	_bool		m_bAbleAct = true;			// 이번 턴에 행동 가능한지 여부(기절이나 시체사라지면 불가능)

protected:
	virtual void	Free();
};
