#pragma once
#include "Hero.h"

class CVestal : public CHero
{
	explicit CVestal(LPDIRECT3DDEVICE9 pGraphicDev);
	CVestal(const CVestal& rhs);
	virtual ~CVestal();

public:
	virtual HRESULT ReadyGameObject() override;
	virtual _int UpdateGameObject(const _float& fTimeDelta) override;
	virtual void LateUpdateGameObject() override;
	virtual void RenderGameObject() override;
};

