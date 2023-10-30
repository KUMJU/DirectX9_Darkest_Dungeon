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

	if ((vSrcPos.x - vSrcScale.x / 2.f < vDstPos.x + vDstScale.x / 2.f) &&
		(vSrcPos.x + vSrcScale.x / 2.f > vDstPos.x - vDstScale.x / 2.f) &&
		(vSrcPos.y - vSrcScale.y / 2.f < vDstPos.y + vDstScale.y / 2.f) &&
		(vSrcPos.y + vSrcScale.y / 2.f > vDstPos.y - vDstScale.y / 2.f) &&
		(vSrcPos.z - vSrcScale.z / 2.f < vDstPos.z + vDstScale.z / 2.f) &&
		(vSrcPos.z + vSrcScale.z / 2.f > vDstPos.z - vDstScale.z / 2.f))
	{
		if (ECollideID::WALL == _pDst->GetColType() || ECollideID::ENVIRONMENT == _pDst->GetColType())
		{
			// x충돌 (동시검사)
			if (abs(vSrcPos.x - vDstPos.x) < ((vSrcScale.x + vDstScale.x) / 2.f))
			{
				_float fGap = (vSrcScale.x + vDstScale.x) / 2.f - abs(vSrcPos.x - vDstPos.x);

				if (vSrcPos.x < vDstPos.x)
					_pSrc->OnCollide(_pDst, fGap, EDirection::RIGHT);
				else
					_pSrc->OnCollide(_pDst, fGap, EDirection::LEFT);
			}

			// z충돌 (동시검사)
			if (abs(vSrcPos.z - vDstPos.z) < ((vSrcScale.z + vDstScale.z) / 2.f))
			{
				_float fGap = (vSrcScale.z + vDstScale.z) / 2.f - abs(vSrcPos.z - vDstPos.z);

				if (vSrcPos.z < vDstPos.z)
					_pSrc->OnCollide(_pDst, fGap, EDirection::TOP);
				else
					_pSrc->OnCollide(_pDst, fGap, EDirection::BOTTOM);
			}

			return true;
		}

		_pSrc->OnCollide(_pDst);
		_pDst->OnCollide(_pSrc);

		return true;
	}

	return false;
}

bool CCollisionMgr::Sphere_AABB(shared_ptr<CGameObject> _pSrc, shared_ptr<CGameObject> _pDst)
{
	return false;
}