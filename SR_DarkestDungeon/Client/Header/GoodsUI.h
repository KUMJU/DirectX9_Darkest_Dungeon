#pragma once

#include"UIObj.h"

BEGIN(Engine)

class CTexture;

END

class CGoodsUI : public CUIObj
{
public:
	explicit CGoodsUI(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CGoodsUI();

public:
	virtual HRESULT ReadyGameObject() override;
	virtual _int UpdateGameObject(const _float& fTimeDelta) override;
	virtual void LateUpdateGameObject() override;
	virtual void RenderGameObject() override;

protected:
	void			AddComponent();

public:

	void SetHeirloomNum(_int _iNum) { m_iHeirloom = _iNum; }
	void SetGoldNum(_int _iNum) { m_iGold = _iNum; }

private:
	shared_ptr<CTexture> m_pTextureCom = nullptr;

	_int m_iGold = 0;
	_int m_iHeirloom = 0;

};

