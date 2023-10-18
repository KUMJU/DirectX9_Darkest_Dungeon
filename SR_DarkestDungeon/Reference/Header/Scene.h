#pragma once
#include"Engine_Define.h"
#include"Layer.h"

BEGIN(Engine)

class ENGINE_DLL CScene
{
public:
	explicit CScene(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CScene();

public:
	virtual HRESULT			ReadyScene();
	virtual _int			UpdateScene(const _float& _fTimeDelta);
	virtual void			LateUpdateScene();
	virtual void			RenderScene();



public:
	void CreateNewLayer(const tstring& _newLayerName);

public:
	shared_ptr<CComponent> GetComponent(const tstring& _strLayerName, const tstring& _strObjName, const tstring& _strComName, COMPONENTID _eID);

protected:
	LPDIRECT3DDEVICE9	m_pGraphicDev;
	unordered_map<tstring, std::shared_ptr<CLayer>> m_mapLayer;

};

END
