#pragma once
#include "Creature.h"

class CMonster : public CCreature
{
public:
	explicit CMonster(LPDIRECT3DDEVICE9 pGraphicDev);
	CMonster(const CCreature& rhs);
	virtual ~CMonster();

public:
	virtual HRESULT ReadyGameObject() override;
	virtual _int UpdateGameObject(const _float& fTimeDelta) override;
	virtual void LateUpdateGameObject() override;
	virtual void RenderGameObject() override;

protected:
	virtual void			AddComponent();
	virtual void			ClimbingTerrain();

protected:
	virtual void	Free();
};

