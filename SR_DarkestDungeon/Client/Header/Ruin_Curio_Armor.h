#pragma once
#include "InteractionObj.h"

class CRuin_Curio_Armor : public CInteractionObj
{
public:
	CRuin_Curio_Armor(LPDIRECT3DDEVICE9 pGraphicDev);
	CRuin_Curio_Armor(const CRuin_Curio_Armor& rhs);
	virtual ~CRuin_Curio_Armor();

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

private:

	_float		m_fActiveTime = CURIOACTIVETIME;
	_bool		m_bActive = false;

	EState m_eAnimState = EState::IDLE;	// 애니메이션 변경을 위한 상태값
	EState m_eCurAnimState = EState::IDLE;	// 애니메이션 변경을 위한 상태값
	EState m_ePrevAnimState = EState::IDLE;	// 애니메이션 변경을 위한 상태값
	tstring		m_strAnimKey = L"";				// 애니메이션 키
	tstring		m_strEffectKey = L"";				// 이펙트 키


};

