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
	// �÷��̾�� Object�� �Ÿ��� ����ϴ� �Լ�
	virtual _float	CalcDistance();

	// ��ȣ�ۿ� Ű UI�� ���� Ű�� �Է¹޴� �Լ�
	virtual void	GetInteractionKey(const _float& fTimeDelta);

	// ��ȣ�ۿ� ����
	virtual void	InteractionAction();

	// ��ȣ�ۿ� ���� �� ȣ��Ǵ� �Լ�
	virtual void	FinishInteraction();

	// ��ȣ�ۿ� ������ Ȯ���ϴ� �Լ�
	virtual _bool	IsInteracting() { return m_bInteracting; }


protected:
	shared_ptr<CRcTex> m_pBufferCom = nullptr;
	shared_ptr<CTexture> m_pTextureCom = nullptr;
	shared_ptr <CTransform> m_pTransformCom = nullptr;

	// �÷��̾��� ��ġ�� �޾ƿ��� ���� �÷��̾� ������Ʈ
	shared_ptr<CGameObject> m_pPlayer = nullptr;
	shared_ptr<CTransform> m_pPlayerTransformCom = nullptr;

	// ��ȣ�ۿ� ���¸� �����ϴ� ����
	_bool	m_bInteracting = false;
};