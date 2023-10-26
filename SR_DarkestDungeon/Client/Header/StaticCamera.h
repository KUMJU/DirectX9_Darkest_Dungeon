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
//�ܺο��� ȣ���ؼ� ����ϴ� �Լ�

	void SetState(ECameraMode _eCamType) { m_eCurrentState = _eCamType; }
	ECameraMode GetState() { return m_eCurrentState; }
	//���� ������� ī�޶� ������ : ����
	void ChangeCameraWithDegree(ECameraMode _eCamType, _float _fDegree, _float _fTime = 1.5f);
	//������(_vCenter)�� ��� ȣ��(ȣ�� ũ��: _fXSpeed, _fZSpeed)�� �׸��� ī�޶� ������(_vDst) �ű�� : ����
	void ChangeCameraWithPoint(ECameraMode _eCamType, _vec3 _vDst, _vec3 _vCenter, _float _fTime = 1.5f);
	//���� �������� ��� ������ �׸��� ī�޶� ������ �ű�� : ����, �ܾƿ�, ī�޶� ����
	void MovingLineCamera(ECameraMode _eCamType, _vec3 _vDst, _float _fTime = 1.5f);
	//ī�޶� ����Ʈ ����
	void AddCameraEffect(EEffectState _eEffect, _float _fTime, _float _fAmplitude = 0.5f);
private:
//Moving Function

	//ī�޶� ����, ī�޶��� ���� ��(At)�� �����̰� Position(Eye)�� ��ȭ, �⺻������ ���� �׸��� ȸ��
	void ChangeCamEyePos();
	//ī�޶� ����, ī�޶��� Position(Eye)�� �����̰� ���� ��(At)�� ��ȭ
	void ChangeCamAtPos();
	//Point to Point. ������ �׸��� �̵� 
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
	//ȸ���� ����
	_float m_fAngle = 0.f;
	_float m_fTotalAngle = 0.f;
	_float m_fCurrentAngle = 0.f;
	//������ ����
	_vec3 m_vDstVec;
	_vec3 m_vOrigin;

	_matrix m_matOrigin;

//Time 

	//Delta Time ������
	_float m_deltaTime = 0.f;
	//ī�޶� ������ ��ü �ð�
	_float m_fTotalTime = 0.f;
	//Ư�� ������ ��ɹ��� ���� ī�޶� ������ �ð�
	_float m_fActTime = 0.f;
	_vec3 m_vSpeed;
	_vec3 m_vDir;

	_float m_fLerp = 0.f;
	_float m_fDir = 1.f;

private:
	queue<unique_ptr<tagEffectInfo>> m_qEffectQueue;

};
