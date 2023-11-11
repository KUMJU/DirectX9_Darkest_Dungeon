#pragma once

#include"Scene.h"
#include "Creature.h"

class CDungeonSystem;
class CDungeonRoom;
class CWall;

class CBossMap : public Engine::CScene
{
public:
	explicit CBossMap(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBossMap();

public:
	virtual HRESULT ReadyScene() override;
	virtual _int UpdateScene(const _float& fTimeDelta) override;
	virtual void LateUpdateScene() override;
	virtual void RenderScene() override;

public:
	const _tchar* Get_String() const { return m_szString; }

public:
	void SetWall(shared_ptr<CWall> _pWall, shared_ptr<CLayer> _pLayer,
		_float _fX, _float _fZ, _float _fSize, tstring _tKeyName);

private:

	HRESULT				Ready_Layer_Environment(tstring pLayerTag);
	HRESULT				Ready_Layer_SkyBox(tstring pLayerTag);
	HRESULT				Ready_Layer_Camera(tstring pLayerTag);

	HRESULT				Ready_Layer_GameObject(tstring pLayerTag);
	HRESULT				Ready_Layer_UI(tstring pLayerTag);

	shared_ptr<CDungeonSystem>	m_pBossMap = {};			//
	shared_ptr<CDungeonRoom>	m_pRoom1 = {};				// 방1	
	shared_ptr<CDungeonRoom>	m_pRoom2 = {};				// 방1	

	vector<shared_ptr<CWall>>	m_pVecWallList;		// 벽들 벡터

	_bool				m_bTestBattle = false; 

	_tchar				m_szString[128];
};

