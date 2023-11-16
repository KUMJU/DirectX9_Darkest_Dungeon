#pragma once
#include "ParticleSystem.h"
#include "Engine_Define.h"

class CFireworkParticle : public CParticleSystem
{
public:
	explicit CFireworkParticle();
	virtual ~CFireworkParticle();

public:
	virtual _int UpdateGameObject(const _float& fTimeDelta) override;
	virtual void RenderGameObject() override;

	virtual void PreRender();
	virtual void PostRender();

	void	Setting(_vec3* _vOrigin, _int _iNumParticles);
	void	ResetParticle(PARTICLE_ATTRIBUTE* attribute);



};

