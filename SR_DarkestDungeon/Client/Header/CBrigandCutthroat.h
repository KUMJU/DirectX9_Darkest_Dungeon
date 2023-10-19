#pragma once
#include "Monster.h"

class CBrigandCutthroat : public CMonster
{
public:
	explicit CBrigandCutthroat(LPDIRECT3DDEVICE9 pGraphicDev);
	CBrigandCutthroat(const CCreature& rhs);
	virtual ~CBrigandCutthroat();

public:
	virtual HRESULT ReadyGameObject() override;
	virtual _int UpdateGameObject(const _float& fTimeDelta) override;
	virtual void LateUpdateGameObject() override;
	virtual void RenderGameObject() override;

private:
	virtual void			AddComponent();
	virtual void			ClimbingTerrain();

private:
	virtual void	Free();
};

