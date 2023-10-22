#include "GameObject.h"
#include"Component.h"

CGameObject::CGameObject() : m_pGraphicDev(nullptr)
{
}

CGameObject::CGameObject(LPDIRECT3DDEVICE9 _pGraphicDev): m_pGraphicDev(_pGraphicDev)
{
//	m_pGraphicDev->AddRef();
}

CGameObject::~CGameObject()
{
	//m_pGraphicDev->Release();
}

void CGameObject::AwakeGameObject()
{
	AddComponent();
}

_int CGameObject::UpdateGameObject(const _float& fTimeDelta)
{
	int iExit(0);

	for (auto& iter : m_mapComponent[ID_DYNAMIC]) {

		if (!iter.second->GetIsActive()) {
			iter.second.reset();
			m_mapComponent->erase(iter.first);
			continue;
		}

		if(iter.second->GetIsEnable())
			iExit = iter.second->UpdateComponent(fTimeDelta);

	}

	return iExit;
}

void CGameObject::LateUpdateGameObject()
{
	for (auto& iter : m_mapComponent[ID_DYNAMIC]) {

		if (!iter.second->GetIsActive()) {
			iter.second.reset();
			m_mapComponent->erase(iter.first);
			continue;
		}

		if (iter.second->GetIsEnable())
			iter.second->LateUpdateComponent();
	}
}


void CGameObject::AddComponent()
{
}

void CGameObject::SetBillBoard(_matrix& _matWorld)
{
	_matrix matBill, matView;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixIdentity(&matBill);

	matBill._11 = matView._11;
	matBill._13 = matView._13;
	matBill._31 = matView._31;
	matBill._33 = matView._33;
	D3DXMatrixInverse(&matBill, 0, &matBill);

	_matWorld = (matBill * _matWorld);
}

std::shared_ptr<CComponent> CGameObject::GetComponent(const tstring& _strKeyName, COMPONENTID _eComID)
{
	auto iter = m_mapComponent->find(_strKeyName);

	if (iter == m_mapComponent->end())
		return nullptr;


	return iter->second;
}

void CGameObject::Compute_ViewZ(const _vec3* pPos)
{
	_matrix		matCamWorld;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matCamWorld);
	D3DXMatrixInverse(&matCamWorld, 0, &matCamWorld);

	_vec3	vCamPos;
	memcpy(&vCamPos, &matCamWorld.m[3][0], sizeof(_vec3));

	vCamPos = vCamPos - *pPos;
	m_fViewZ = D3DXVec3Length(&(vCamPos));
}

void CGameObject::RemoveComponent()
{
}
