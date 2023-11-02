#include "UIMgr.h"
#include"UIObj.h"

IMPLEMENT_SINGLETON(CUIMgr)

CUIMgr::CUIMgr()
{
}

CUIMgr::~CUIMgr()
{
}

void CUIMgr::AddUIObject(tstring _strKeyName, shared_ptr<CUIObj> _pObj)
{

	if (m_UIList.end() == m_UIList.find(_strKeyName))
		m_UIList.insert({ _strKeyName ,_pObj });

}

void CUIMgr::AllVisibleOn()
{
	for (auto& iter : m_UIList) {
		iter.second->SetVisible(true);
	}
}

void CUIMgr::AllVisibleOff()
{
	for (auto& iter : m_UIList) {
		iter.second->SetVisible(false);
	}
}

void CUIMgr::SelectUIVisibleOn(tstring _strKeyName)
{
	auto iter = m_UIList.find(_strKeyName);

	if (iter != m_UIList.end())
		(*iter).second->SetVisible(true);

}

void CUIMgr::SelectUIVisibleOff(tstring _strKeyName)
{
	auto iter = m_UIList.find(_strKeyName);

	if (iter != m_UIList.end())
		(*iter).second->SetVisible(false);

}

shared_ptr<CUIObj> CUIMgr::FindUI(tstring _strKeyName)
{
	auto iter = m_UIList.find(_strKeyName);
	if (m_UIList.end() != iter){
		return iter->second;
	}

	return nullptr;

}

_bool CUIMgr::PickingUI(LONG _fX, LONG _fY)
{
	for (auto& iter : m_UIList) {
		if (iter.second->GetVisible()) {
			RECT rc = iter.second->GetUIRect();

			if (rc.left <= _fX && rc.right >= _fX
				&& rc.top <= _fY && rc.bottom >= _fY) {
				iter.second->PickingUI(_fX, _fY);
				return true;
			}
		}
	}
	return false;
}
