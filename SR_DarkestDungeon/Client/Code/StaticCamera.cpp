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
	m_pPlrTransCom = dynamic_pointer_cast<CTransform>(Engine::Get_Component(L"Layer_GameObj", L"Obj_Player", L"Com_Transform", COMPONENTID::ID_DYNAMIC));

	FAILED_CHECK_RETURN(CCamera::ReadyGameObject(), E_FAIL);

	m_eCurrentState = ECameraMode::FPS;
	m_matView = *(m_pPlrTransCom->GetWorld());
	D3DXMatrixInverse(&m_matView,0, &m_matView);

	return S_OK;
}

_int CStaticCamera::UpdateGameObject(const _float& fTimeDelta)
{

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
	m_deltaTime = fTimeDelta;
	return 0;
}

void CStaticCamera::LateUpdateGameObject()
{

	m_vUp = _vec3(0.f, 1.f, 0.f);
	_vec3 vLook, vPos;
	_vec3 test1, test2;

	_matrix matYpos;

	D3DXQUATERNION qRot;
	D3DXQUATERNION qRot2;
	int a;
	switch (m_eCurrentState)
	{
	case ECameraMode::IDLE:

		m_vAt;
		m_vEye;
		a = 5;

		break;

	case ECameraMode::FPS:

		m_matView = *(m_pPlrTransCom->GetWorld());
		m_matView.m[3][1] += 3.f;
		D3DXMatrixInverse(&m_matView, 0, &m_matView);

		break;
	case ECameraMode::LOOKBACK:

		LookBack();

		break;
	case ECameraMode::BATTLE:

		//if (m_fActTime >= m_fTotalTime) {
		//	m_vEye = m_vDstVec;
		//	m_eCurrentState = ECameraMode::IDLE;
		//}
		//D3DXQuaternionIdentity(&qRot2);
		//D3DXQuaternionRotationAxis(&qRot, &m_vUp, m_fAngle * m_deltaTime);
		//D3DXMatrixRotationQuaternion(&matYpos, &qRot);

		//D3DXQuaternionSlerp(&qRot, &qRot2, &qRot, 0.f);
		//D3DXVec3TransformCoord(&m_vEye, &m_vEye, &matYpos);
		//D3DXVec3TransformCoord(&m_vAt, &m_vAt, &matYpos);


		//D3DXVec3Normalize(&test1, &m_vAt);
		//D3DXVec3Normalize(&test2, &m_vEye);


		//m_vAt += test1 * 2.f * m_deltaTime;
		//m_vEye += test2 * 2.f * m_deltaTime;

		//m_fActTime += m_deltaTime;



		D3DXQuaternionIdentity(&qRot2);
		D3DXQuaternionRotationAxis(&qRot, &m_vUp, D3DXToRadian(-2.f));
		D3DXMatrixRotationQuaternion(&matYpos, &qRot);

		D3DXQuaternionSlerp(&qRot, &qRot2, &qRot, 0.f);
		D3DXVec3TransformNormal(&m_vEye, &m_vEye, &matYpos);

		

		m_fAngle += 2.f;

		//돌렸던 고개를 원상복구
		if (m_fAngle >= 90.f) {
			m_eCurrentState = ECameraMode::IDLE;
			m_fAngle = 0.f;
		}

		break;
	case ECameraMode::ENUM_END:

		break;
	default:
		break;
	}


	KeyInput();

}

void CStaticCamera::KeyInput()
{
	if (GetAsyncKeyState('O') & 0x8000) {

		if (m_eCurrentState != ECameraMode::LOOKBACK) {
			m_eCurrentState = ECameraMode::LOOKBACK;
		}
	}

	if (GetAsyncKeyState('P') & 0x8000) {

		if (m_eCurrentState != ECameraMode::BATTLE) {
			_vec3 vNewEyePos = m_vAt + _vec3{ 4.f, 0.f, 0.f };
		//	ChangeCamEyePos(vNewEyePos);
			m_eCurrentState = ECameraMode::BATTLE;
		}
	}

}

void CStaticCamera::LookBack()
{
	_matrix matYpos;

	D3DXQUATERNION qRot;
	D3DXQUATERNION qRot2;

	D3DXQuaternionIdentity(&qRot2);
	D3DXQuaternionSlerp(&qRot, &qRot2, &qRot, 0.f);

	D3DXQuaternionRotationAxis(&qRot, &m_vUp, D3DXToRadian(2.f * m_fDir));
	D3DXMatrixRotationQuaternion(&matYpos, &qRot);

	D3DXVec3TransformNormal(&m_vAt, &m_vAt, &matYpos);

	m_fAngle += 2.f * m_fDir;
	
	//돌렸던 고개를 원상복구
	if (m_fDir < 0.f) {
		if (m_fAngle <= 0.f) {
			m_eCurrentState = ECameraMode::FPS;
			m_fAngle = 0.f;
			m_fDir *= -1.f;
		}
	}
	//뒤로 고개를 돌릴 때 
	else {
		if (m_fAngle >= 180.f) {
			m_eCurrentState = ECameraMode::IDLE;
			m_fDir *= -1.f;
		}
	}
}

void CStaticCamera::ChangeCamEyePos(_vec3 _vDst, _float _fTime)
{

	_vec3 vTemp1 = m_vAt - m_vEye;
	_vec3 vTemp2 = m_vAt - _vDst;

	vTemp2.y = 0.f;
	D3DXVec3Normalize(&vTemp1, &vTemp1);
	D3DXVec3Normalize(&vTemp2, &vTemp2);

	_float fVecDot = D3DXVec3Dot(&vTemp1, &vTemp2);
	
	m_fTotalTime = _fTime;
	m_fAngle = acosf(fVecDot) / _fTime;

	m_vDstVec = _vDst;
	m_fActTime = 0.f;
}

void CStaticCamera::ChangeCamAtPos(_vec3 _vDst, _float _fTime)
{
}

void CStaticCamera::ShakingCamera()
{
}

void CStaticCamera::ZoomIn()
{
}

void CStaticCamera::ZoomOut()
{
}
