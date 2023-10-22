#pragma once

#include"UIObj.h"
#include"Item.h"

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

public:
	void InsertItem(shared_ptr<CItem> _pItem) {
		if(m_itemList.size() <= 10)
			m_itemList.push_back(_pItem);
	}

protected:
	void	AddComponent();

private:
	shared_ptr<CTexture> m_pTextureCom;
	list<shared_ptr<CGameObject>> m_itemList;

};

