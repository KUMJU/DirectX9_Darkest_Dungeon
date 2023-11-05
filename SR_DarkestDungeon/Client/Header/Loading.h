#pragma once
#include "Engine_Define.h"

class CLoading
{
public:
	enum LOADINGID { LOADING_STAGE, LOADING_BOSS, LOADING_END };

public:
	explicit CLoading(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLoading();

public:
	_bool					Get_Finish() const { return m_bFinish; }
	CRITICAL_SECTION* Get_Crt() { return &m_Crt; }
	LOADINGID				Get_LoadingID() const { return m_eLoading; }
	const _tchar* Get_String() const { return m_szString; }

public:
	HRESULT				Ready_Loading(LOADINGID eLoading);
	_uint				Loading_ForStage();

public:
	static unsigned int CALLBACK		Thread_Main(void* pArg);

private:
	HANDLE				m_hThread;
	CRITICAL_SECTION	m_Crt;
	LPDIRECT3DDEVICE9	m_pGraphicDev;
	_bool				m_bFinish;
	LOADINGID			m_eLoading;
	_tchar				m_szString[128];

private:
	virtual void			Free();

};

