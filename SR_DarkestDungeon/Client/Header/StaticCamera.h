#pragma once

#include"Camera.h"

BEGIN(Engine)

class CTransform;

END


enum class ECameraMode {
	IDLE,
	FPS,
	LOOKBACK,
	BATTLE,
	ENUM_END
};

class CStaticCamera : public CCamera
{
public:
	explicit CStaticCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CStaticCamera();

public:
	virtual HRESULT			ReadyGameObject();

	virtual _int		UpdateGameObject(const _float& fTimeDelta);
	virtual void		LateUpdateGameObject();

public:
	void KeyInput();

public:
//Moving Function

	//뒤돌아보기 기능을 할 때 사용
	void LookBack();
	
	void ChangeCamEyePos(_vec3 _vDst, _float _fTime = 1.5f);
	void ChangeCamAtPos(_vec3 _vDst, _float _fTime = 1.5f);

public:
//Camera Effect
	void ShakingCamera();
	void ZoomIn();
	void ZoomOut();

private:

	shared_ptr<CTransform> m_pPlrTransCom = nullptr;
	ECameraMode m_eCurrentState = ECameraMode::IDLE;
	_float m_fAngle = 0.f;
	_float m_deltaTime = 0.f;
	
	_float m_fDir = 1.f;
	_float m_fTotalTime = 0.f;
	_float m_fActTime = 0.f;
	_vec3 m_vDstVec;

};
