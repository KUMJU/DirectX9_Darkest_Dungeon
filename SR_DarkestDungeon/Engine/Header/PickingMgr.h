#pragma once

#include"Engine_Define.h"

BEGIN(Engine)

class CGameObject;

class ENGINE_DLL CPickingMgr
{
	DECLARE_SINGLETON(CPickingMgr)

private:
	explicit CPickingMgr();
	~CPickingMgr();

public:

	void ReadyPickingMgr(LPDIRECT3DDEVICE9 _pGraphicDev) {
		m_pGraphicDev = _pGraphicDev;
	}

	void RayPicking(LONG _lX, LONG _lY);
	_bool IntersectingRay(_vec3 _vRayPos, _vec3 _vRayNormal, _vec3 _vMin, _vec3 _vMax);

	void RemoveList();
	void AddList(shared_ptr<CGameObject> _pObj);

private:

	LPDIRECT3DDEVICE9 m_pGraphicDev = nullptr;
	list<shared_ptr<CGameObject>> m_PickingList;

};

END
