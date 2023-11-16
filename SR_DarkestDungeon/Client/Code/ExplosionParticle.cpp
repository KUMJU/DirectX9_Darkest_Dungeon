#include "pch.h"
//#include "ExplosionParticle.h"
//
//CExplosionParticle::CExplosionParticle(LPDIRECT3DDEVICE9 pGraphicDev)
//    : CParticleSystem(pGraphicDev)
//{
//}
//
//CExplosionParticle::~CExplosionParticle()
//{
//}
//
//HRESULT CExplosionParticle::ReadyGameObject()
//{
//    return S_OK;
//}
//
//_int CExplosionParticle::UpdateGameObject(const _float& fTimeDelta)
//{
//    for (auto& iter : m_listParticles)
//    {
//        if (iter.bIsAlive)
//        {
//            iter.vPosition += iter.vVelocity * fTimeDelta;
//
//            iter.fAge += fTimeDelta;
//
//            if (iter.fAge > iter.fLifeTime)
//                iter.bIsAlive = false;
//        }
//    }
//
//    __super::UpdateGameObject(fTimeDelta);
//
//    RemoveDeadParticles();
//
//    return S_OK;
//}
//
//void CExplosionParticle::LateUpdateGameObject()
//{
//    __super::LateUpdateGameObject();
//}
//
//void CExplosionParticle::RenderGameObject()
//{
//    m_pTextureCom->SetTexture(0);
//
//    __super::RenderGameObject();
//}
//
//void CExplosionParticle::ResetParticle(PARTICLE_ATTRIBUTE* _pAttribute)
//{
//    _pAttribute->bIsAlive = true;
//    _pAttribute->vPosition = m_vOrigin;
//
//    _vec3 vMin = _vec3(-1.f, -1.f, -1.f);
//    _vec3 vMax = _vec3(1.f, 1.f, 1.f);
//
//    // 파티클마다 랜덤 가속도 생성
//    GetRandomVector(
//        &_pAttribute->vVelocity,
//        &vMin,
//        &vMax);
//
//    // 구체를 만들기 위해서 정규화
//    D3DXVec3Normalize(
//        &_pAttribute->vVelocity,
//        &_pAttribute->vVelocity);
//
//    _pAttribute->vVelocity *= 100.f;
//
//    _pAttribute->tColor = D3DXCOLOR(
//        GetRandomFloat(0.f, 1.f),
//        GetRandomFloat(0.f, 1.f),
//        GetRandomFloat(0.f, 1.f),
//        1.f);
//
//    _pAttribute->fAge = 0.f;
//    _pAttribute->fLifeTime = 2.f;   // 2초의 수명
//}
//
//void CExplosionParticle::SetParticle(_vec3* _vOrigin, _int _numParticles, tstring _strTextureKey)
//{
//    m_vOrigin = *_vOrigin;
//    m_fSize = 0.9f;
//    m_vbSize = 2048;
//    m_vbOffset = 0;
//    m_vbBatchSize = 512;
//
//    for (int i = 0; i < _numParticles; i++)
//        AddParticle();
//
//    m_pTextureCom->SetTextureKey(_strTextureKey, TEX_NORMAL);
//}
