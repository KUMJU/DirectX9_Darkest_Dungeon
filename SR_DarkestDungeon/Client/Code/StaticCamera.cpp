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

	_vec3 vLook , vPos;
	m_pPlrTransCom->GetInfo(INFO::INFO_LOOK, &vLook);
	m_pPlrTransCom->GetInfo(INFO::INFO_LOOK, &vPos);
	m_vAt = vPos + vLook * 50.f ;

	return S_OK;
}

_int CStaticCamera::UpdateGameObject(const _float& fTimeDelta)
{
	int iExit(0);
	iExit = __super::UpdateGameObject(fTimeDelta);

	m_deltaTime = fTimeDelta;
	return iExit;
}

void CStaticCamera::LateUpdateGameObject()
{

	m_vUp = _vec3(0.f, 1.f, 0.f);
	_vec3 vLook, vPos;

	switch (m_eCurrentState)
	{
	case ECameraMode::IDLE:

		m_pPlrTransCom->GetInfo(INFO::INFO_POS, &vPos);
		m_pPlrTransCom->GetInfo(INFO::INFO_LOOK, &vLook);

		m_vEye = vPos + _vec3(0.f, 2.f, 0.f);
		m_vAt = vPos + vLook * 50.f;

		break;
	case ECameraMode::LOOKBACK:

	//	m_pPlrTransCom->GetInfo(INFO::INFO_POS, &m_vEye);
		//m_vEye += _vec3(0.f, 2.f, 0.f);
		m_vEye += _vec3(m_deltaTime, 0.f, 0.f);

		LookBack();

		break;
	case ECameraMode::BATTLE:

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
	if (GetAsyncKeyState('Z') & 0x8000) {

		if (m_eCurrentState != ECameraMode::LOOKBACK) {
			m_eCurrentState = ECameraMode::LOOKBACK;
		}
	}

	if (GetAsyncKeyState('A') & 0x8000) {

		if (m_eCurrentState != ECameraMode::LOOKBACK) {
			m_eCurrentState = ECameraMode::LOOKBACK;
		}
	}

}

void CStaticCamera::LookBack()
{
	_matrix matYpos;

	D3DXQUATERNION qRot;
	D3DXQUATERNION qRot2;

	D3DXQuaternionIdentity(&qRot2);
	D3DXQuaternionRotationAxis(&qRot, &m_vUp, D3DXToRadian(2.f));
	D3DXMatrixRotationQuaternion(&matYpos, &qRot);

	D3DXQuaternionSlerp(&qRot, &qRot2, &qRot, 0.f);
	D3DXVec3TransformCoord(&m_vAt, &m_vAt, &matYpos);
	m_fAngle += 2.f;

	if (m_fAngle >= 180.f) {
		m_eCurrentState = ECameraMode::IDLE;
		m_fAngle = 0.f;
	}
}

void CStaticCamera::ChangeCamEyePos(_vec3 _vDst, _float _fTime)
{
	_vec3 vTemp1 = m_vAt - m_vEye;
	_vec3 vTemp2 = m_vAt - _vDst;

	_float fVecDot = D3DXVec3Dot(&vTemp1, &vTemp2);
	
	acosf(fVecDot);

	//m_vAt

	//_vec3 

	//m_deltaTime

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
