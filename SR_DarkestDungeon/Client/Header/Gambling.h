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

	// 상호작용 동작
	virtual void	Interaction();

	// 상호작용 종료 체크 함수
	virtual _bool	IsFinish();

	// 상호작용 종료 시 호출되는 함수
	virtual void	FinishInteraction();

	virtual void	ChangeTexture();

	//게임 종료시 돈을 지급받는 함수 
	void			GetReward();

private:
	shared_ptr<CCardGame> m_pCardGame = nullptr;

};

