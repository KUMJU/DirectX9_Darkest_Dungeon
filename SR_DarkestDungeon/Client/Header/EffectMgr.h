#pragma once
#include "Engine_Define.h"

#include "Transform.h"
#include "RcTex.h"
#include "Animator.h"

class CEffect;

#define	POOLSIZE 10

class CEffectMgr
{
	DECLARE_SINGLETON(CEffectMgr)

public:
	explicit CEffectMgr();
	virtual ~CEffectMgr();

public:
	void	SetEffectMgr(LPDIRECT3DDEVICE9 _pGraphicDev);

public:
	shared_ptr<CEffect> GetEffect();
	void	ReturnEffect(shared_ptr<CEffect> _pEffect);

private:
	vector<shared_ptr<CEffect>> m_pEffectList;
	LPDIRECT3DDEVICE9	m_pGraphicDev;
};