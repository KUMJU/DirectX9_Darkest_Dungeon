#include"pch.h"
#include "PlayerFPSUI.h"
#include"Export_Utility.h"

CPlayerFPSUI::CPlayerFPSUI(LPDIRECT3DDEVICE9 _GraphicDev)
	:CUIObj(_GraphicDev)
{
}

CPlayerFPSUI::~CPlayerFPSUI()
{
}

HRESULT CPlayerFPSUI::ReadyGameObject()
{
    m_vSize = { 175.f , 140.f ,0.f };
    m_vAngle = { 0.f, 0.f, 0.f };
    m_vPos = { 640.f - m_vSize.x, -220.f, 0.5f };

    m_bVisible = false;
    m_bEnable = true;
    m_bActive = true;

    m_pTransCom->SetScale(m_vSize.x, m_vSize.y, m_vSize.z);
    m_pTransCom->SetPosition(m_vPos.x, m_vPos.y, m_vPos.z);
    m_pTransCom->SetAngle({ 0.f, 0.f, 0.f });

    CUIObj::ReadyGameObject();

	return S_OK;
}

_int CPlayerFPSUI::UpdateGameObject(const _float& fTimeDelta)
{

	_int iExit(0);
	iExit = CGameObject::UpdateGameObject(fTimeDelta);

    AddRenderGroup(RENDERID::RENDER_UI, shared_from_this());

	return iExit;
}

void CPlayerFPSUI::LateUpdateGameObject()
{
}

void CPlayerFPSUI::RenderGameObject()
{
    if (!m_bVisible)
        return;


    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->GetWorld());
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
    m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, FALSE);

    m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0xc0);
    m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

    m_pTextureCom->SetTexture(0);
    m_pRcTexCom->RenderBuffer();

    m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

    m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
    m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CPlayerFPSUI::AddComponent()
{
	shared_ptr<CComponent> pComponent;

	CUIObj::AddComponent();

	pComponent = m_pTextureCom = make_shared<CTexture>(m_pGraphicDev);
	m_pTextureCom->SetTextureKey(L"Player_HUD_bottom_left", TEX_NORMAL);
	m_mapComponent[ID_STATIC].insert({ L"Com_Texture",pComponent });


}
