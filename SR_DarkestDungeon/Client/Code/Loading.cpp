#include <pch.h>
#include "Loading.h"

CLoading::CLoading(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
	, m_bFinish(false)
	, m_eLoading(LOADING_END)
{
	ZeroMemory(m_szString, sizeof(m_szString));
	m_pGraphicDev->AddRef();
}

CLoading::~CLoading()
{
}

HRESULT CLoading::Ready_Loading(LOADINGID eLoading)
{
	InitializeCriticalSection(&m_Crt);
	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, Thread_Main, this, 0, nullptr);
	m_eLoading = eLoading;

	return S_OK;
}

_uint CLoading::Loading_ForStage()
{
	return _uint();
}

unsigned int CLoading::Thread_Main(void* pArg)
{
	return 0;
}

void CLoading::Free()
{
	WaitForSingleObject(m_hThread, INFINITE);
	CloseHandle(m_hThread);
	DeleteCriticalSection(&m_Crt);

	Safe_Release(m_pGraphicDev);
}
