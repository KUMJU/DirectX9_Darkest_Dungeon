#pragma once

#include"Engine_Define.h"

BEGIN(Engine)

struct tTest {
	IDirect3DBaseTexture9* temp1;
	_bool temp2;
};

class ENGINE_DLL CResourceMgr
{
	DECLARE_SINGLETON(CResourceMgr)

private:
	explicit CResourceMgr();
	~CResourceMgr();

public:
	//초기 세팅
	void ReadyResource(LPDIRECT3DDEVICE9 _pGraphicDev);

public:
	//텍스쳐를 세팅해서 저장하는 함수, 큐브와 일반 텍스쳐 동일하게 사용
	HRESULT CreateNewTexture(const tstring& _KeyName, TEXTUREID _eType, wstring _pPath, const _uint& _iCnt);
	//텍스쳐를 불러오는 함수
	vector<IDirect3DBaseTexture9*>* GetTexture(const tstring& _keyName, TEXTUREID _eType);

public:
	//씬 전환 이후 사용하지 않는 텍스쳐를 지우거나 전체를 날려버릴 때 사용
	void RemoveAllTexture();
	void Free();

private:


	LPDIRECT3DDEVICE9 m_pGraphicDev;

	map<tstring, vector<IDirect3DBaseTexture9*> > m_TextureMap;
	map<tstring, vector<IDirect3DBaseTexture9*>> m_CubeMap;

};

END

