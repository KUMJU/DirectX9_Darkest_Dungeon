#pragma once
#include"Engine_Define.h"

BEGIN(Engine)


class ENGINE_DLL CLight
{
public:
	explicit CLight(LPDIRECT3DDEVICE9 _pGraphicDev);
	~CLight();

public:
	HRESULT			ReadyLight(const D3DLIGHT9* _pLightInfo, const _uint& _iIndex);
	void			SetPosition(_vec3 _vPos);
	_uint			GetLightNum() { return m_iIndex; }

public:

	void			LightOn();
	void			LightOff();

private:
	LPDIRECT3DDEVICE9			m_pGraphicDev;
	D3DLIGHT9					m_tLightInfo;
	_uint						m_iIndex;

private:
	void Free();

};

END