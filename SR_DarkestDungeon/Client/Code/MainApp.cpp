#include"pch.h"
#include "MainApp.h"
#include"SceneMgr.h"
#include"Weald_Dungeon.h"
#include "Village.h"

CMainApp::CMainApp()
{
}

CMainApp::~CMainApp()
{
	Free();
}

HRESULT CMainApp::Ready_MainApp()
{
	FAILED_CHECK_RETURN(SetUp_DefaultSetting(&m_pGraphicDev), E_FAIL);
	CResourceMgr::GetInstance()->ReadyResource(m_pGraphicDev);

    shared_ptr<CScene> pMainLogo = make_shared<CVillage>(m_pGraphicDev);
	Engine::ChangeScene(pMainLogo);
	pMainLogo->ReadyScene();

    return S_OK;
}

_int CMainApp::Update_MainApp(const _float& fTimeDelta)
{
	Engine::Update_InputDev();
	Engine::UpdateScene(fTimeDelta);

    return 0;
}

void CMainApp::LateUpdate_MainApp()
{
   Engine::LateUpdateScene();

}

void CMainApp::Render_MainApp()
{
    m_pDeviceClass->Render_Begin(D3DXCOLOR(0.f, 0.f, 1.f, 1.f));

    Engine::RenderScene(m_pGraphicDev);

    Engine::Render_End();

}

HRESULT CMainApp::SetUp_DefaultSetting(LPDIRECT3DDEVICE9* ppGraphicDev)
{

	FAILED_CHECK_RETURN(Engine::Ready_GraphicDev(g_hWnd, MODE_WIN, WINCX, WINCY, &m_pDeviceClass), E_FAIL);

	*ppGraphicDev = m_pDeviceClass->Get_GraphicDev();
	(*ppGraphicDev)->AddRef();

	(*ppGraphicDev)->SetRenderState(D3DRS_LIGHTING, FALSE);

	(*ppGraphicDev)->SetRenderState(D3DRS_ZENABLE, TRUE);		// Z버퍼에 깊이 값을 무조건 기록은 하지만 자동 정렬을 할지 말지 결정
	(*ppGraphicDev)->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);  // Z버퍼에 깊이 값을 기록할 지 결정

	// Font
	FAILED_CHECK_RETURN(Engine::Ready_Font((*ppGraphicDev), L"Font_Default", L"바탕", 15, 20, FW_HEAVY), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Font((*ppGraphicDev), L"Font_Jinji", L"궁서", 30, 30, FW_THIN), E_FAIL);



	// Dinput

	FAILED_CHECK_RETURN(Engine::Ready_InputDev(g_hInst, g_hWnd), E_FAIL);

	(*ppGraphicDev)->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	(*ppGraphicDev)->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	return S_OK;
}

void CMainApp::Free()
{
	Engine::Release_Utility();
	Engine::Release_System();
}
