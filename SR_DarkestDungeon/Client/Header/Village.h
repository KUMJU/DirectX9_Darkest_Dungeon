#pragma once

#include"Scene.h"

class CVillage : public Engine::CScene
{
public:
	explicit CVillage(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CVillage();

public:
	virtual HRESULT ReadyScene() override;
	virtual _int UpdateScene(const _float& fTimeDelta) override;
	virtual void LateUpdateScene() override;
	virtual void RenderScene() override;

private:

	HRESULT				Ready_Layer_Environment(tstring pLayerTag);
	HRESULT				Ready_Layer_SkyBox(const tstring* pLayerTag);
	HRESULT				Ready_Layer_GameObject(const tstring* pLayerTag);
	HRESULT				Ready_Layer_UI(const tstring* pLayerTag);

};

