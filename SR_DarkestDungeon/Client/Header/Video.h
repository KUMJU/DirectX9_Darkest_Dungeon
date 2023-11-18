#pragma once
#include "Engine_Define.h"
class CVideo
{
public:
	enum class VIDEOID { VIDEO_START, VIDEO_ENDING, ENUM_END };

public:
	explicit CVideo(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CVideo();

public:
	VIDEOID			Get_VideoID() const { return m_eVideoID; }

public:
	HRESULT				Ready_Video(VIDEOID eVideo);
	void				PlayVideo(const wchar_t* videoFilename, HWND hwnd);
	void				StopVideo();

private:
	LPDIRECT3DDEVICE9	m_pGraphicDev;

	//Checking State
	_bool				m_bFinish;

	VIDEOID				m_eVideoID;

	shared_ptr<IGraphBuilder> m_pGraph = NULL;
	IMediaEventEx* m_pEvent = NULL;
	IBaseFilter* m_pSource = NULL;
	IFileSourceFilter* m_pFileSourceFilter = NULL;
	shared_ptr<IVideoWindow> m_pVidWin = NULL;
	shared_ptr<IMediaControl> m_pControl = NULL;
};