#pragma once

#include"GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;

END

class CInteractionObj : public CGameObject
{
public:
	CInteractionObj(LPDIRECT3DDEVICE9 pGraphicDev);
	CInteractionObj(const CInteractionObj& rhs);
	virtual ~CInteractionObj();

public:
	virtual HRESULT			ReadyGameObject();
	virtual _int			UpdateGameObject(const _float& fTimeDelta);
	virtual void			LateUpdateGameObject();
	virtual void			RenderGameObject();

protected:
	virtual void			AddComponent();

protected:
	// 플레이어와 Object의 거리를 계산하는 함수
	virtual _float	CalcDistance();

	// 상호작용 키를 띄우는 함수
	virtual void	GetInteractionKey(const _float& fTimeDelta);

	// 상호작용 중인지를 알려주는 함수 (플레이어 이동을 막기 위함)
	virtual _bool	IsInteracting() { return m_bInteracting; }

	// 상호작용 동작
	virtual void	InteractionAction();


protected:
	shared_ptr<CRcTex> m_pBufferCom = nullptr;
	shared_ptr<CTexture> m_pTextureCom = nullptr;
	shared_ptr <CTransform> m_pTransformCom = nullptr;

	// 플레이어의 위치를 받아오기 위한 플레이어 오브젝트
	shared_ptr<CGameObject> m_pPlayer = nullptr;
	shared_ptr<CTransform> m_pPlayerTransformCom = nullptr;

	// 상호작용 상태를 저장하는 변수
	_bool	m_bInteracting = false;
};