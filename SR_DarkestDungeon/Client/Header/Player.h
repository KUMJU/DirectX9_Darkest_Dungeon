#pragma once

#include"GameObject.h"
#include "PlayerHand.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CAnimator;
class CCollider;

END


class CPlayerHand;
class CInventory;
class CItem;

enum class EPlayerMove
{
	RIGHT,
	LEFT,
	UP,
	DOWN,
	ENUM_END
};

class CPlayer : public CGameObject
{

public:
	explicit CPlayer(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPlayer();

public:
	virtual HRESULT ReadyGameObject() override;
	virtual _int UpdateGameObject(const _float& fTimeDelta) override;
	virtual void LateUpdateGameObject() override;
	virtual void RenderGameObject() override;
	void			SetInventory(shared_ptr<CInventory> _pInventory) { m_pInventory = _pInventory; }

private:
	void			AddComponent();
	void			KeyInput(const _float& fTimeDelta);
	void			ClimbingTerrain();

	void			ShakingHand();
	void			InsertItem (shared_ptr<CItem> _pItem);
	virtual void	OnCollide(shared_ptr<CGameObject> _pObj);

protected:
	// Player Component
	shared_ptr<CTransform> m_pTransformCom = nullptr;
	shared_ptr<CCollider> m_pColliderCom = nullptr;

	//Player HandComponenet
	shared_ptr<CPlayerHand> m_pPlayerHand = nullptr;

public:
	void SetPlayerHand(shared_ptr<CPlayerHand> _pHand) { m_pPlayerHand = _pHand; }

private:
	_float		m_fSpeed = 3.f;
	EHandItem m_eCurrentItem = EHandItem::ENUM_END;
	shared_ptr<CInventory> m_pInventory = nullptr;

	_float		m_fDeltaTime = 0.f;
	EPlayerMove m_eLastMove = EPlayerMove::ENUM_END;

private:
	virtual void	Free();

};