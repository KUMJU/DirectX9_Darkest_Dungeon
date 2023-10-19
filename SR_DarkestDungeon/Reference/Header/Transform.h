#pragma once

#include"Component.h"
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CTransform : public CComponent 
{
public:

	explicit CTransform();
	explicit CTransform(const _vec3& _vPos);
	explicit CTransform(const _vec3& _vPos, const _vec3& _vScale);
	explicit CTransform(const _vec3& _vPos, const _vec3& _vScale, const _vec3& _vRotation);
	~CTransform();

public:

	void GetInfo(INFO _eType, _vec3* _pInfo) {
		memcpy(_pInfo, &m_matWorld.m[_eType][0], sizeof(_vec3));
	}

	const _vec3* GetAngles() { return &m_vAngle; }
	const _vec3* GetScale() { return &m_vScale; }
	const _matrix* GetWorld() { return &m_matWorld; }

	void SetAngle(_vec3 _vAngle) { m_vAngle = _vAngle; }

public:

	void SetPosition(const _float& _fX, const _float& _fY, const _float& _fZ)
	{
		m_vInfo[INFO_POS].x = _fX;
		m_vInfo[INFO_POS].y = _fY;
		m_vInfo[INFO_POS].z = _fZ;
	
	}

	void SetScale(const _float& _fX, const _float& _fY, const _float& _fZ)
	{
		m_vScale = { _fX, _fY, _fZ };
	}

	void Rotation(ROTATION eType, const _float& fAngle)
	{
		*(((_float*)&m_vAngle) + eType) += fAngle;
	}


public:
	HRESULT			ReadyTransform();
	virtual			_int	UpdateComponent(const _float& fTimeDelta);
	virtual			void	LateUpdateComponent();

	void			MoveForward(const _vec3* pDir, const _float& fTimeDelta, const _float& fSpeed)
	{
		m_vInfo[INFO_POS] += *pDir * fTimeDelta * fSpeed;
	}

private:

	_vec3 m_vInfo[INFO_END];
	_vec3 m_vScale;
	_vec3 m_vAngle;
	_matrix m_matWorld;
};

END