#pragma once
#include "Inside.h"
class CTervarn : public CInside
{
public:
	explicit CTervarn(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTervarn(const CTervarn& rhs);
	virtual ~CTervarn();

public:
	virtual HRESULT			ReadyGameObject();
	virtual _int			UpdateGameObject(const _float& fTimeDelta);
	virtual void			LateUpdateGameObject();
	virtual void			RenderGameObject();

protected:
	vector<shared_ptr<CGameObject>>		m_vecGameObject = {};
};

