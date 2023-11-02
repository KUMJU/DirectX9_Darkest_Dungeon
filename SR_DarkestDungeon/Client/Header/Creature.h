#pragma once

#include"GameObject.h"
#include "Skill.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CAnimator;

END

class CStatView;

// Creature 공통 스탯
typedef struct tagCreatureCommonStat
{
	_int		iHp;			// 체력
	_int		iMaxHp;			// 최대체력
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

	// 스킬	
	SKILL1,
	SKILL2,
	SKILL3,
	SKILL4,
	SKILL5,
	SKILL6,
	SKILL7,

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
		vector<shared_ptr<CSkill>>& _pVecSkill);

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
	shared_ptr<CSkill> GetSkill(_int _iIdx) { return m_pVecSkill[_iIdx]; }
	// 이름으로 스킬 불러오기 (일단 이거 사용)
	shared_ptr<CSkill> GetSkill(tstring _strSkillName);

	//HRESULT	SetSkill(shared_ptr<vector<shared_ptr<CSkill>>> _pSkill);	// 스킬 vector로 한 번에 넣어주기
	void	SetSkill(vector<shared_ptr<CSkill>>& _vVec)
	{
		m_pVecSkill = _vVec;
	}

	_int	GetSkillNum() { return (_int)size(m_pVecSkill); }

	_bool	GetMoving() { return m_bMoving; }
	void	SetMoving(_bool _bMoving) { m_bMoving = _bMoving; }

	_bool	GetAttackMoving() { return m_bAttackMoving; }
	void	SetAttackMoving(_bool _bMoving) { m_bAttackMoving = _bMoving; }

	_float	GetMovingSpeed() { return m_fMovingSpeed; }
	void	SetMovingSpeed(_float _bMovingSpeed) { m_fMovingSpeed = _bMovingSpeed; }

	_vec3	GetTargetPos() { return m_vTargetPos; }
	void	SetTargetPos(_vec3 _vTargetPos) { m_vTargetPos = _vTargetPos; }

	_vec3	GetTargetPos2() { return m_vTargetPos2; }
	void	SetTargetPos2(_vec3 _vTargetPos) { m_vTargetPos2 = _vTargetPos; }

	STAT	GetCommonStat() { return m_tCommonStat; }
	void	SetCommonStat(STAT _tStat) { m_tCommonStat = _tStat; }

	_int	GetPosition() { return m_iPosition; }
	void	SetPosition(_int _iPosition) { m_iPosition += _iPosition; }

	_int	GetSize() { return m_iSize; }
	void	SetSize(_int _iSize) { m_iSize = _iSize; }

	_bool	GetHitted() { return m_bHitted; }
	void	SetHitted(_bool _bHitted) { m_bHitted = _bHitted; }
	void	SetEffectOn(_bool _bEffectOn) { m_bEffectOn = _bEffectOn; }
	_bool	GetAttacking(_int _iIndex) { return m_bAttacking[_iIndex]; }
	void	SetAttacking(_bool _bAttacking, _int _iIndex) { m_bAttacking[_iIndex] = _bAttacking; }
	_bool	IsAttacking();	// 공격하고 있는지

	_bool	GetIsStun() { return m_bState[2]; }
	_bool	GetIsCorpse() { return m_bCorpse; }
	_bool	GetIsDeath() { return m_bDeath; }
	_bool	GetIsBeforeDeath() { return m_bBeforeDeath; }
	void	SetDeath(_bool _bDeath) { m_bDeath = _bDeath; }
	void	SetBeforeDeath(_bool _bBeforeDeath) { m_bBeforeDeath = _bBeforeDeath; }
	void	SetBlight(_bool _bBlight) { m_bState[0] = true; }
	void	SetBleed(_bool _bBleed) { m_bState[1] = true; }
	void	SetStun(_bool _bStun) { m_bState[2] = true; }
	void	SetStartBarOn(_bool _bStatBarOn) { bStatBarOn = _bStatBarOn; }

	_int	GetMaxHp() { return m_tCommonStat.iMaxHp; }
	void	SetFront(_bool _bFront) { m_bFront = _bFront; }

	_int	GetHp() { return m_tCommonStat.iHp; }
	void	SetHp(_int _iValue) { m_tCommonStat.iHp = _iValue; }
	void	IncreaseHP(_int _iValue) {
		m_tCommonStat.iHp += _iValue;
		if (m_tCommonStat.iHp >= m_tCommonStat.iMaxHp)
			m_tCommonStat.iHp = m_tCommonStat.iMaxHp;
	}
	void	DecreaseHP(_int _iValue)
	{ 
		m_tCommonStat.iHp -= _iValue; 
		if (m_tCommonStat.iHp < 0)
		{
			m_tCommonStat.iHp = 0;
		}
	}

	_int	GetCurrentBleed() { return m_bBleedDot[0]; }
	_int	GetCurrentPoision() { return m_bBlightDot[0]; }

	_int	GetDodge() { return m_tCommonStat.iDodge; }
	void	SetDodge(_int _iValue) { m_tCommonStat.iDodge = _iValue; }

	_int	GetSpeed() { return m_tCommonStat.iSpeed; }
	void	SetSpeed(_int _iValue) { m_tCommonStat.iSpeed = _iValue; }

	_int	GetAttackPower() { return m_tCommonStat.iAttackPower; }
	void	SetAttackPower(_int _iValue) { m_tCommonStat.iAttackPower = _iValue; }

	_int	GetOrder() { return m_tCommonStat.iOrder; }
	void	SetOrder(_int _iValue) { m_tCommonStat.iOrder = _iValue; }

	const _tchar* Get_String() const { return m_szString; }
	const _tchar* Get_String2() const { return m_szString2; }
	const _tchar* Get_String3() const { return m_szString3; }

	void	StartCalculate();


	// 쓰는지??
	_bool	GetDone() { return m_bDone; }
	_bool	GetAbleAct() { return m_bAbleAct; }
	_bool	GetTurn() { return m_bMyTurn; }
	void	SetTurn(_bool _bTurn) { m_bMyTurn = _bTurn; }
	void	SetDone(_bool _bDone) { m_bDone = _bDone; }
	void	SetAbleAct(_bool _bAbleAct) { m_bAbleAct = _bAbleAct; }

	virtual void	AttackCreature(shared_ptr<CCreature> _pCreature, shared_ptr<CSkill> _pSkill);
	void	MovePos(_vec3 _vPos, const _float& fTimeDelta, _float _fSpeed);
	_float	MovingSpeed(_vec3 _vPos, _float _fMovingTime);
	void	OffTurnCursor();	// 자기차례임을 나타내는 cursor 종료
	void	OnTurnCursor();	// 자기차례임을 나타내는 cursor 키기

public:

	// 다른 크리처 공격시

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

	//스탯 인포 설정
	void SettingStatBar();

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
	_bool		m_bIsHero = false;
	STAT		m_tCommonStat;				// 스탯

	_int		m_iPosition;				// 위치 (0~3)

	_bool		m_bDeath = false;			// 사망 여부(몬스터는 시체까지 소멸할때, 영웅은 사망할때)
	_bool		m_bCorpse = false;			// 몬스터의 시체여부
	_bool		m_bBeforeDeath = false;		// 영웅의 죽음의 일격 여부
	_bool		m_bState[4] = { 0 };	// 순서대로 중독, 출혈, 기절, 시체 여부

	_int		m_bBlightDot[4] = { 0 };	// 턴마다 중독 도트뎀
	_int		m_bBleedDot[4] = { 0 };		// 턴마다 출혈 도트뎀

	_bool		m_bHitted = false;			// 자신의 피격상태 여부
	_bool		m_bEffectOn = false;		// 이펙트 여부
	// m_bHitted && m_bEffectOn -> 피격 애니메이션 on
	// !m_bHitted && m_bEffectOn -> 타격 애니메이션 on
	// 예시) 내가 공격시 나오는 이펙트 출력, 상대는 피격 이펙트 출력

	vector<shared_ptr<CSkill>>	m_pVecSkill;	// 스킬

	EAnimState m_eAnimState = EAnimState::IDLE;	// 애니메이션 변경을 위한 상태값
	EAnimState m_eCurAnimState = EAnimState::IDLE;	// 애니메이션 변경을 위한 상태값
	EAnimState m_ePrevAnimState = EAnimState::IDLE;	// 애니메이션 변경을 위한 상태값
	tstring		m_strAnimKey = L"";				// 애니메이션 키
	tstring		m_strEffectKey = L"";				// 이펙트 키

	// 쓰는지??
	_bool		m_bMyTurn = false;			// 자신의 턴 여부 
	_bool		m_bDone = false;			// 이번 턴에 행동 했는지 여부
	_bool		m_bAbleAct = true;			// 이번 턴에 행동 가능한지 여부(기절이나 시체사라지면 불가능)

	// 스킬 사용중
	_bool		m_bAttacking[7] = { 0 };

	_int		m_iSize = 1;	// 캐릭터 크기

	_tchar		m_szString[128];
	_tchar		m_szString2[128];
	_tchar		m_szString3[128];

	_bool		m_bMoving = false;			// 맞아서 움직이는중(기술, 당기기 등)
	_bool		m_bAttackMoving = false;	// 공격스킬로 인해 움직이는중
	_vec3		m_vTargetPos = { 0, 0, 0 };		// 목적지 1
	_vec3		m_vTargetPos2 = { 0, 0, 0 };	// 목적지 2
	_float		m_fMovingSpeed = 0.f;

	//스탯 표시창
	shared_ptr<CStatView> m_pStatInfo = nullptr;
	//스탯Info ON/Off
	_bool		bStatBarOn = true;

	// 플레이어 앞인지 뒤인지
	_bool		m_bFront = true;


protected:
	virtual void	Free();
};
