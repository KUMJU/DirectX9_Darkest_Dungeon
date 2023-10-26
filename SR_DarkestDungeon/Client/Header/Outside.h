#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CAnimator;

END

class COutside : public CGameObject
{
public:
	COutside(LPDIRECT3DDEVICE9 pGraphicDev, tstring _strKey);
	COutside(const COutside& rhs);
	virtual ~COutside();

public:
	virtual HRESULT			ReadyGameObject();
	virtual _int			UpdateGameObject(const _float& fTimeDelta);
	virtual void			LateUpdateGameObject();
	virtual void			RenderGameObject();

protected:
	virtual void			AddComponent();
	virtual void			SetScale(_vec3 _vScale) { m_vScale = _vScale; }

protected:
	shared_ptr<CRcTex> m_pBufferCom = nullptr;
	shared_ptr<CTexture> m_pTextureCom = nullptr;
	shared_ptr <CTransform> m_pTransformCom = nullptr;
	shared_ptr<CAnimator> m_pAnimatorCom = nullptr;

protected:
	vector<shared_ptr<CGameObject>>		m_vecGameObject = {};
	tstring								m_strKey;
};

