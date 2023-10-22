#include "Layer.h"
#include "CollisionMgr.h"

CLayer::CLayer()
{
}

CLayer::~CLayer()
{
}

void CLayer::AwakeLayer()
{
	for (auto& iter : m_objectMap) {
		for (auto& it : iter.second) {
			//GameComponenet Setting
			it->AwakeGameObject();
		}
	}


}

HRESULT CLayer::ReadyLayer()
{

	for (auto& iter : m_objectMap) {
		for (auto& it : iter.second) {
			//GameComponenet Setting
			it->ReadyGameObject();
		}
	}


	return S_OK;
}

_int CLayer::UpdateLayer(const _float& fTimeDelta)
{
	_int		iResult(0);

	for (auto& iter : m_objectMap)
	{
		for (auto it = iter.second.begin(); it != iter.second.end();) {

			//Dead Check
			if (false == ((*it)->GetIsActive())) {
				it->reset();
				iter.second.erase(it);
				it++;
				continue;
			}

			//Enable Check
			if (false == ((*it)->GetIsEnable())) {
				it++;
				continue;
			}

			iResult = (*it)->UpdateGameObject(fTimeDelta);

			if (iResult & 0x80000000)
				return iResult;

			it++;
		}
	}

	return iResult;
}

void CLayer::LateUpdateLayer()
{
	for (auto& iter : m_objectMap)
	{
		for (auto it = iter.second.begin(); it != iter.second.end();) {

			//Dead Check
			if (false == ((*it)->GetIsActive())) {
				it->reset();
				iter.second.erase(it);
				it++;
				continue;
			}

			//Enable Check
			if (false == ((*it)->GetIsEnable())) {
				it++;
				continue;
			}

			(*it)->LateUpdateGameObject();
			
			// Collision Check
			if ((*it)->IsColliding())
			{
				CCollisionMgr::GetInstance()->CheckCollision(*it);
			}

			it++;
		}
	}
}

HRESULT CLayer::CreateGameObject(tstring _strObjName, shared_ptr<CGameObject> _pObject)
{
	auto iter = m_objectMap.find(_strObjName);
	
	//이미 동일 오브젝트를 넣은 적이 있을 때
	if (m_objectMap.end() == iter) {
		list<shared_ptr<CGameObject>> vTempVec;
		vTempVec.push_back(_pObject);
		m_objectMap.insert({ _strObjName ,vTempVec });

	}
	else {
		m_objectMap[_strObjName].push_back(_pObject);
	}


	return E_NOTIMPL;
}

shared_ptr<CComponent> CLayer::GetComponent(const tstring& _strObjName, const tstring& _strComName, COMPONENTID _eID)
{
	auto iter = m_objectMap.find(_strObjName);

	if (iter == m_objectMap.end())
		return nullptr;

	return iter->second.front()->GetComponent(_strComName, _eID);
}
