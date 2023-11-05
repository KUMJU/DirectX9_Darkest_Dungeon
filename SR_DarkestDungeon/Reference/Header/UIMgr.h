#pragma once

#include"Engine_Define.h"


BEGIN(Engine)

class CUIObj;

class ENGINE_DLL CUIMgr
{
	DECLARE_SINGLETON(CUIMgr)
private:

	explicit CUIMgr();
	virtual ~CUIMgr();

public:
	void AddUIObject(tstring _strKeyName, shared_ptr<CUIObj> _pObj);
	
	void AllVisibleOn();
	void AllVisibleOff();
	
	void SelectUIVisibleOn(tstring _strKeyName);
	void SelectUIVisibleOff(tstring _strKeyName);
	
	void NarrationOn(tstring _strKeyName);

	shared_ptr<CUIObj> FindUI(tstring _strKeyName);

	//���� Visible�� true�� UI�� �˻��� ��, �� UI�� ������� ��ŷ
	_bool PickingUI(LONG _fX, LONG _fY);
	void HoverUI(LONG _fX, LONG _fY);

private:

	map<tstring, shared_ptr<CUIObj>> m_UIList;

};


END
