#pragma once
#include "InteractionObj.h"
#include "CardGame.h"

class CGambling : public CInteractionObj
{
public:
	CGambling(LPDIRECT3DDEVICE9 pGraphicDev);
	CGambling(const CGambling& rhs);
	virtual ~CGambling();

public:
	virtual HRESULT			ReadyGameObject();
	virtual _int			UpdateGameObject(const _float& fTimeDelta);
	virtual void			LateUpdateGameObject();
	virtual void			RenderGameObject();

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

	//���� ����� ���� ���޹޴� �Լ� 
	void			GetReward();

private:
	shared_ptr<CCardGame> m_pCardGame = nullptr;

};

