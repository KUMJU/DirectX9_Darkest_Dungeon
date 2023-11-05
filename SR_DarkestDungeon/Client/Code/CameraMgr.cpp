#include"pch.h"
#include "CameraMgr.h"

IMPLEMENT_SINGLETON(CCameraMgr)

CCameraMgr::CCameraMgr()
{
}

CCameraMgr::~CCameraMgr()
{
}

void CCameraMgr::SetFPSMode()
{
	m_pMainCam->SetState(ECameraMode::FPS);
}

void CCameraMgr::SetVillageMode()
{
	m_pMainCam->SetState(ECameraMode::VILLAGE);
}

void CCameraMgr::SetState(ECameraMode _eState)
{
	m_pMainCam->SetState(_eState);
}

void CCameraMgr::MovingStraight(ECameraMode _eType, _vec3 _vPos)
{
	m_pMainCam->MovingLineCamera(_eType, _vPos);

}

void CCameraMgr::CameraOrbit(ECameraMode _eType, _vec3 _vDst, _vec3 _vCenter)
{
	m_pMainCam->ChangeCameraWithPoint(_eType, _vDst, _vCenter);
}

void CCameraMgr::CameraRotation(ECameraMode _eType, _float _fDegree)
{
	m_pMainCam->ChangeCameraWithDegree(_eType, _fDegree);

}

void CCameraMgr::MovingRightVec(_int _iDir)
{
	m_pMainCam->MovingRightVec(_iDir);
}

void CCameraMgr::AddEffectInfo(EEffectState _eEffect, _float _fTime, _float _fAmplitude)
{
	m_pMainCam->AddCameraEffect(_eEffect, _fTime, _fAmplitude);
}
