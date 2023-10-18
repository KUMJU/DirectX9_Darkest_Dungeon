#include "Camera.h"

CCamera::CCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CCamera::~CCamera()
{
}

HRESULT CCamera::ReadyGameObject()
{
	CGameObject::ReadyGameObject();

	D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vAt, &m_vUp);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);

	if (EProjectionType::PERSPECTIVE == m_eCurrentType) {
		D3DXMatrixPerspectiveFovLH(&m_matProj, m_fFov, m_fAspect, m_fNear, m_fFar);
	}
	else {
		//UI Size ... etc
		//D3DXMatrixOrthoLH(&m_matProj)
	}


	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

	return S_OK;
}

_int CCamera::UpdateGameObject(const _float& fTimeDelta)
{
	CGameObject::UpdateGameObject(fTimeDelta);

	D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vAt, &m_vUp);

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);

	return 0;
}

void CCamera::LateUpdateGameObject()
{
	CGameObject::LateUpdateGameObject();

}

void CCamera::Free()
{
}
