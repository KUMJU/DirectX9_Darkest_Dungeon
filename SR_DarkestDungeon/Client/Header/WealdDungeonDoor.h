#pragma once
#include "InteractionObj.h"

class CWealdDungeonDoor : public CInteractionObj
{
public:
	CWealdDungeonDoor(LPDIRECT3DDEVICE9 pGraphicDev, EWealdDungeonDoorType _eDoorType);
	CWealdDungeonDoor(const CWealdDungeonDoor& rhs);
	virtual ~CWealdDungeonDoor();

public:
	virtual HRESULT			ReadyGameObject();
	virtual _int			UpdateGameObject(const _float& fTimeDelta);
	virtual void			LateUpdateGameObject();
	virtual void			RenderGameObject();

public:
	_int	GetCurrent() { return m_iCurrent; }

protected:
	virtual void			AddComponent();
	virtual void			GetInteractionKey(const _float& fTimeDelta);

protected:

	// 상호작용 동작
	virtual void	Interaction();

	// 상호작용 종료 체크 함수
	virtual _bool	IsFinish();

	// 상호작용 종료 시 호출되는 함수
	virtual void	FinishInteraction();

	virtual void	ChangeTexture();

protected:
	EWealdDungeonDoorType m_eDoorType;

private:
	_int	m_iCurrent = 10;
	
};

