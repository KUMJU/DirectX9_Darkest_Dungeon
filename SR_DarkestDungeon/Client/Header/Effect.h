#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CAnimator;
class CTransform;
class CRcTex;

END

class CEffect : public CGameObject
{
public:
	explicit CEffect(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CEffect();

public:
	virtual HRESULT ReadyGameObject() override;
	virtual _int	UpdateGameObject(const _float& fTimeDelta);
	virtual void	LateUpdateGameObject();
	virtual void	RenderGameObject();

public:
	virtual void	AddComponent();

	// 애니메이션 설정
	void	SetInfo(tstring _strAnimKey, _vec2 _vTextureScale, _vec3* _vPos, const _vec3* _vScale, _float _fAnimTime, _bool _bLoop);
	void	SetAnimKey(tstring _strAnimKey) { m_strAnimKey = _strAnimKey; }
	void	SetTime(_float _fAnimTime) { m_fAnimTime = _fAnimTime; }
	void	SetLoop(_bool _bLoop) { m_bLoop = _bLoop; }

	// Transform 설정
	void	SetPos(_vec3* _vPos); 
	void	SetScale(const _vec3* _vScale);

public:
	void	SetMove(_bool _bMove, _vec3 _vDir, _float _fTime);
	void	SetFade(_bool _bFade, _bool _bAppear, _float _fTime);
	_bool	GetFade() { return m_bFading; }

	void	Move();
	void	Fade();

private:
	shared_ptr<CTransform>	m_pTransformCom;
	shared_ptr<CAnimator>	m_pAnimatorCom;
	shared_ptr<CRcTex>		m_pRcTexCom;

private:
	_float	m_fDeltaTime;

	// 이펙트를 호출한 객체의 포지션
	_vec3*	m_vPos;
	// 이펙트를 호출한 객체의 스케일
	const _vec3*	m_vScale;

	// 이펙트 출력을 위한 갭 (포지션, 스케일 차이 세팅)
	_vec3	m_vPosGap = { 0.f, 0.f, 0.f };
	_vec3	m_vScaleGap = { 0.f, 0.f, 0.f };

	// 애니메이션 키
	tstring	m_strAnimKey;
	// 애니메이션 동작 속도
	_float	m_fAnimTime;
	// 반복 여부
	_bool	m_bLoop;
	// 투명도
	_int	m_iAlpha;

	// 특정 기능 누적 시간
	_float	m_fAccumMoveTime = 0.f;
	_float	m_fAccumFadeTime = 0.f;

	// 특정 기능 종료 시간
	_float	m_fMoveTime = 0.f;
	_float	m_fFadeTime = 0.f;

	// 특정 기능 상태
	_bool	m_bMoving = false;
	_bool	m_bFading = false;

	// 무브 방향
	_vec3	m_vMoveDir;

	// 페이드 인 or 아웃
	_bool	m_bAppear;
};