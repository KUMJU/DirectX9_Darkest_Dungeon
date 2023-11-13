#include"LightMgr.h"

IMPLEMENT_SINGLETON(CLightMgr)

CLightMgr::CLightMgr()
{
}

CLightMgr::~CLightMgr()
{
}

HRESULT CLightMgr::ReadyLight(LPDIRECT3DDEVICE9 _pGraphicDev, const D3DLIGHT9* _pLightInfo, const _uint& iIndex)
{
	shared_ptr<CLight> pLight = make_shared<CLight>(_pGraphicDev);

	if (nullptr != pLight) {
		pLight->ReadyLight(_pLightInfo, iIndex);
		m_LightList.push_back(pLight);
	}

	return S_OK;
}

void CLightMgr::RemoveList()
{
	m_LightList.clear();
}

void CLightMgr::AddList(shared_ptr<CLight> _pLight)
{
	m_LightList.push_back(_pLight);
}

shared_ptr<CLight> CLightMgr::InitPointLight(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	D3DLIGHT9 pLightInfo;

	ZeroMemory(&pLightInfo, sizeof(D3DLIGHT9));

	pLightInfo.Type = D3DLIGHT_POINT;
	pLightInfo.Range = 30.f;
	
	pLightInfo.Diffuse.r = 1.f;
	pLightInfo.Diffuse.g = 1.f;
	pLightInfo.Diffuse.b = 1.f;
	pLightInfo.Ambient.r = 1.f;
	pLightInfo.Ambient.g = 1.f;
	pLightInfo.Ambient.b = 1.f;
	pLightInfo.Specular.r = 1.0f;
	pLightInfo.Specular.g = 1.f;
	pLightInfo.Specular.b = 1.f;

	pLightInfo.Position.x = 20.f;
	pLightInfo.Position.y = 3.f;
	pLightInfo.Position.z = 10.f;

	//pLightInfo.Attenuation0 = 0.f;
	//pLightInfo.Attenuation1 = 5.f;
	//pLightInfo.Attenuation2 = 0.f;


	shared_ptr<CLight> pLight = make_shared<CLight>(_pGraphicDev);
	pLight->ReadyLight(&pLightInfo, (_uint)m_LightList.size());

	m_LightList.push_back(pLight);

	return pLight;
}

shared_ptr<CLight> CLightMgr::InitDirectionLight(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	return shared_ptr<CLight>();
}
