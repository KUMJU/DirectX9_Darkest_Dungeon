#pragma once

#include"Engine_Define.h"
#include"StaticCamera.h"

class CCameraMgr
{
	DECLARE_SINGLETON(CCameraMgr)

private:
	explicit CCameraMgr();
	virtual ~CCameraMgr();

public:
	void SetMainCamera(shared_ptr<CStaticCamera> _pCam) { m_pMainCam = _pCam; }

public: //Camera Moving
	
	//�⺻ 1��Ī ȭ������ ��ȯ
	void SetFPSMode();
	//ZoomIn, ZoomOut, Basic Moving
	void MovingStraight(ECameraMode _eType, _vec3 _vPos);
	//����
	void CameraOrbit(ECameraMode _eType, _vec3 _vDst, _vec3 _vCenter);
	//����
	void CameraRotation(ECameraMode _eType, _float _fDegree);

public: //Camera Effect : ���� �۾� ����

	void AddEffectInfo(EEffectState _eEffect, _float _fTime, _float _fAmplitude = 0.1f);

private:

	shared_ptr<CStaticCamera> m_pMainCam = nullptr;


};

