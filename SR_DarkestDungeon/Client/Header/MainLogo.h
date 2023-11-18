#pragma once

#include"Export_Utility.h"
#include"Loading.h"

class CBackGround;

class CMainLogo : public CScene
{
public:

	explicit CMainLogo(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CMainLogo();

public:
	virtual HRESULT			ReadyScene();
	virtual _int			UpdateScene(const _float& _fTimeDelta);
	virtual void			LateUpdateScene();
	virtual void			RenderScene();


public:

	HRESULT				Ready_Layer_UI(tstring pLayerTag);

	void PlayVideo(const wchar_t* videoFilename, HWND hwnd);

private:
	
	_bool m_bLoadingFin = false;
	_bool m_bIsFirstView = true;
	shared_ptr<CLoading> m_pLoading;

	shared_ptr<CBackGround> m_pBackGround;

	shared_ptr<IMediaControl> m_pControl = NULL;

};

