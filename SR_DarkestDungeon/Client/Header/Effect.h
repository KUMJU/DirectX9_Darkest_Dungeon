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

	// �ִϸ��̼� ����
	void	SetInfo(tstring _strAnimKey, _vec2 _vTextureScale, _vec3* _vPos, const _vec3* _vScale, _float _fAnimTime, _bool _bLoop);
	void	SetAnimKey(tstring _strAnimKey) { m_strAnimKey = _strAnimKey; }
	void	SetTime(_float _fAnimTime) { m_fAnimTime = _fAnimTime; }
	void	SetLoop(_bool _bLoop) { m_bLoop = _bLoop; }

	// Transform ����
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

	// ����Ʈ�� ȣ���� ��ü�� ������
	_vec3*	m_vPos;
	// ����Ʈ�� ȣ���� ��ü�� ������
	const _vec3*	m_vScale;

	// ����Ʈ ����� ���� �� (������, ������ ���� ����)
	_vec3	m_vPosGap = { 0.f, 0.f, 0.f };
	_vec3	m_vScaleGap = { 0.f, 0.f, 0.f };

	// �ִϸ��̼� Ű
	tstring	m_strAnimKey;
	// �ִϸ��̼� ���� �ӵ�
	_float	m_fAnimTime;
	// �ݺ� ����
	_bool	m_bLoop;
	// ����
	_int	m_iAlpha;

	// Ư�� ��� ���� �ð�
	_float	m_fAccumMoveTime = 0.f;
	_float	m_fAccumFadeTime = 0.f;

	// Ư�� ��� ���� �ð�
	_float	m_fMoveTime = 0.f;
	_float	m_fFadeTime = 0.f;

	// Ư�� ��� ����
	_bool	m_bMoving = false;
	_bool	m_bFading = false;

	// ���� ����
	_vec3	m_vMoveDir;

	// ���̵� �� or �ƿ�
	_bool	m_bAppear;
};