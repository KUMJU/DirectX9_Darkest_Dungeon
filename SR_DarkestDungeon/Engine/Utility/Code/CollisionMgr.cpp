#include "CollisionMgr.h"
#include "GameObject.h"
#include "Collider.h"
#include "GameMgr.h"

IMPLEMENT_SINGLETON(CCollisionMgr)

CCollisionMgr::CCollisionMgr()
{
}

CCollisionMgr::~CCollisionMgr()
{
}

void CCollisionMgr::CheckCollision(shared_ptr<CGameObject> _pDst)
{
	shared_ptr<CGameObject> pPlayer = CGameMgr::GetInstance()->GetPlayer();

	AABB_AABB(pPlayer, _pDst);
}

bool CCollisionMgr::AABB_AABB(shared_ptr<CGameObject> _pSrc, shared_ptr<CGameObject> _pDst)
{
	shared_ptr<CCollider> pSrcCollider =
		dynamic_pointer_cast<CCollider>(_pSrc->GetComponent(L"Com_Collider", ID_DYNAMIC));

	shared_ptr<CCollider> pDsCollider =
		dynamic_pointer_cast<CCollider>(_pDst->GetComponent(L"Com_Collider", ID_DYNAMIC));

	_vec3 vSrcPos = *pSrcCollider->GetPos();
	_vec3 vSrcScale = *pSrcCollider->GetScale();

	_vec3 vDstPos = *pDsCollider->GetPos();
	_vec3 vDstScale = *pDsCollider->GetScale();

	if ((vSrcPos.x - vSrcScale.x / 2.f <= vDstPos.x + vDstScale.x / 2.f) &&
		(vSrcPos.x + vSrcScale.x / 2.f >= vDstPos.x - vDstScale.x / 2.f) &&
		(vSrcPos.y - vSrcScale.y / 2.f <= vDstPos.y + vDstScale.y / 2.f) &&
		(vSrcPos.y + vSrcScale.y / 2.f >= vDstPos.y - vDstScale.y / 2.f) &&
		(vSrcPos.z - vSrcScale.z / 2.f <= vDstPos.z + vDstScale.z / 2.f) &&
		(vSrcPos.z + vSrcScale.z / 2.f >= vDstPos.z - vDstScale.z / 2.f))
	{
		_pSrc->OnCollide(_pDst);
		_pDst->OnCollide(_pSrc);
	}

	return false;
}

bool CCollisionMgr::Sphere_AABB(shared_ptr<CGameObject> _pSrc, shared_ptr<CGameObject> _pDst)
{
	return false;
}