#include "ResourceMgr.h"

IMPLEMENT_SINGLETON(CResourceMgr)

CResourceMgr::CResourceMgr()
{
	
}

CResourceMgr::~CResourceMgr()
{
}

void CResourceMgr::ReadyResource(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	m_pGraphicDev = _pGraphicDev;

}

HRESULT CResourceMgr::CreateNewTexture(const tstring& _KeyName, TEXTUREID _eType, wstring _pPath, const _uint& _iCnt)
{
	if (_eType >= TEXTUREID::TEX_END)
		return E_FAIL;
	
	IDirect3DBaseTexture9* pTexture = nullptr;
	vector<IDirect3DBaseTexture9*> vTempVec;
	vTempVec.reserve(_iCnt);
	
	for (_uint i = 0; i < _iCnt; ++i)
	{
		TCHAR		szFileName[128] = L"";
		wstring strPathCopy = _pPath;

		if (i < 10 && 1 != _iCnt) {
			size_t iSize = strPathCopy.find(L"%");
			strPathCopy.insert(iSize, L"0");
		}

		wsprintf(szFileName, strPathCopy.c_str(), i);

		switch (_eType)
		{
		case TEX_NORMAL:
			FAILED_CHECK_RETURN(D3DXCreateTextureFromFile(m_pGraphicDev, szFileName, (LPDIRECT3DTEXTURE9*)&pTexture), E_FAIL);
			break;

		case TEX_CUBE:
			FAILED_CHECK_RETURN(D3DXCreateCubeTextureFromFile(m_pGraphicDev, szFileName, (LPDIRECT3DCUBETEXTURE9*)&pTexture), E_FAIL);
			break;
		}

		vTempVec.push_back(pTexture);
	}


	if (TEXTUREID::TEX_CUBE == _eType) {

		m_CubeMap.insert({ _KeyName,vTempVec });
	}
	else if (TEXTUREID::TEX_NORMAL == _eType)
		m_TextureMap.insert({ _KeyName,vTempVec });

	return S_OK;
}

vector<IDirect3DBaseTexture9*>* CResourceMgr::GetTexture(const tstring& _keyName, TEXTUREID _eType)
{

	if (_eType == TEXTUREID::TEX_NORMAL) {
		auto iter = m_TextureMap.find(_keyName);

		if (iter == m_TextureMap.end())
			return nullptr;

		return &(iter->second);
	}
	else if (_eType == TEXTUREID::TEX_CUBE) {
		auto iter = m_CubeMap.find(_keyName);

		if (iter == m_CubeMap.end())
			return nullptr;

		return &(iter->second);
	}

	return nullptr;
}


void CResourceMgr::RemoveAllTexture()
{
	for (auto& iter : m_TextureMap) {
		for_each(iter.second.begin(), iter.second.end(), CDeleteObj());
		iter.second.clear();
		m_TextureMap.erase(iter.first);
	}

	for (auto& iter : m_CubeMap) {
		for_each(iter.second.begin(), iter.second.end(), CDeleteObj());
		iter.second.clear();
		m_CubeMap.erase(iter.first);
	}

}

void CResourceMgr::Free()
{
	RemoveAllTexture();
}
