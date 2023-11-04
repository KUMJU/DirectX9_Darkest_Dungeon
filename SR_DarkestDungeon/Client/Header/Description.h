#pragma once
#include "UIObj.h"

BEGIN(Engine)

class CTexture;

END

enum class EDescriptionType
{
	SIMPLE,
	TITLE,
	SKILL1,
	SKILL2,
	SKILL3,
	ENUM_END
};

class CDescription : public CUIObj
{
public:
	explicit CDescription(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CDescription();

public:
	virtual HRESULT		ReadyGameObject() override;
	virtual _int		UpdateGameObject(const _float& fTimeDelta) override;
	virtual void		LateUpdateGameObject() override;
	virtual void		RenderGameObject() override;

public:
	void				SetDescription(EDescriptionType _eDescription, tstring _content1, tstring _title = L"",
		tstring _strColorTitle1 = L"", D3DCOLOR _tColor1 = D3DCOLOR_ARGB(255, 255, 255, 255), tstring _strColorContent1 = L"",
		tstring _strColorTitle2 = L"", D3DCOLOR _tColor2 = D3DCOLOR_ARGB(255, 255, 255, 255), tstring _strColorContent2 = L"");

protected:
	void				AddComponent();

public:

protected:
	EDescriptionType m_eDescriptionType = EDescriptionType::ENUM_END;

	shared_ptr<CTexture> m_pTextureCom = nullptr;

	tstring	m_strTitle = L"";
	tstring m_strContent1 = L"";

	// 스킬에 사용되는 컴포넌트들
	shared_ptr<CTexture> m_pEmptyPosTexture = nullptr;
	shared_ptr<CTexture> m_pFillPosTexture = nullptr;
	shared_ptr<CTexture> m_pFillTargetTexture = nullptr;
	shared_ptr<CTexture> m_pLinkPosTexture = nullptr;
	shared_ptr<CTexture> m_pLinkTargetTexture = nullptr;

	D3DCOLOR m_tColor1;
	tstring	m_strColorTitle1 = L"";
	tstring m_strColorContent1 = L"";

	D3DCOLOR m_tColor2;
	tstring m_strColorTitle2 = L"";
	tstring m_strColorContent2 = L"";
};