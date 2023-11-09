#include "pch.h"
#include "Effect.h"

#include "Component.h"
#include "RcTex.h"
#include "Transform.h"
#include "Animator.h"
#include "ResourceMgr.h"


CEffect::CEffect(LPDIRECT3DDEVICE9 pGraphicDev)
    : Engine::CGameObject(pGraphicDev)
{
}

CEffect::~CEffect()
{
}

HRESULT CEffect::ReadyGameObject()
{
    return S_OK;
}

_int CEffect::UpdateGameObject(const _float& fTimeDelta)
{
    m_fDeltaTime = fTimeDelta;

    _int iExit = __super::UpdateGameObject(fTimeDelta);

    if (!m_bLoop)
    {
        if (m_pAnimatorCom->CheckFinish())
        {
            m_bActive = false;
        }
    }

    if (m_bMoving)
        Move();

    if (m_bFading)
        Fade();

    return iExit;
}

void CEffect::LateUpdateGameObject()
{
}

void CEffect::RenderGameObject()
{
    m_pTransformCom->SetPosition(m_vPos->x + m_vPosGap.x, m_vPos->y + m_vPosGap.y, m_vPos->z + m_vPosGap.z);
    m_pTransformCom->SetScale(m_vScale->x * m_vScaleGap.x, m_vScale->y * m_vScaleGap.y, m_vScale->z * m_vScaleGap.z);

    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->GetWorld());
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
    m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, FALSE);

    m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
    m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    m_pGraphicDev->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
    
    m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
    m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);

    m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(255, 255, 255, 255));

    m_pAnimatorCom->SetAnimTexture();
    m_pRcTexCom->RenderBuffer();

    m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, 0xffffffff);

    m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
    m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

    __super::RenderGameObject();
}

void CEffect::AddComponent()
{
    shared_ptr<CComponent> pComponent;

    // RcTex
    pComponent = m_pRcTexCom = make_shared<CRcTex>(m_pGraphicDev);
    m_pRcTexCom->ReadyBuffer();
    m_mapComponent[ID_STATIC].insert({ L"Com_RcTex",pComponent });

    // Transform
    pComponent = m_pTransformCom = make_shared<CTransform>();
    m_pTransformCom->ReadyTransform();
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform",pComponent });

    // Animator
    pComponent = m_pAnimatorCom = make_shared<CAnimator>(m_pGraphicDev);
    m_pAnimatorCom->SetAnimKey(m_strAnimKey, 0.05f);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Animator", pComponent });
}

void CEffect::SetInfo(tstring _strAnimKey, _vec2 _vTextureScale, _vec3* _vPos, const _vec3* _vScale, _float _fAnimTime, _bool _bLoop)
{
    m_strAnimKey = _strAnimKey;
    m_fAnimTime = _fAnimTime;
    m_bLoop = _bLoop;

    m_vPos = _vPos;
    m_vScale = _vScale;

    m_pAnimatorCom->SetAnimKey(m_strAnimKey, m_fAnimTime / CResourceMgr::GetInstance()->GetTexture(m_strAnimKey, TEX_NORMAL)->size());

    shared_ptr< tagTextureInfo> p = *CResourceMgr::GetInstance()->GetTexture(m_strAnimKey, TEX_NORMAL)->begin();
    _vec2 vSize = p->vImgSize;

    m_vScaleGap = { vSize.x / _vTextureScale.x, vSize.y / _vTextureScale.y, 1.f };
}

void CEffect::SetPos(_vec3* _vPos)
{
    m_vPos = _vPos;
}

void CEffect::SetScale(const _vec3* _vScale)
{
    m_vScale = _vScale;
}

void CEffect::SetMove(_bool _bMove, _vec3 _vDir, _float _fTime)
{
    m_bMoving = _bMove;
    m_vMoveDir = _vDir;
    m_fMoveTime = _fTime;
}

void CEffect::SetFade(_bool _bFade, _bool _bAppear, _float _fTime)
{
    m_bFading = _bFade;
    m_bAppear = _bAppear;
    m_fFadeTime = _fTime;

    if (m_bAppear)
        m_iAlpha = 0;

    else
        m_iAlpha = 255;
}

void CEffect::Move()
{
    m_vPosGap += m_vMoveDir * m_fDeltaTime;

    m_fAccumMoveTime += m_fDeltaTime;

    // ¹«ºù Á¾·á
    if (m_fAccumMoveTime >= m_fMoveTime)
    {
        m_bMoving = false;

        m_fAccumMoveTime = 0.f;
        m_fMoveTime = 0.f;
        m_vMoveDir = { 0.f, 0.f, 0.f };
    }
}

void CEffect::Fade()
{
    if (m_bAppear)
    {
        m_iAlpha += 1;
        if (m_iAlpha >= 255)
            m_iAlpha = 255;
    }

    else
    {
        m_iAlpha -= 1;
        if (m_iAlpha <= 0) m_iAlpha = 0;
    }

    m_fAccumFadeTime += m_fDeltaTime;

    // ¹«ºù Á¾·á
    if (m_fAccumFadeTime >= m_fFadeTime)
    {
        m_bFading = false;

        m_fAccumFadeTime = 0.f;
        m_fFadeTime = 0.f;
    }
}
