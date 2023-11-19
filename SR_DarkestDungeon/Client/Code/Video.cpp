#include "pch.h"
#include "Video.h"
#include "GameMgr.h"

CVideo::CVideo(LPDIRECT3DDEVICE9 pGraphicDev)
    : m_pGraphicDev(pGraphicDev)
    , m_bFinish(false)
	, m_eVideoID(VIDEOID::ENUM_END)
{
}

CVideo::~CVideo()
{
}

HRESULT CVideo::Ready_Video(VIDEOID eVideo)
{
	m_eVideoID = eVideo;

	switch (eVideo)
	{
	case CVideo::VIDEOID::VIDEO_START:
		PlayVideo(L"../Bin/Resource/Video/old_road.wmv", g_hWnd);
		break;
	case CVideo::VIDEOID::VIDEO_ENDING:
		PlayVideo(L"../Bin/Resource/Video/epilog_long.wmv", g_hWnd);
		break;
	default:
		break;
	}

	return S_OK;
}

void CVideo::PlayVideo(const wchar_t* videoFilename, HWND hwnd) {

	CGameMgr::GetInstance()->SetGameState(EGameState::VIDEO);

	CoInitialize(NULL);

	CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void**)&m_pGraph);
	m_pGraph->QueryInterface(IID_IMediaControl, (void**)&m_pControl);
	m_pGraph->QueryInterface(IID_IMediaEventEx, (void**)&m_pEvent);

	// Add the source filter to the graph
	CoCreateInstance(CLSID_AsyncReader, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (void**)&m_pSource);
	m_pGraph->AddFilter(m_pSource, L"Source");

	// Load the video file
	m_pSource->QueryInterface(IID_IFileSourceFilter, (void**)&m_pFileSourceFilter);
	m_pFileSourceFilter->Load(videoFilename, NULL);

	// Render the file
	m_pGraph->RenderFile(videoFilename, NULL);

	// Set the video window
	m_pGraph->QueryInterface(IID_IVideoWindow, (void**)&m_pVidWin);
	m_pVidWin->put_Owner((OAHWND)hwnd);
	m_pVidWin->put_WindowStyle(WS_CHILD | WS_CLIPSIBLINGS);
	m_pVidWin->put_MessageDrain((OAHWND)hwnd);
	m_pVidWin->put_Left(0);
	m_pVidWin->put_Top(0);
	m_pVidWin->put_Width(1280); // Adjust to your window's width
	m_pVidWin->put_Height(720); // Adjust to your window's height
	m_pVidWin->put_Visible(OATRUE);

	// Run the graph
	HRESULT hr = m_pControl->Run();

	// Wait for completion or timeout after 5000 milliseconds (5 seconds)
	DWORD result = WaitForSingleObject(m_pEvent, 100);

	//long evCode;
	//m_pEvent->WaitForCompletion(INFINITE, &evCode);

	// Cleanup
	//pVidWin->put_Visible(OAFALSE);  // Hide the video window
	//m_pControl->Release();
	//pEvent->Release();
	//pVidWin->Release();
	//pGraph->Release();
	//pFileSourceFilter->Release();

	//CoUninitialize();
}

void CVideo::StopVideo()
{
	if(m_pControl)
		m_pControl->Stop();
	CGameMgr::GetInstance()->SetGameState(EGameState::PRCESS);
}
