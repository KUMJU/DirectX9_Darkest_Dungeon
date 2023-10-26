#pragma once
#include "Monster.h"

#define	HITTEDTIME	1.f
#define	ATTACK1TIME	1.f
#define	ATTACK2TIME	1.f

class CBrigandBloodletter : public CMonster
{
public:
	explicit CBrigandBloodletter(LPDIRECT3DDEVICE9 pGraphicDev);
	CBrigandBloodletter(const CCreature& rhs);
	virtual ~CBrigandBloodletter();

public:
	virtual HRESULT ReadyGameObject() override;
	virtual _int UpdateGameObject(const _float& fTimeDelta) override;
	virtual void LateUpdateGameObject() override;
	virtual void RenderGameObject() override;

private:
	virtual void			AddComponent();

private:
	virtual void	Free();

private:
	_float		m_fHittedTime = HITTEDTIME;
	_float		m_fAttack1Time = ATTACK1TIME;
	_float		m_fAttack2Time = ATTACK2TIME;
};

