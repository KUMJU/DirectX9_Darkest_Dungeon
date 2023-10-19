#pragma once

#include"Camera.h"

BEGIN(Engine)

class CTransform;

END

class CStaticCamera : public CCamera
{
public:
	explicit CStaticCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CStaticCamera();

public:
	virtual HRESULT			ReadyGameObject();

	virtual _int		UpdateGameObject(const _float& fTimeDelta);
	virtual void		LateUpdateGameObject();

private:

	shared_ptr<CTransform> m_pPlrTransCom = nullptr;

};
