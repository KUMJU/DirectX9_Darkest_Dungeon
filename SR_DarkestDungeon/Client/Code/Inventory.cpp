#include"pch.h"
#include "Inventory.h"
#include"Item.h"
#include"Export_Utility.h"

CInventory::CInventory(LPDIRECT3DDEVICE9 _pGraphicDev)
    :Engine::CUIObj(_pGraphicDev)
{
}

CInventory::~CInventory()
{
}

HRESULT CInventory::ReadyGameObject()
{
    m_vSize = { 209.f , 46.f ,1.f };
    m_bVisible = true;
    m_bEnable = true;
    m_bActive = true;
    
    m_pTransCom->SetScale(m_vSize.x, m_vSize.y, m_vSize.z);
    m_pTransCom->SetPosition(0.f, -320.f, 1.f);
    m_pTransCom->SetAngle({0.f, 0.f, 0.f});

    return S_OK;
}

_int CInventory::UpdateGameObject(const _float& fTimeDelta)
{
    int iExit(0);
    CGameObject::UpdateGameObject(fTimeDelta);
    AddRenderGroup(RENDER_UI, shared_from_this());

    for (auto& iter : m_itemList) {
        iter->UpdateGameObject(fTimeDelta);
    }

    return iExit;
}

void CInventory::LateUpdateGameObject()
{

    for (auto& iter : m_itemList) {
        iter->LateUpdateGameObject();
    }

    __super::LateUpdateGameObject();
}

void CInventory::RenderGameObject()
{
    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->GetWorld());
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
    m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, FALSE);

    m_pTextureCom->SetTexture(0);
    m_pRcTexCom->RenderBuffer();


    for (auto& iter : m_itemList) {
        iter->RenderGameObject();
    }

    m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
    m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

}

void CInventory::AddComponent()
{
    shared_ptr<CComponent> pComponent;

    CUIObj::AddComponent();

    pComponent = m_pTextureCom = make_shared<CTexture>(m_pGraphicDev);
    m_pTextureCom->SetTextureKey(L"UI_Inventory", TEX_NORMAL);
    m_mapComponent[ID_STATIC].insert({ L"Com_Texture",pComponent });

}
