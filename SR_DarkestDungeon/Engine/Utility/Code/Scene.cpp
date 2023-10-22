#include "Scene.h"

CScene::CScene(LPDIRECT3DDEVICE9 _pGraphicDev) : m_pGraphicDev(_pGraphicDev)
{
}

CScene::~CScene()
{
}

HRESULT CScene::ReadyScene()
{
	return S_OK;
}

_int CScene::UpdateScene(const _float& _fTimeDelta)
{
	_int		iResult(0);

	for (auto& iter : m_mapLayer)
	{
		iResult = iter.second->UpdateLayer(_fTimeDelta);

		if (iResult & 0x80000000)
			return iResult;
	}

	return _int();
}

void CScene::LateUpdateScene()
{
	for (auto& iter : m_mapLayer)
		iter.second->LateUpdateLayer();


}

void CScene::RenderScene()
{
	//로그 출력 및 확인용 함수
}

void CScene::CreateNewLayer(const tstring& _newLayerName)
{
	shared_ptr<CLayer> m_NewLayer = shared_ptr<CLayer>();
	m_NewLayer->ReadyLayer();
	m_mapLayer.insert({_newLayerName, m_NewLayer});

}

HRESULT CScene::AddNewObject(const tstring& _strLayerName, const tstring& _strObjKey, shared_ptr<CGameObject> _pObj)
{
	return E_NOTIMPL;
}

shared_ptr<CComponent> CScene::GetComponent(const tstring& _strLayerName, const tstring& _strObjName, const tstring& _strComName, COMPONENTID _eID)
{
	auto iter = m_mapLayer.find(_strLayerName);

	if (m_mapLayer.end() == iter) {
		return nullptr;
	}
	return (iter->second)->GetComponent(_strObjName, _strComName, _eID);
}
