#pragma once
#include "InteractionObj.h"

class CRuinDungeonDoor : public CInteractionObj
{
public:
	CRuinDungeonDoor(LPDIRECT3DDEVICE9 pGraphicDev, ERuinDungeonDoorType _eDoorType);
	CRuinDungeonDoor(const CRuinDungeonDoor& rhs);
	virtual ~CRuinDungeonDoor();

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

	// ��ȣ�ۿ� ����
	virtual void	Interaction();

	// ��ȣ�ۿ� ���� üũ �Լ�
	virtual _bool	IsFinish();

	// ��ȣ�ۿ� ���� �� ȣ��Ǵ� �Լ�
	virtual void	FinishInteraction();

	virtual void	ChangeTexture();

protected:
	ERuinDungeonDoorType m_eDoorType;

private:
	_int	m_iCurrent = 10;

};

