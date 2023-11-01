#include"PickingMgr.h"
#include"GameObject.h"

IMPLEMENT_SINGLETON(CPickingMgr)

CPickingMgr::CPickingMgr()
{
}

CPickingMgr::~CPickingMgr()
{
}

void CPickingMgr::RayPicking(LONG _lX, LONG _lY)
{
	if (m_PickingList.empty())
		return;

	D3DVIEWPORT9		ViewPort;
	ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));
	m_pGraphicDev->GetViewport(&ViewPort);

	D3DXVECTOR3	vMouse;
	vMouse.x = (float)_lX / (ViewPort.Width * 0.5f) - 1.f;
	vMouse.y =  (float)_lY / -(ViewPort.Height * 0.5f) + 1.f;
	vMouse.z = 0.f;

	_matrix matProj, matView;
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, 0, &matProj);
	D3DXVec3TransformCoord(&vMouse, &vMouse, &matProj);

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, 0, &matView);

	_vec3		vRayPos, vRayDir;

	vRayPos = { 0.f, 0.f, 0.f };
	vRayDir = vMouse - vRayPos;
	vRayDir.z = 1.f;

	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matView);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matView);

	_vec3 vMin, vMax;

	for (auto& iter : m_PickingList) {

		iter->GetMinMaxPos(vMin, vMax);
		if (IntersectingRay(vRayPos, vRayDir, vMin, vMax)) {
			iter->PickingObj();
			break;
		}
	}
}

_bool CPickingMgr::IntersectingRay(_vec3 _vRayPos, _vec3 _vRayNormal, _vec3 _vMin, _vec3 _vMax)
{

	_float fMinX = (_vMin.x - _vRayPos.x) / _vRayNormal.x;
	_float fMaxX = (_vMax.x - _vRayPos.x) / _vRayNormal.x;

	_float fMinY = (_vMin.y - _vRayPos.y) / _vRayNormal.y;
	_float fMaxY = (_vMax.y - _vRayPos.y) / _vRayNormal.y;

	_float fMinZ = (_vMin.z	- _vRayPos.z) / _vRayNormal.z;
	_float fMaxZ = (_vMax.z - _vRayPos.z) / _vRayNormal.z;

	float tmin = (fMinX > fMinY) ? fMinX : fMinY;
	float tmax = (fMaxX < fMaxY) ? fMinX : fMinY;


	if (tmax < 0)
		return false;


	/*if (fMinX > fMaxY || fMinY > fMaxX)
		return false;*/

	if (tmin > tmax)
		return false;

	return true;
}

void CPickingMgr::RemoveList()
{
	m_PickingList.clear();
}

void CPickingMgr::AddList(shared_ptr<CGameObject> _pObj)
{
	m_PickingList.push_back(_pObj);
}
