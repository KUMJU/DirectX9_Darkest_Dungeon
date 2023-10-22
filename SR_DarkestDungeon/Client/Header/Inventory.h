#pragma once

#include"UIObj.h"

BEGIN(Engine)

class CTexture;

END


class CInventory : public CUIObj
{
public:
	explicit CInventory(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CInventory();

public:
	virtual HRESULT ReadyGameObject() override;
	virtual _int UpdateGameObject(const _float& fTimeDelta) override;
	virtual void LateUpdateGameObject() override;
	virtual void RenderGameObject() override;

protected:
	void			AddComponent();

private:
	shared_ptr<CTexture> m_pTextureCom;



};

