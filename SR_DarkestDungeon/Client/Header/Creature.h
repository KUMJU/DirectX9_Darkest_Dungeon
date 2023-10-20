#pragma once

#include"GameObject.h"

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

}STAT;

// Attack 타입
enum class ATTACKTYPE
{ 
	ATTACK_NORMAL, 
	ATTACK_BLIGHT, 
	ATTACK_BLEED, 
	ATTACK_STRESS
};

class CCreature : public CGameObject
{
public:
	explicit CCreature(LPDIRECT3DDEVICE9 pGraphicDev);
	CCreature(const CCreature& rhs);
	virtual ~CCreature();

public:
	virtual HRESULT ReadyGameObject() override;
	virtual _int UpdateGameObject(const _float& fTimeDelta) override;
	virtual void LateUpdateGameObject() override;
	virtual void RenderGameObject() override;

public:
	STAT	GetCommonStat() { return m_tCommonStat; }
	void	SetCommonStat(STAT _tStat) { m_tCommonStat = _tStat; }
	void	SetTurn(_bool _bTurn) { m_bMyTurn = _bTurn; }
	void	SetHitted(_bool _bHitted) { m_bHitted = _bHitted; }
	void	SetEffectOn(_bool _bEffectOn) { m_bEffectOn = _bEffectOn; }
	void	SetBlight(_bool _bBlight) { m_bBlighting = true; }
	void	SetBleed(_bool _bBleed) { m_bBleeding = true; }

protected:
	// 턴 시작시
	virtual void	StartTurn();

	// 다른 크리처 공격시
	virtual void	AttackCreature(CCreature* _pCreature, _float _fSkillRatio, ATTACKTYPE _eAttackTYPE,
		_int _iDotDamage = 0, _int _iTurn = 0);

	// 공격 종료시
	virtual void	EndAttack(CCreature* _pCreature);

	// 중독 공격
	virtual void	BlightAttack(_int _iBlightDmg, _int _iTurn);

	// 출혈 공격
	virtual void	BleedAttack(_int _iBleedDmg, _int _iTurn);

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

	// 예시) 내가 공격시 나오는 이펙트 출력, 상대는 피격 이펙트 출력

protected:
	STAT		m_tCommonStat;				// 스탯
	_bool		m_bMyTurn = false;			// 자신의 턴 여부 

	_bool		m_bHitted = false;			// 자신의 피격상태 여부
	_bool		m_bEffectOn = false;		// 이펙트 여부
	// m_bHitted && m_bEffectOn -> 피격 애니메이션 on
	// !m_bHitted && m_bEffectOn -> 타격 애니메이션 on

	_bool		m_bBlighting = false;			// 중독여부
	_int		m_bBlightDot[4] = { 0 };	// 턴마다 중독 도트뎀
	_bool		m_bBleeding = false;			// 출혈여부
	_int		m_bBleedDot[4] = { 0 };		// 턴마다 출혈 도트뎀

protected:
	virtual void	Free();
};

