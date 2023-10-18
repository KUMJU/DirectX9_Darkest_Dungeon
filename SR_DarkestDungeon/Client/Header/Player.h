//#pragma once
//
//#include"GameObject.h"
//
//BEGIN(Engine)
//
//class CRcTex;
//class CTexture;
//class CTransform;
//
//END
//
//class CPlayer : public CGameObject
//{
//
//public:
//	explicit CPlayer(LPDIRECT3DDEVICE9 pGraphicDev);
//	virtual ~CPlayer();
//
//public:
//	virtual HRESULT ReadyGameObject() override;
//	virtual _int UpdateGameObject(const _float& fTimeDelta) override;
//	virtual void LateUpdateGameObject() override;
//	virtual void RenderGameObject() override;
//
//private:
//	HRESULT			AddComponent();
//	void			KeyInput(const _float& fTimeDelta);
//	void			Climbing_Terrain();
//	_vec3			Picking_OnTerrain();
//
//private:
//	CRcTex* m_pBufferCom = nullptr;
//	CTexture* m_pTextureCom = nullptr;
//	CTransform* m_pTransformCom = nullptr;
//	CCalculator* m_pCalculatorCom = nullptr;
//
//	_float		m_fSpeed = 10.f;
//
//public:
//	static CPlayer* Create(LPDIRECT3DDEVICE9 pGraphicDev);
//
//private:
//	virtual void	Free();
//
//};
//
