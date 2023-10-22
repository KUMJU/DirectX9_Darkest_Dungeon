#pragma once

#include"Component.h"
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CCollider : public CComponent
{
public:
	explicit CCollider();
	explicit CCollider(ECollideID _eCollideID);
	explicit CCollider(ECollideID _eCollideID, _vec3* _vPos, const _vec3& _vScale);
	explicit CCollider(ECollideID _eCollideID, _vec3* _vPos, const _float& _fRadius);
	~CCollider();

public:

	ECollideID GetType() { return m_eCollideID; }
	const _vec3* GetPos() { return m_vPos; }
	const _vec3* GetScale() { return &m_vScale; }
	const _float GetRadius() { return m_fRadius; }

	void SetType(ECollideID _eCollideID) { m_eCollideID = _eCollideID; }
	void SetPos(_vec3* _vPos) { m_vPos = _vPos; }
	void SetScale(const _vec3 _vScale) { m_vScale = _vScale; }
	void SetRadius(const _float _fRadius) {	m_fRadius = _fRadius; }

public:
			HRESULT	ReadyCollider();
	virtual	_int	UpdateComponent(const _float& fTimeDelta);
	virtual	void	RenderCollider(void);

private:

	ECollideID m_eCollideID;

	_vec3* m_vPos;
	_vec3 m_vScale;

	_float m_fRadius;
	//_bool	m_bVisible;

};

END