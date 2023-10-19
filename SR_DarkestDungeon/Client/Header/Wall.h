#pragma once
#include "GameObject.h" 

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;

END



class CWall : public CGameObject
{
public:
	CWall(LPDIRECT3DDEVICE9 pGraphicDev);
	CWall(const CWall& rhs);
	virtual ~CWall();

	shared_ptr<CTransform> Get_TransformCom() { return m_pTransformCom; }

public:
	virtual HRESULT ReadyGameObject() override;
	virtual _int UpdateGameObject(const _float& fTimeDelta) override;
	virtual void LateUpdateGameObject() override;
	virtual void RenderGameObject() override;

private:
	virtual void			AddComponent();

private:
	shared_ptr<CRcTex> m_pBufferCom = nullptr;
	shared_ptr<CTexture> m_pTextureCom = nullptr;

	shared_ptr<CRcTex> m_pBufferCom2 = nullptr;
	shared_ptr<CTexture> m_pTextureCom2 = nullptr;

	shared_ptr<CTransform> m_pTransformCom = nullptr;

	_int		m_iNum = 0;
};
