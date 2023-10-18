#include "SceneMgr.h"
#include"Export_Utility.h"

IMPLEMENT_SINGLETON(CSceneMgr)

CSceneMgr::CSceneMgr() : m_pCurrentScene(nullptr)
{
}

CSceneMgr::~CSceneMgr()
{
}

int CSceneMgr::Update(const float& _fDeltaTime)
{
	if (nullptr == m_pCurrentScene)
		return -1;

	return m_pCurrentScene->UpdateScene(_fDeltaTime);
}

void CSceneMgr::LateUpdate()
{
	m_pCurrentScene->LateUpdateScene();

}

void CSceneMgr::Render(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	Engine::RenderGameObject(_pGraphicDev);

	if (m_pCurrentScene)
		m_pCurrentScene->RenderScene();

}

HRESULT CSceneMgr::ChangeScene(shared_ptr<CScene> _newScene)
{
	if (m_pCurrentScene)
	{
		m_pCurrentScene.reset();
	}

	Engine::ClearRenderGroup();
	m_pCurrentScene = _newScene;

	return S_OK;
}



void CSceneMgr::CreateScene(const tstring& _strSceneName, shared_ptr<CScene> _newScene)
{

}

shared_ptr<CComponent> CSceneMgr::GetComponenet(const tstring& _strLayerName, const tstring& _strObjName, const tstring& _strComName, COMPONENTID _eID)
{
	if (!m_pCurrentScene)
		return nullptr;

	return 	m_pCurrentScene->GetComponent(_strLayerName, _strObjName, _strComName, _eID);
}
