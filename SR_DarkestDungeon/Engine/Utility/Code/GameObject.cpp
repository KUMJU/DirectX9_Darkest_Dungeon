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

std::shared_ptr<CComponent> CGameObject::GetComponent(const tstring& _strKeyName, COMPONENTID _eComID)
{
	auto iter = m_mapComponent->find(_strKeyName);

	if (iter == m_mapComponent->end())
		return nullptr;


	return iter->second;
}

void CGameObject::RemoveComponent()
{
}
