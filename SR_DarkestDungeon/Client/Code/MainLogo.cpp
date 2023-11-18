#include"pch.h"
#include "MainLogo.h"
#include"SoundMgr.h"
#include"Weald_Dungeon.h"
#include"BackGround.h"
#include"UIMgr.h"
#include"BackGround.h"
#include"Export_System.h"


CMainLogo::CMainLogo(LPDIRECT3DDEVICE9 _pGraphicDev)
	:CScene(_pGraphicDev)
{
}

CMainLogo::~CMainLogo()
{
}

HRESULT CMainLogo::ReadyScene()
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pLoading = make_shared<CLoading>(m_pGraphicDev);
	m_pLoading->Ready_Loading(CLoading::LOADING_STAGE);

	if (!m_pLoading)
		return E_FAIL;

	Ready_Layer_UI(L"Layer_UI");

	for (auto& iter : m_mapLayer) {
		iter.second->ReadyLayer();
	}

	//CSoundMgr::GetInstance()->PlayBGM(L"Main_Loading_loop.wav", 1.f);
	PlayVideo(L"../Bin/Resource/Video/epilog.wmv", g_hWnd);


	return S_OK;
}

_int CMainLogo::UpdateScene(const _float& _fTimeDelta)
{
	_int iExit = __super::UpdateScene(_fTimeDelta);

	if (true == m_pLoading->Get_Finish())
	{
		if (!m_bLoadingFin && !m_bIsFirstView) {
			m_pBackGround->SetLoadingState();
			CSoundMgr::GetInstance()->StopAll();
			CSoundMgr::GetInstance()->PlayBGM(L"Main_theme.wav", 1.f);
			m_bLoadingFin = true;
		}

	}

	return iExit;
}

void CMainLogo::LateUpdateScene()
{

	if (true == m_pLoading->Get_Finish())
	{
		if (GetAsyncKeyState(VK_RETURN))
		{
			if (!m_bIsFirstView && m_bLoadingFin) {
				CSoundMgr::GetInstance()->StopAll();
				Engine::ChangeScene(m_pLoading->GetCompleteScene());
				CSceneMgr::GetInstance()->SetLoadingState(false);
			}
		}

		if (GetAsyncKeyState(VK_SPACE)) {

			if (m_bIsFirstView) {
				m_bIsFirstView = false;
				m_pControl->Stop();
			}

		}
	}
}

void CMainLogo::RenderScene()
{
	if (m_bIsFirstView && true == m_pLoading->Get_Finish()) {
		tstring strStart = L"Loading Complete";
		_vec2 vStart = { 0.f, 400.f };
		_vec2 vEnd = { 300.f, 500.f };

		RenderFontCenter(L"Font_Point", strStart.c_str(), &vStart, &vEnd, D3DXCOLOR(0.878f, 0.807f, 0.6f, 1.f));
	}
}

HRESULT CMainLogo::Ready_Layer_UI(tstring pLayerTag)
{
	shared_ptr<CLayer> m_pLayer = make_shared<CLayer>();
	m_mapLayer.insert({ pLayerTag, m_pLayer });

	shared_ptr<CGameObject> m_pBG = make_shared<CBackGround>(m_pGraphicDev);
	m_pBackGround = dynamic_pointer_cast<CBackGround>(m_pBG);
	m_pLayer->CreateGameObject(L"Obj_BackGround", m_pBG);

	dynamic_pointer_cast<CLayer>(m_pLayer)->AwakeLayer();

	return S_OK;
}

void CMainLogo::PlayVideo(const wchar_t* videoFilename, HWND hwnd) {
	CoInitialize(NULL);

	shared_ptr<IGraphBuilder> pGraph = NULL;
	IMediaEventEx* pEvent = NULL;

	CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void**)&pGraph);
	pGraph->QueryInterface(IID_IMediaControl, (void**)&m_pControl);
	pGraph->QueryInterface(IID_IMediaEventEx, (void**)&pEvent);

	// Add the source filter to the graph
	IBaseFilter* pSource = NULL;
	CoCreateInstance(CLSID_AsyncReader, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (void**)&pSource);
	pGraph->AddFilter(pSource, L"Source");

	// Load the video file
	IFileSourceFilter* pFileSourceFilter = NULL;
	pSource->QueryInterface(IID_IFileSourceFilter, (void**)&pFileSourceFilter);
	pFileSourceFilter->Load(videoFilename, NULL);

	// Render the file
	pGraph->RenderFile(videoFilename, NULL);

	// Set the video window
	shared_ptr<IVideoWindow> pVidWin = NULL;
	pGraph->QueryInterface(IID_IVideoWindow, (void**)&pVidWin);
	pVidWin->put_Owner((OAHWND)hwnd);
	pVidWin->put_WindowStyle(WS_CHILD | WS_CLIPSIBLINGS);
	pVidWin->put_MessageDrain((OAHWND)hwnd);
	pVidWin->put_Left(0);
	pVidWin->put_Top(0);
	pVidWin->put_Width(1280); // Adjust to your window's width
	pVidWin->put_Height(720); // Adjust to your window's height
	pVidWin->put_Visible(OATRUE);

	// Run the graph
	HRESULT hr = m_pControl->Run();

	// Wait for completion or timeout after 5000 milliseconds (5 seconds)
	DWORD result = WaitForSingleObject(pEvent, 1000);

	// Cleanup
	//pVidWin->put_Visible(OAFALSE);  // Hide the video window
	//m_pControl->Release();
	//pEvent->Release();
	//pVidWin->Release();
	//pGraph->Release();
	//pFileSourceFilter->Release();

	CoUninitialize();
}