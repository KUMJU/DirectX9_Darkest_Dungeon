#include "pch.h"
#include "Description.h"

#include"Export_Utility.h"
#include"Export_System.h"

CDescription::CDescription(LPDIRECT3DDEVICE9 _pGraphicDev)
	: Engine::CUIObj(_pGraphicDev)
{
}

CDescription::~CDescription()
{
}

HRESULT CDescription::ReadyGameObject()
{
	m_vSize = { 100.f, 30.f, 0.f };
	m_vAngle = { 0.f, 0.f, 0.f };
	m_vPos = { 0.f, 0.f, 0.f };

	m_bVisible = false;
	m_bEnable = true;
	m_bActive = true;

	m_pTransCom->SetScale(m_vSize.x, m_vSize.y, m_vSize.z);
	m_pTransCom->SetPosition(m_vPos.x, m_vPos.y, m_vPos.z);
	m_pTransCom->SetAngle({ 0.f, 0.f, 0.f });

	CUIObj::ReadyGameObject();

	return S_OK;
}

_int CDescription::UpdateGameObject(const _float& fTimeDelta)
{
	if (!m_bVisible)
		return 0;

	int iExit(0);
	CGameObject::UpdateGameObject(fTimeDelta);
	AddRenderGroup(RENDER_UI, shared_from_this());

	return iExit;
}

void CDescription::LateUpdateGameObject()
{
	if (!m_bVisible)
		return;
}

void CDescription::RenderGameObject()
{
}

void CDescription::SetDescription(EDescriptionType _eDescription, tstring _content1, tstring _title, tstring _strColorTitle1, D3DCOLOR _tColor1, tstring _strColorContent1, tstring _strColorTitle2, D3DCOLOR _tColor2, tstring _strColorContent2)
{
	m_eDescriptionType = _eDescription;

	m_strContent1 = _content1;
	m_strTitle = _title;

	m_tColor1 = _tColor1;
	m_strColorTitle1 = _strColorTitle1;
	m_strColorContent1 = _strColorContent1;

	m_tColor2 = _tColor2;
	m_strColorTitle2 = _strColorTitle2;
	m_strColorContent2 = _strColorContent2;
}

void CDescription::AddComponent()
{
	__super::AddComponent();

	shared_ptr<CComponent> pComponent;

	// 틀 텍스처
	pComponent = m_pTextureCom = make_shared<CTexture>(m_pGraphicDev);
	m_pTextureCom->SetTextureKey(L"", TEX_NORMAL);
	m_mapComponent[ID_STATIC].insert({ L"Com_Texture", pComponent });

	// 스킬 위치 On 텍스처
	pComponent = m_pFillPosTexture = make_shared<CTexture>(m_pGraphicDev);
	m_pFillPosTexture->SetTextureKey(L"", TEX_NORMAL);
	m_mapComponent[ID_STATIC].insert({ L"Com_TexturePosOn", pComponent });

	// 스킬 위치&타겟 off 텍스처
	pComponent = m_pEmptyPosTexture = make_shared<CTexture>(m_pGraphicDev);
	m_pEmptyPosTexture->SetTextureKey(L"", TEX_NORMAL);
	m_mapComponent[ID_STATIC].insert({ L"Com_TexturePosOff", pComponent });

	// 스킬 타겟 On 텍스처
	pComponent = m_pFillTargetTexture = make_shared<CTexture>(m_pGraphicDev);
	m_pFillTargetTexture->SetTextureKey(L"", TEX_NORMAL);
	m_mapComponent[ID_STATIC].insert({ L"Com_TextureTargetOn", pComponent });

	// 스킬 위치 Link 텍스처
	pComponent = m_pLinkPosTexture = make_shared<CTexture>(m_pGraphicDev);
	m_pLinkPosTexture->SetTextureKey(L"", TEX_NORMAL);
	m_mapComponent[ID_STATIC].insert({ L"Com_TexturePosLink", pComponent });

	// 스킬 타겟 Link 텍스처
	pComponent = m_pLinkTargetTexture = make_shared<CTexture>(m_pGraphicDev);
	m_pLinkTargetTexture->SetTextureKey(L"", TEX_NORMAL);
	m_mapComponent[ID_STATIC].insert({ L"Com_TextureTargetLink", pComponent });
}
