#include "MyFont.h"

CMyFont::CMyFont(LPDIRECT3DDEVICE9 pGraphicDev)
    : m_pGraphicDev(pGraphicDev), m_pSprite(nullptr), m_pFont(nullptr)
{
   // m_pGraphicDev->AddRef();

}

CMyFont::~CMyFont()
{
	//m_pGraphicDev->Release();
}

HRESULT CMyFont::Ready_Font(const _tchar* pFontType, const _uint& iWidth, const _uint& iHeight, const _uint& iWeight)
{
	D3DXFONT_DESC			Font_Desc;
	ZeroMemory(&Font_Desc, sizeof(D3DXFONT_DESC));

	Font_Desc.CharSet = HANGEUL_CHARSET;
	lstrcpy(Font_Desc.FaceName, pFontType);

	Font_Desc.Width = iWidth;
	Font_Desc.Height = iHeight;
	Font_Desc.Weight = iWeight;

	FAILED_CHECK_RETURN(D3DXCreateFontIndirect(m_pGraphicDev, &Font_Desc, &m_pFont), E_FAIL);
	FAILED_CHECK_RETURN(D3DXCreateSprite(m_pGraphicDev, &m_pSprite), E_FAIL);

	return S_OK;
}

void CMyFont::Render_Font(const _tchar* pString, const _vec2* pPos, D3DXCOLOR Color)
{
	RECT	rc = { (long)pPos->x, (long)pPos->y };

	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);

	m_pFont->DrawTextW(m_pSprite, pString, lstrlen(pString), &rc, DT_NOCLIP, Color);

	m_pSprite->End();
}

shared_ptr<CMyFont> CMyFont::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFontType, const _uint& iWidth, const _uint& iHeight, const _uint& iWeight)
{
	std::shared_ptr<CMyFont> pInstance = std::make_shared<CMyFont>(pGraphicDev);


	if (FAILED(pInstance->Ready_Font(pFontType, iWidth, iHeight, iWeight)))
	{
		pInstance.reset();

		MSG_BOX("MyFont Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CMyFont::Free()
{
	m_pFont->Release();
	m_pSprite->Release();
}
