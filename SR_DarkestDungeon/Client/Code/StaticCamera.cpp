#include"pch.h"
#include "StaticCamera.h"
#include"Export_Utility.h"

CStaticCamera::CStaticCamera(LPDIRECT3DDEVICE9 pGraphicDev)
    :CCamera(pGraphicDev)
{
}

CStaticCamera::~CStaticCamera()
{
}

HRESULT CStaticCamera::ReadyGameObject()
{
	m_pPlrTransCom = dynamic_pointer_cast<CTransform>(Engine::Get_Component(L"Layer_4_GameObj", L"Obj_Player", L"Com_Transform", COMPONENTID::ID_DYNAMIC));

	HRESULT result = CCamera::ReadyGameObject();

	m_eCurrentState = ECameraMode::FPS;
	m_matView = *(m_pPlrTransCom->GetWorld());
	D3DXMatrixInverse(&m_matView,0, &m_matView);

	return result;
}

_int CStaticCamera::UpdateGameObject(const _float& fTimeDelta)
{

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
	m_deltaTime = fTimeDelta;
	return 0;
}

void CStaticCamera::LateUpdateGameObject(){
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);


	m_vUp = _vec3(0.f, 1.f, 0.f);
	_vec3 vLook, vPos;
	_vec3 test1, test2;

	_matrix matYpos;

	switch (m_eCurrentState)
	{
	case ECameraMode::IDLE:

		break;
	case ECameraMode::FPS:

		m_matView = *(m_pPlrTransCom->GetWorld());
		m_matView.m[3][1] += 3.f;
		D3DXMatrixInverse(&m_matView, 0, &m_matView);

		break;

	case ECameraMode::ROTATION:
		ChangeCamEyePos();
		break;

	case ECameraMode::ORBIT:
		ChangeCamAtPos();
		break;

	case ECameraMode::ZOOMIN:
		MovingDirect();
		break;

	case ECameraMode::ZOOMOUT:
		MovingDirect();
		break;

	case ECameraMode::ENUM_END:

		break;

	default:
		break;
	}

	if (!m_qEffectQueue.empty())
		CameraEffectProcess();



}


void CStaticCamera::ChangeCameraWithDegree(ECameraMode _eCamType, _float _fDegree, _float _fTime)
{
	CamReset();

	if (_eCamType == ECameraMode::ROTATION) {
		m_eCurrentState = _eCamType;
		m_fAngle = _fDegree / _fTime;
		m_fTotalTime = _fTime;
		m_fTotalAngle = _fDegree;
	}
}

void CStaticCamera::ChangeCameraWithPoint(ECameraMode _eCamType, _vec3 _vDst, _vec3 _vCenter, _float _fTime)
{

	if (_eCamType == ECameraMode::ROTATION)
		return;

	CamReset();

	m_eCurrentState = _eCamType;
	m_vDstVec = _vDst;
	m_fTotalTime = _fTime;


	_vec3 vCurrentPos;
	_matrix matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, 0 , &matView);

	memcpy(&vCurrentPos, &matView.m[3][0], sizeof(_vec3));
	
	_vCenter.y = vCurrentPos.y;
	_vDst.y = vCurrentPos.y;

	_float fXLen = fabsf(vCurrentPos.x - _vDst.x);
	_float fZLen = fabsf(vCurrentPos.z - _vDst.z);

	m_vSpeed.x = fXLen / _fTime;
	m_vSpeed.z = fZLen / _fTime;

	_vec3 vDirVec1 = _vCenter - _vDst;
	_vec3 vDirVec2 = _vCenter - vCurrentPos;

	D3DXVec3Normalize(&vDirVec1, &vDirVec1);
	D3DXVec3Normalize(&vDirVec2, &vDirVec2);

	_float fAngle = D3DXVec3Dot(&vDirVec1, &vDirVec2);
	m_fAngle = acosf(fAngle);

	m_fTotalAngle = m_fAngle;
	_float fDegree = D3DXToDegree(m_fAngle);
	m_vOrigin = vCurrentPos;

	m_matOrigin = matView;

	m_fAngle = m_fAngle / _fTime;

}

void CStaticCamera::MovingLineCamera(ECameraMode _eCamType, _vec3 _vDst, _float _fTime)
{
	m_eCurrentState = _eCamType;

	CamReset();

	_vec3 vCurrentPos;

	_matrix matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, 0, &matView);
	memcpy(&vCurrentPos, &matView.m[3][0], sizeof(_vec3));

	m_fTotalTime = _fTime;
	_vec3 vDir = _vDst - vCurrentPos;
	m_vSpeed.x =  D3DXVec3Length(&vDir);
	m_vSpeed.x /= _fTime;

	D3DXVec3Normalize(&vDir, &vDir);

	m_vDir = vDir;
	m_vDir.y = 0.f;
	m_vDstVec = _vDst;


}


void CStaticCamera::ChangeCamEyePos()
{
	D3DXQUATERNION q;
	_matrix matYpos, matView;
	_vec3 vUpvec = { 0.f, 1.f , 0.f };

	m_fActTime += m_deltaTime;

	_float fCalAngle = D3DXToRadian(m_fAngle) * m_deltaTime;

	if (m_fActTime + m_deltaTime >= m_fTotalTime) {
		fCalAngle = D3DXToRadian(m_fTotalAngle) - m_fCurrentAngle;
		return;
	}

	if (m_fActTime >= m_fTotalTime) {
		m_eCurrentState = ECameraMode::IDLE;
		return;
	}

	D3DXQuaternionRotationAxis(&q, &vUpvec, fCalAngle);

	//쿼터니언을 행렬로 변환
	D3DXMatrixRotationQuaternion(&matYpos, &q);

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, 0, &matView);

	m_matView = matYpos * matView;
	D3DXMatrixInverse(&m_matView, 0, &m_matView);

	m_fCurrentAngle += D3DXToRadian(m_fAngle) * m_deltaTime;

	if (m_fActTime >= m_fTotalTime) {
		m_eCurrentState = ECameraMode::IDLE;
		return;
	}

}

void CStaticCamera::ChangeCamAtPos()
{
	D3DXQUATERNION q,qRot; 
	_matrix matYpos, matRot, matTrans, matRot2, matView , matParent , matWorld;
	_vec3 vUpvec = { 0.f, 1.f , 0.f };
	_vec3 vPos;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, 0, &matView);

	//D3DXQuaternionSlerp

	D3DXMatrixIdentity(&matWorld);
	//특정 vector 축으로 회전하는 쿼터니언 생성
	D3DXQuaternionRotationAxis(&q, &vUpvec, -1.f * m_fAngle * m_deltaTime);

	memcpy(&vPos, &matView.m[3][0], sizeof(_vec3));
	

	//쿼터니언을 행렬로 변환
	D3DXMatrixRotationQuaternion(&matYpos, &q);
	D3DXMatrixTranslation(&matTrans, m_vSpeed.x * m_deltaTime, 0.f, m_vSpeed.z * 0.2f* m_deltaTime);
	D3DXMatrixTranslation(&matParent, m_vDstVec.x * m_deltaTime, 0.f , m_vDstVec.z * m_deltaTime);

	//스 * 자 * 이 * 공 * 부
	matView *= matTrans * matYpos;
	m_matView = matView;
	D3DXMatrixInverse(&m_matView, 0, &m_matView);

	m_fActTime += m_deltaTime;
	m_fCurrentAngle += m_fAngle * m_deltaTime;

	if (m_fTotalTime <= m_fActTime) {
		m_eCurrentState = ECameraMode::IDLE;
	}

}

void CStaticCamera::MovingDirect()
{
	_vec3 vCurrentPos, vCurPos2;
	_matrix matView;

	m_fActTime += m_deltaTime;

	if (m_fTotalTime <= m_fActTime) {
		m_eCurrentState = ECameraMode::IDLE;
	}

	
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, 0, &matView);
	memcpy(&vCurrentPos, &matView.m[3][0], sizeof(_vec3));
	vCurPos2 = vCurrentPos;

	m_fLerp += (_float)0.3 * m_deltaTime;

	if (m_fLerp > 0.8f) 
		m_fLerp = 0.8f;

	D3DXVec3Lerp(&vCurrentPos, &vCurrentPos, &m_vDstVec, m_fLerp);

	memcpy(&matView.m[3][0] , &vCurrentPos , sizeof(_vec3));
	D3DXMatrixInverse(&matView, 0, &matView);

	m_matView = matView;


}

void CStaticCamera::CamReset()
{
	m_fLerp = 0.f;
	m_fAngle = 0.f;
	m_fTotalAngle = 0.f;
	m_fCurrentAngle = 0.f;
	m_fTotalTime = 0.f;
	m_fActTime = 0.f;
	m_fDir = 1.f;
}

void CStaticCamera::AddCameraEffect(EEffectState _eEffect, _float _fTime, _float _fAmplitude)
{

	unique_ptr<tagEffectInfo> pEffectInfo = make_unique<tagEffectInfo>();
	
	pEffectInfo->eEffectType = _eEffect;
	pEffectInfo->fTime = _fTime;
	pEffectInfo->fAmplitude = _fAmplitude;
	pEffectInfo->MoveDistance = 0.f;
	pEffectInfo->fActTime = 0.f;
	pEffectInfo->fDir = 1.f;

	m_qEffectQueue.push(std::move(pEffectInfo));

}

void CStaticCamera::CameraEffectProcess()
{
	switch (m_qEffectQueue.front()->eEffectType)
	{
	case EEffectState::SHAKING:
		ShakingCamera();
		break;
	case EEffectState::FADEIN:
		break;
	case EEffectState::FADEOUT:
		break;
	case EEffectState::ENUM_END:
		break;
	default:
		break;
	}
	

}

//방향 Vertical 고정
void CStaticCamera::ShakingCamera()
{
	_vec3 vCurrentPos;
	_matrix matView;

	_float fDir = m_qEffectQueue.front()->fDir;

	//이동거리 구하기
	_float fDistance = fDir * 10.f * m_deltaTime;

	//한 방향으로 진폭만큼 이동했을시 방향 전환
	if (fabsf(m_qEffectQueue.front()->MoveDistance + fDistance) >= m_qEffectQueue.front()->fAmplitude) {
		fDistance = m_qEffectQueue.front()->fAmplitude * fDir - m_qEffectQueue.front()->MoveDistance;
		m_qEffectQueue.front()->MoveDistance = m_qEffectQueue.front()->fAmplitude * fDir;
		m_qEffectQueue.front()->fDir *= -1.f;
	}
	else {
		m_qEffectQueue.front()->MoveDistance += fDistance;
	}

	//현재 카메라 position에 더해주기
	memcpy(&vCurrentPos, &m_matView.m[3][0], sizeof(_vec3));

	vCurrentPos.y += fDistance;

	memcpy(&m_matView.m[3][0], &vCurrentPos, sizeof(_vec3));

	//지속시간 측정 후, 지정한 시간에 도달하면 pop을 해 없애준다
	m_qEffectQueue.front()->fActTime += m_deltaTime;
	
	if (m_qEffectQueue.front()->fActTime >= m_qEffectQueue.front()->fTime)
		m_qEffectQueue.pop();
	

}
