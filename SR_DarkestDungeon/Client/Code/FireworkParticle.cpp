#include "pch.h"
#include "FireworkParticle.h"

#include "GameMgr.h"
#include "Transform.h"

CFireworkParticle::CFireworkParticle()
{
	m_fSize = 0.1f;
	m_vbSize = 2048;
	m_vbOffset = 0;
	m_vbBatchSize = 512;
}

void CFireworkParticle::Setting(_vec3* _vOrigin, _int _iNumParticles)
{
	m_vOrigin = *_vOrigin;

	for (int i = 0; i < _iNumParticles; ++i)
		AddParticle();
}

CFireworkParticle::~CFireworkParticle()
{
}

_int CFireworkParticle::UpdateGameObject(const _float& fTimeDelta)
{
	list<PARTICLE_ATTRIBUTE>::iterator i;

	for (i = m_listParticles.begin(); i != m_listParticles.end(); i++)
	{
		if (i->bIsAlive)
		{
			i->vPosition += i->vVelocity * fTimeDelta;

			i->fAge += fTimeDelta;

			if (i->fAge > i->fLifeTime)
				i->bIsAlive = false;
		}
	}

	return S_OK;
}

void CFireworkParticle::RenderGameObject()
{
	__super::RenderGameObject();
}

void CFireworkParticle::PreRender()
{
	D3DXMATRIX matTriangleWorld;
	
	_vec3* pPlayerPos = dynamic_pointer_cast<CTransform>(CGameMgr::GetInstance()->GetPlayer()->GetComponent(L"Com_Transform", ID_DYNAMIC))->GetPos();


	D3DXMatrixIdentity(&matTriangleWorld);
	D3DXMatrixTranslation(&matTriangleWorld, m_vOrigin.x - pPlayerPos->x, m_vOrigin.y - pPlayerPos->y, m_vOrigin.z - pPlayerPos->z);
	/*matTriangleWorld.m[0][0] = 0.1f;
	matTriangleWorld.m[1][1] = 0.1f;
	matTriangleWorld.m[2][2] = 0.1f;*/
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matTriangleWorld);

	__super::PreRender();

	/*m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);*/

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, false);
}

void CFireworkParticle::PostRender()
{
	__super::PostRender();

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, true);
}

void CFireworkParticle::ResetParticle(PARTICLE_ATTRIBUTE* attribute)
{
	attribute->bIsAlive = true;
	attribute->vPosition = m_vOrigin;

	_vec3 min = _vec3(-1.f, - 1.f, -1.f);
	_vec3 max = _vec3(1.f, 1.f, 1.f);

	GetRandomVector(
		&attribute->vVelocity,
		&min,
		&max);

	D3DXVec3Normalize(
		&attribute->vVelocity,
		&attribute->vVelocity);

	attribute->vVelocity *= 30.f;

	attribute->tColor = D3DXCOLOR(
		GetRandomFloat(0.f, 1.f),
		GetRandomFloat(0.f, 1.f),
		GetRandomFloat(0.f, 1.f),
		1.f);

	attribute->fAge = 0.f;
	attribute->fLifeTime = 2.f;
}
