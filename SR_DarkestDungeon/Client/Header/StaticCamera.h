#pragma once

#include"Camera.h"
#include<queue>

BEGIN(Engine)
class CTransform;
END

enum class ECameraMode {
	IDLE,
	FPS,
	ROTATION,
	ORBIT,
	ZOOMIN,
	ZOOMOUT,
	ENUM_END
};

enum class EEffectState {
	SHAKING,
	FADEIN,
	FADEOUT,
	ENUM_END
};

struct tagEffectInfo {
	EEffectState eEffectType;
	_float		 fAmplitude;
	_float		 fDir;
	_float		 MoveDistance;
	_float		 fTime;
	_float		 fActTime;
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
//외부에서 호출해서 사용하는 함수

	void SetState(ECameraMode _eCamType) { m_eCurrentState = _eCamType; }
	ECameraMode GetState() { return m_eCurrentState; }
	//각도 계산으로 카메라 돌리기 : 자전
	void ChangeCameraWithDegree(ECameraMode _eCamType, _float _fDegree, _float _fTime = 1.5f);
	//기준점(_vCenter)을 잡고 호선(호선 크기: _fXSpeed, _fZSpeed)을 그리며 카메라 포지션(_vDst) 옮기기 : 공전
	void ChangeCameraWithPoint(ECameraMode _eCamType, _vec3 _vDst, _vec3 _vCenter, _float _fTime = 1.5f);
	//최종 포지션을 잡고 직선을 그리며 카메라 포지션 옮기기 : 줌인, 줌아웃, 카메라 무빙
	void MovingLineCamera(ECameraMode _eCamType, _vec3 _vDst, _float _fTime = 1.5f);
	//카메라 이펙트 세팅
	void AddCameraEffect(EEffectState _eEffect, _float _fTime, _float _fAmplitude = 0.5f);
private:
//Moving Function

	//카메라 공전, 카메라의 보는 곳(At)은 고정이고 Position(Eye)가 변화, 기본적으로 원을 그리며 회전
	void ChangeCamEyePos();
	//카메라 자전, 카메라의 Position(Eye)은 고정이고 보는 곳(At)이 변화
	void ChangeCamAtPos();
	//Point to Point. 직선을 그리며 이동 
	void MovingDirect();
	//Camrea Parameter Reset
	void CamReset();

private:
//Camera Effect
	void CameraEffectProcess();
	void ShakingCamera();

private:
	shared_ptr<CTransform> m_pPlrTransCom = nullptr;
	ECameraMode m_eCurrentState = ECameraMode::IDLE;
	
//Moving Range
	//회전할 각도
	_float m_fAngle = 0.f;
	_float m_fTotalAngle = 0.f;
	_float m_fCurrentAngle = 0.f;
	//도착할 지점
	_vec3 m_vDstVec;
	_vec3 m_vOrigin;

	_matrix m_matOrigin;

//Time 

	//Delta Time 보관용
	_float m_deltaTime = 0.f;
	//카메라가 동작할 전체 시간
	_float m_fTotalTime = 0.f;
	//특정 동작을 명령받은 이후 카메라가 움직인 시간
	_float m_fActTime = 0.f;
	_vec3 m_vSpeed;
	_vec3 m_vDir;

	_float m_fLerp = 0.f;
	_float m_fDir = 1.f;

private:
	queue<unique_ptr<tagEffectInfo>> m_qEffectQueue;

};
