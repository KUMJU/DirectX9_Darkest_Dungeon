#pragma once
#include "GameObject.h" 

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CCollider;

END



class CWall : public CGameObject
{
public:
	CWall(LPDIRECT3DDEVICE9 pGraphicDev, const tstring& _KeyName, _int _iCnt, _bool _bRandom);
	CWall(const CWall& rhs);
	virtual ~CWall();

	shared_ptr<CTransform> Get_TransformCom() { return m_pTransformCom; }

public:
	virtual HRESULT ReadyGameObject() override;
	virtual _int UpdateGameObject(const _float& fTimeDelta) override;
	virtual void LateUpdateGameObject() override;
	virtual void RenderGameObject() override;
	
	virtual void OnCollide(shared_ptr<CGameObject> _pObj) override;

	_bool	IsHorizontal() { return m_bHorizontal; }


private:
	virtual void			AddComponent();

private:
	shared_ptr<CRcTex> m_pBufferCom = nullptr;
	shared_ptr<CTexture> m_pTextureCom = nullptr;

	//shared_ptr<CRcTex> m_pBufferCom2 = nullptr;
	//shared_ptr<CTexture> m_pTextureCom2 = nullptr;

	shared_ptr<CTransform> m_pTransformCom = nullptr;

	shared_ptr<CCollider> m_pColliderCom = nullptr;

	_int		m_iNum = 0;

	_int		m_iCnt = 1;

	_bool		m_bRandom = false;

	_bool		m_bHorizontal = true;

	tstring		m_strKeyName = L"";
};
