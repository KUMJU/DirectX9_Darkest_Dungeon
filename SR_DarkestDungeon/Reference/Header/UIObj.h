#pragma once
#include"GameObject.h"
#include"Transform.h"
#include"RcTex.h"

BEGIN(Engine)

class ENGINE_DLL CUIObj : public CGameObject
{
public:
	explicit CUIObj(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CUIObj();

public:
	virtual HRESULT ReadyGameObject() override;
	virtual _int UpdateGameObject(const _float& fTimeDelta) override;
	virtual void LateUpdateGameObject() override;
	virtual void RenderGameObject() override;

protected:
	void			AddComponent();


public:

	void SetSize(_vec3 _vSize) { m_vSize = _vSize; }
	_vec3 GetSize() { return m_vSize; }

	void SetVisible(_bool _bValue) { m_bVisible = _bValue; }
	_bool GetVisible() { return m_bVisible; }

	RECT GetUIRect() { return m_UIRect; }

	virtual void PickingUI(LONG _fX, LONG _fY) {};
	virtual void HoverUI(LONG _fX, LONG _fY) {};

protected:

	void CalculateUIRect();

protected:

	shared_ptr<CTransform> m_pTransCom = nullptr;
	shared_ptr<CRcTex> m_pRcTexCom = nullptr;

	//UI Width(x), Height(y) ���û��¿����� ũ�Ⱑ 2�̱⶧���� ���� ũ���� 1/2�� ������ ����
	//�� UI ReadyObject() ���� �������ֱ�
	_vec3 m_vSize = { 0.f, 0.f, 0.f};
	_bool m_bVisible = true;

	RECT m_UIRect;

};

END