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
	
	//기본 1인칭 화면으로 전환
	void SetFPSMode();
	//ZoomIn, ZoomOut, Basic Moving
	void MovingStraight(ECameraMode _eType, _vec3 _vPos);
	//공전
	void CameraOrbit(ECameraMode _eType, _vec3 _vDst, _vec3 _vCenter);
	//자전
	void CameraRotation(ECameraMode _eType, _float _fDegree);

public: //Camera Effect : 아직 작업 안함

	void AddEffectInfo(EEffectState _eEffect, _float _fTime, _float _fAmplitude = 0.1f);

private:

	shared_ptr<CStaticCamera> m_pMainCam = nullptr;


};

