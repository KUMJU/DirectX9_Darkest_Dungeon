#pragma once

#include"Engine_Define.h"


BEGIN(Engine)

class CGameObject;

enum class EUIType {
	INVENTORY,
	HERO_STATE,
	MONSTER_STATE,
	MINIMAP,
	ENUM_END
};


class CUIMgr
{
	DECLARE_SINGLETON(CUIMgr)
private:

	explicit CUIMgr();
	virtual ~CUIMgr();

public:
	void AddUIObject(EUIType _eType, shared_ptr<CGameObject>);
	
	void AllVisibleOn();
	void AllVisibleOff();
	
	void SelectUIVisibleOn(EUIType _eType);
	void SelectUIVisibleOn();
	


private:

	vector<shared_ptr<CGameObject>> m_UIList[static_cast<unsigned int>(EUIType::ENUM_END)];

};


END
