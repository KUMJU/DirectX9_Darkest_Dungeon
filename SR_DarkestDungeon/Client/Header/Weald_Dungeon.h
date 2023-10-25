#pragma once

#include"Scene.h"
#include "Creature.h"

class CDungeonSystem;
class CDungeonRoom;

class CWeald_Dungeon : public Engine::CScene
{
public:
	explicit CWeald_Dungeon(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CWeald_Dungeon();

public:
	virtual HRESULT ReadyScene() override;
	virtual _int UpdateScene(const _float& fTimeDelta) override;
	virtual void LateUpdateScene() override;
	virtual void RenderScene() override;

public:
	const _tchar* Get_String() const { return m_szString; }

private:

	HRESULT				Ready_Layer_Environment(tstring pLayerTag);
	HRESULT				Ready_Layer_SkyBox(tstring pLayerTag);
	HRESULT				Ready_Layer_Camera(tstring pLayerTag);

	HRESULT				Ready_Layer_GameObject(tstring pLayerTag);
	HRESULT				Ready_Layer_UI(tstring pLayerTag);

	shared_ptr<CDungeonSystem> m_pWealdDungeon = {};	// 던전1
	shared_ptr<CDungeonRoom> m_pRoom1 = {};				// 방1
	shared_ptr<CDungeonRoom> m_pRoom2 = {};				// 방2

	_bool				m_bTestBattle = false;

	_tchar		m_szString[128];
};

