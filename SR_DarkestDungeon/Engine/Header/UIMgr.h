#pragma once

#include"Engine_Define.h"


BEGIN(Engine)

class CUIObj;
class CGameObject;

class ENGINE_DLL CUIMgr
{
	DECLARE_SINGLETON(CUIMgr)
private:

	explicit CUIMgr();
	virtual ~CUIMgr();

public:
	void AddUIObject(tstring _strKeyName, shared_ptr<CUIObj> _pObj);
	shared_ptr<CUIObj> GetUIObject(tstring _strKeyName);
	
public:

	void SceneUIInitialize();
	void SceneChange();

	
public:


	void AllVisibleOn();
	void AllVisibleOff();
	
	void SelectUIVisibleOn(tstring _strKeyName);
	void SelectUIVisibleOff(tstring _strKeyName);
	
	void NarrationOn(tstring _strKeyName);
	void RemoveAllListElements();

	shared_ptr<CUIObj> FindUI(tstring _strKeyName);

	// ���� Visible�� true�� UI�� �˻��� ��, �� UI�� ������� ��ŷ
	_bool PickingUI(LONG _fX, LONG _fY);
	void HoverUI(LONG _fX, LONG _fY);

	// ȣ��â ���� �Լ�
	void SetDescription(shared_ptr<CGameObject> _pGameObject);
	void SetDescription(shared_ptr<CGameObject> _pGameObject, _vec3 _vPos);
	void SetDescription(tstring _strText, _vec3 _vPos);

	//�÷��̾� FPS(������)��� ��ȯ UI

	void SwitchFPSMode();

private:

	map<tstring, shared_ptr<CUIObj>> m_UIList;

};


END
