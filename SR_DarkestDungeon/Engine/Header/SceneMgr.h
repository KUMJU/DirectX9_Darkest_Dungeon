#pragma once

#include"Engine_Define.h"
#include"Enums.h"
#include"Scene.h"

BEGIN(Engine)


class ENGINE_DLL CSceneMgr
{
	DECLARE_SINGLETON(CSceneMgr)

private:
	explicit CSceneMgr();
	~CSceneMgr();

public:

	int Update(const float& _fDeltaTime);
	void LateUpdate();
	void Render(LPDIRECT3DDEVICE9 _pGraphicDev);


	// Scene 전환
	HRESULT ChangeScene(shared_ptr<CScene> _newScene);
	//새로운 Scene을 생성
	void CreateScene(const tstring& _strSceneName , shared_ptr<CScene> _newScene);

	shared_ptr<CComponent> GetComponenet(const tstring& _strLayerName, const tstring& _strObjName, const tstring& _strComName, COMPONENTID _eID);

public:
	
	shared_ptr<CScene> GetCurrentScene() { return m_pCurrentScene; }

private:
	shared_ptr<CScene> m_pCurrentScene;	
};

END

