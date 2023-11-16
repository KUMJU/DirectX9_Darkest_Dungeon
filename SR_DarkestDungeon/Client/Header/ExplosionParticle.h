#pragma once
#include "ParticleSystem.h"

class CExplosionParticle : public CParticleSystem
{
public:
	explicit CExplosionParticle(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CExplosionParticle();

public:
	virtual HRESULT	ReadyGameObject() override;
	virtual _int	UpdateGameObject(const _float& fTimeDelta) override;
	virtual void	LateUpdateGameObject() override;
	virtual void	RenderGameObject() override;

public:
	virtual void	ResetParticle(PARTICLE_ATTRIBUTE* _pAttribute);

public:
	void			SetParticle(_vec3* _vOrigin, _int _numParticles, tstring _strTextureKey);
};

