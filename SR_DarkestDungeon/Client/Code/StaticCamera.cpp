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

	//_vec3 vtempVec = { 0.f, 10.f, 0.f };

	//m_pPlrTransCom->GetInfo(INFO::INFO_POS, &m_vEye);
	//m_vUp = m_vEye + vtempVec;
	m_vAt = _vec3(0.f, 0.f, 1.f);


	m_fFov = D3DXToRadian(60.f);
	m_fAspect = (_float)WINCX / WINCY;
	m_fNear = 0.1f;
	m_fFar = 1000.f;

	FAILED_CHECK_RETURN(CCamera::ReadyGameObject(), E_FAIL);

	return S_OK;
}

_int CStaticCamera::UpdateGameObject(const _float& fTimeDelta)
{
	int iExit(0);
	iExit = __super::UpdateGameObject(fTimeDelta);

	return iExit;
}

void CStaticCamera::LateUpdateGameObject()
{
	_vec3 vtempVec;
	_vec3 vTemp2 = { 0.f,0.f,10.f };

	m_pPlrTransCom->GetInfo(INFO::INFO_POS, &m_vEye);
	m_pPlrTransCom->GetInfo(INFO::INFO_LOOK, &vtempVec);

	D3DXVec3Normalize(&vtempVec, &vtempVec);
	m_vAt = m_vEye + vtempVec * 10.f;
	m_vUp = _vec3(0.f, 1.f, 0.f);
}