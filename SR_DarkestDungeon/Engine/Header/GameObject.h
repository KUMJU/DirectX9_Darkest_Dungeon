#pragma once

#include"Engine_Define.h"

BEGIN(Engine)

class CComponent;

class ENGINE_DLL CGameObject : public enable_shared_from_this<CGameObject>
{
public:
	explicit CGameObject();
	explicit CGameObject(LPDIRECT3DDEVICE9 _pGraphicDev);
	~CGameObject();

public:
	virtual void			AwakeGameObject();
	virtual HRESULT			ReadyGameObject() { return S_OK; }
	virtual _int			UpdateGameObject(const _float& fTimeDelta);
	virtual void			LateUpdateGameObject();
	virtual void			RenderGameObject() {}

public:
	_bool GetIsActive() { return m_bActive; }
	_bool GetIsEnable() { return m_bEnable; }

	void SetActive(_bool _bValue) { m_bActive = _bValue; }
	void SetEnable(_bool _bValue) { m_bEnable = _bValue; }

	void SetPos(_vec3 _vPos) { m_vPos = _vPos; }
	void SetAngle(_vec3 _vAngle) { m_vAngle = _vAngle; }

	void	SetBillBoard(_matrix& _matWorld);


	std::shared_ptr<CComponent> GetComponent(const tstring& _strKeyName, COMPONENTID _eComID);

	//Component Manage
protected:
	void RemoveComponent();

private:
	virtual void AddComponent();

protected:
	LPDIRECT3DDEVICE9 m_pGraphicDev;

	_bool m_bActive = true;
	_bool m_bEnable = true;

	_vec3 m_vPos = _vec3{ 0.f, 0.f, 0.f };
	_vec3 m_vAngle = _vec3{ 0.f, 0.f, 0.f };

protected:
	map<tstring, std::shared_ptr<CComponent>> m_mapComponent[ID_END];

};

END
