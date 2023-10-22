#include "Collider.h"

CCollider::CCollider()
{
}

CCollider::CCollider(ECollideID _eCollideID)
    : m_eCollideID(_eCollideID)
{
}

CCollider::CCollider(ECollideID _eCollideID, _vec3* _vPos, const _vec3& _vScale)
    : m_eCollideID(_eCollideID), m_vPos(_vPos), m_vScale(_vScale)
{
}

CCollider::CCollider(ECollideID _eCollideID, _vec3* _vPos, const _float& _fRadius)
    : m_eCollideID(_eCollideID), m_vPos(_vPos), m_fRadius(_fRadius)
{
}

CCollider::~CCollider()
{
}

HRESULT CCollider::ReadyCollider()
{
    return S_OK;
}

_int CCollider::UpdateComponent(const _float& fTimeDelta)
{
    return 0;
}

void CCollider::RenderCollider(void)
{
}