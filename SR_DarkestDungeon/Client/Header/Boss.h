#pragma once

#include"GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CAnimator;

END

enum class EBossState
{
	// Phase1
	P1_IDLE,
	P1_ATTACK,
	P1_LASER1,
	P1_LASER2,
	P1_DASH,
	P1_CHANGE,

	// Phase2
	P2_IDLE,
	P2_DEATH,

	ENUM_END
};

class CBoss : public CGameObject
{
public:
	CBoss(LPDIRECT3DDEVICE9 pGraphicDev);
	CBoss(const CBoss& rhs);
	virtual ~CBoss();

public:
	virtual HRESULT			ReadyGameObject();
	virtual _int			UpdateGameObject(const _float& fTimeDelta);
	virtual void			LateUpdateGameObject();
	virtual void			RenderGameObject();

protected:
	virtual void			AddComponent();

	virtual void			FSM(const _float& fTimeDelta);
	virtual void			ChangeAnim();
	virtual void			AnimDuration(const _float& fTimeDelta);
	virtual void			KeyInput();

protected:
	// 플레이어와 Object의 거리를 계산하는 함수
	_float	CalcDistance();
	// 플레이어와 Object의 거리차 벡터를 구하는 함수
	_vec3	CalcDirection();
	void	ChasePlayer(const _float& fTimeDelta, float _fSpeed);

public:


protected:

	_int	m_iHp = 200;
	_float	m_fP1_IdleSpeed = 10.f;
	_float	m_fP1_DashSpeed = 130.f;

	float	m_fP1AttackTime = 0.06f * 12;
	float	m_fP1Laser1Time = 0.06f * 7;
	float	m_fP1Laser2Time = 0.06f * 4;
	float	m_fP1DashTime = 0.06f * 11;
	float	m_fP1ChangeTime = 0.06f * 43;
	
	EBossState m_eAnimState =		EBossState::P1_IDLE;	// 애니메이션 변경을 위한 상태값
	EBossState m_eCurAnimState =	EBossState::P1_IDLE;	// 애니메이션 변경을 위한 상태값
	EBossState m_ePrevAnimState =	EBossState::P1_IDLE;	// 애니메이션 변경을 위한 상태값
	tstring		m_strAnimKey = L"";				// 애니메이션 키
	tstring		m_strEffectKey = L"";				// 이펙트 키

	// 본체 관련 Component
	shared_ptr<CTransform>	m_pTransformCom = nullptr;
	shared_ptr<CRcTex>		m_pBufCom = nullptr;
	shared_ptr<CAnimator>	m_pTextureCom = nullptr;

	// 본체 피격, 타격 이펙트 관련 Component
	shared_ptr<CTransform> m_pEffectTransformCom = nullptr;
	shared_ptr<CRcTex> m_pEffectBufCom = nullptr;
	shared_ptr<CAnimator> m_pEffectTextureCom = nullptr;

	// 플레이어의 위치를 받아오기 위한 플레이어 오브젝트
	shared_ptr<CGameObject>	m_pPlayer = nullptr;
	shared_ptr<CTransform>	m_pPlayerTransformCom = nullptr;
};