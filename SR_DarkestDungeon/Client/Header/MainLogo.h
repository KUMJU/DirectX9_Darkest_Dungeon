#pragma once

#include"Scene.h"

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

	// ���Ŀ� �����ý������� �ű� ����
public:
	// �� creature �ڵ� ��ġ
	void	LineBattlePosition(vector<shared_ptr<CGameObject>> _pHeroes, vector<shared_ptr<CGameObject>> _pMonsters);

private:

	HRESULT				Ready_Layer_Environment(tstring pLayerTag);
	HRESULT				Ready_Layer_SkyBox(tstring pLayerTag);
	HRESULT				Ready_Layer_GameObject(tstring pLayerTag);
	HRESULT				Ready_Layer_UI(const tstring* pLayerTag);

};

