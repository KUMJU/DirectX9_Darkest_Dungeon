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
	//�ʱ� ����
	void ReadyResource(LPDIRECT3DDEVICE9 _pGraphicDev);

public:
	//�ؽ��ĸ� �����ؼ� �����ϴ� �Լ�, ť��� �Ϲ� �ؽ��� �����ϰ� ���
	HRESULT CreateNewTexture(const tstring& _KeyName, TEXTUREID _eType, wstring _pPath, const _uint& _iCnt);
	//�ؽ��ĸ� �ҷ����� �Լ�
	vector<IDirect3DBaseTexture9*>* GetTexture(const tstring& _keyName, TEXTUREID _eType);

public:
	//�� ��ȯ ���� ������� �ʴ� �ؽ��ĸ� ����ų� ��ü�� �������� �� ���
	void RemoveAllTexture();
	void Free();

private:


	LPDIRECT3DDEVICE9 m_pGraphicDev;

	map<tstring, vector<IDirect3DBaseTexture9*> > m_TextureMap;
	map<tstring, vector<IDirect3DBaseTexture9*>> m_CubeMap;

};

END

