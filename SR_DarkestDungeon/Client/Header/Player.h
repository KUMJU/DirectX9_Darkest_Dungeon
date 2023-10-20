#pragma once

#include"GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CAnimator;

END


class CPlayer : public CGameObject
{

public:
	explicit CPlayer(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPlayer();

public:
	virtual HRESULT ReadyGameObject() override;
	virtual _int UpdateGameObject(const _float& fTimeDelta) override;
	virtual void LateUpdateGameObject() override;
	virtual void RenderGameObject() override;

private:
	void			AddComponent();
	void			KeyInput(const _float& fTimeDelta);
	void			ClimbingTerrain();

private:
	// Player Component
	shared_ptr<CTransform> m_pTransformCom;
	shared_ptr<CAnimator> m_pTextureCom;
	shared_ptr<CRcTex> m_pBufCom;

private:
	_float		m_fSpeed = 5.f;

private:
	virtual void	Free();

};