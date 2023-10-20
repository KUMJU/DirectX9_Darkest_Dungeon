#pragma once

#include"Scene.h"
#include "Creature.h"

#define WALLSIZEX 12.f
#define WALLSIZEUPY 5.f
#define PATHSIZEX 18.f

class CMainLogo : public Engine::CScene
{
public:
	explicit CMainLogo(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMainLogo();

public:
	virtual HRESULT ReadyScene() override;
	virtual _int UpdateScene(const _float& fTimeDelta) override;
	virtual void LateUpdateScene() override;
	virtual void RenderScene() override;

public:
	// 추후에 던전시스템으로 옮길 예정
	// 열 맞춰서 배치하는 기능
	void	FormBattlePosition(vector<shared_ptr<CGameObject>> _pHeroes, vector<shared_ptr<CGameObject>> _pMonsters,
		_float _fAngle, _vec3 _vOrigin);

private:

	HRESULT				Ready_Layer_Environment(tstring pLayerTag);
	HRESULT				Ready_Layer_SkyBox(tstring pLayerTag);
	HRESULT				Ready_Layer_GameObject(tstring pLayerTag);
	HRESULT				Ready_Layer_UI(const tstring* pLayerTag);

};

