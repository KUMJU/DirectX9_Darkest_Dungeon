#include "pch.h"
#include "Effect.h"

#include "Component.h"
#include "RcTex.h"
#include "Transform.h"
#include "Animator.h"
#include "Texture.h"

#include "ResourceMgr.h"
#include "EffectMgr.h"


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
    m_fDeltaTime = 0.016668f;

    _int iExit = __super::UpdateGameObject(fTimeDelta);

    if (!m_bLoop)
    {
        if (m_bAnimation)
        {
            if (m_pAnimatorCom->CheckFinish())
            {
                CEffectMgr::GetInstance()->ReturnEffect(dynamic_pointer_cast<CEffect>(shared_from_this()));
            }
        }

        else
        {
            if (m_fAnimTime <= 0.f)
                CEffectMgr::GetInstance()->ReturnEffect(dynamic_pointer_cast<CEffect>(shared_from_this()));

            else
            {
                m_fAnimTime -= m_fDeltaTime;
            }
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
    m_pTransformCom->SetScale(m_vScale.x * m_vScaleGap.x, m_vScale.y * m_vScaleGap.y, m_vScale.z * m_vScaleGap.z);

    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->GetWorld());
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
    m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, FALSE);

    m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
    m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    m_pGraphicDev->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
    
    /*m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
    m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);

    m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(255, 255, 255, 255));*/

    if (m_bAnimation)
    {
        m_pAnimatorCom->SetAnimTexture();
        m_pRcTexCom->RenderBuffer();
    }

    else
    {
        if (m_bTwoTexture)
        {
            m_pTransformCom->SetPosition(m_vPos->x + m_vPosGap.x - 0.4f, m_vPos->y + m_vPosGap.y, m_vPos->z + m_vPosGap.z);
            m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->GetWorld());
            m_pTextureCom1->SetTexture(0);
            m_pRcTexCom->RenderBuffer();

            m_pTransformCom2->SetPosition(m_vPos->x + m_vPosGap.x + 0.4f, m_vPos->y + m_vPosGap.y, m_vPos->z + m_vPosGap.z);
            m_pTransformCom2->SetScale(m_vScale.x * m_vScaleGap.x, m_vScale.y * m_vScaleGap.y, m_vScale.z * m_vScaleGap.z);
            m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom2->GetWorld());
            m_pTextureCom2->SetTexture(0);
            m_pRcTexCom->RenderBuffer();
        }

        else
        {
            m_strAnimKey;
            m_pTextureCom1->SetTexture(0);
            m_pRcTexCom->RenderBuffer();
        }
    }

    //m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, 0xffffffff);

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

    pComponent = m_pTransformCom2 = make_shared<CTransform>();
    m_pTransformCom2->ReadyTransform();
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform2",pComponent });

    // Animator
    pComponent = m_pAnimatorCom = make_shared<CAnimator>(m_pGraphicDev);
    m_pAnimatorCom->SetAnimKey(m_strAnimKey, 0.05f);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Animator", pComponent });

    // Texture
    pComponent = m_pTextureCom1 = make_shared<CTexture>(m_pGraphicDev);
    m_pTextureCom1->SetTextureKey(L"", TEX_NORMAL);
    m_mapComponent[ID_STATIC].insert({ L"Com_Texture1", pComponent });

    pComponent = m_pTextureCom2 = make_shared<CTexture>(m_pGraphicDev);
    m_pTextureCom2->SetTextureKey(L"", TEX_NORMAL);
    m_mapComponent[ID_STATIC].insert({ L"Com_Texture2", pComponent });
}

void CEffect::SetSkillEffect(tstring _strAnimKey, _vec2 _vTextureScale, _vec3* _vPos, const _vec3* _vScale, _float _fAnimTime)
{
    m_strAnimKey = _strAnimKey;
    m_fAnimTime = _fAnimTime;
    m_bLoop = false;

    m_vPos = _vPos;
    m_vScale = *_vScale;

    _float fTest = m_fAnimTime / CResourceMgr::GetInstance()->GetTexture(m_strAnimKey, TEX_NORMAL)->size();

    m_pAnimatorCom->SetAnimKey(m_strAnimKey, m_fAnimTime / CResourceMgr::GetInstance()->GetTexture(m_strAnimKey, TEX_NORMAL)->size());

    shared_ptr< tagTextureInfo> p = *CResourceMgr::GetInstance()->GetTexture(m_strAnimKey, TEX_NORMAL)->begin();
    _vec2 vSize = p->vImgSize;

    m_vScaleGap = { vSize.x / _vTextureScale.x, vSize.y / _vTextureScale.y, 1.f };
    m_vPosGap = { 0.f, 0.f, 0.f };

    m_bAnimation = true;
}

void CEffect::SetDamageEffect(_int _iDamage, _bool _bDamage, _vec3* _vPos, _float _fAnimTime)
{
    if (_iDamage < 10)
    {
        m_pTextureCom1->SetTextureKey(L"UI_Damage" + to_wstring(_iDamage), TEX_NORMAL);

        m_bTwoTexture = false;
    }
    
    else
    {
        m_pTextureCom1->SetTextureKey(L"UI_Damage" + to_wstring(_iDamage / 10), TEX_NORMAL);
        m_pTextureCom2->SetTextureKey(L"UI_Damage" + to_wstring(_iDamage % 10), TEX_NORMAL);

        m_bTwoTexture = true;
    }

    m_vPos = _vPos;
    m_vPosGap = { 0.f, 1.f, 0.f };
    m_vScale = { 0.5f, 0.5f, 0.5f };
    m_vScaleGap = { 1.f, 1.f, 1.f };
    m_bLoop = false;
    m_bAnimation = false;
    m_fAnimTime = _fAnimTime;

    SetMove(true, { 0.f, 1.5f, 0.f }, m_fAnimTime);

}

void CEffect::SetFontEffect(tstring _strAnimKey, _vec3* _vPos, const _vec3* _vScale, _float _fAnimTime)
{
    m_strAnimKey = _strAnimKey;
    
    m_pTextureCom1->SetTextureKey(m_strAnimKey, TEX_NORMAL);
    m_bTwoTexture = false;

    m_vPos = _vPos;
    m_vPosGap = { 0.f, 1.f, 0.f };
    m_vScale = { 1.f, 1.f, 1.f };
    m_vScaleGap = { 1.f, 1.f, 1.f };
    m_bLoop = false;
    m_bAnimation = true;
    m_fAnimTime = _fAnimTime;

    SetMove(true, { 0.f, 1.5f, 0.f }, m_fAnimTime);
}

void CEffect::SetHeadEffect(tstring _strAnimKey, _vec3* _vPos, const _vec3* _vScale, _float _fAnimTime, _bool _bLoop)
{
}

void CEffect::SetProjEffect(tstring _strAnimKey, _vec3 _vPos, const _vec3* _vScale, _float _fAnimTime)
{

    m_strAnimKey = _strAnimKey;
    m_fAnimTime = _fAnimTime;

    _float _fTest = m_fAnimTime / CResourceMgr::GetInstance()->GetTexture(m_strAnimKey, TEX_NORMAL)->size();
    m_pAnimatorCom->SetAnimKey(m_strAnimKey, m_fAnimTime / CResourceMgr::GetInstance()->GetTexture(m_strAnimKey, TEX_NORMAL)->size());
    m_bTwoTexture = false;

    m_vPos = &_vPos;
    m_vPosGap = { 0.f, 0.f, 0.f };
    m_vScale = { 10.f , 10.f, 10.f };
    //  m_vScale = *_vScale;
    m_vScaleGap = { 1.f, 1.f, 1.f };
    m_bLoop = false;
    m_bAnimation = true;

}

void CEffect::SetPos(_vec3* _vPos)
{
    m_vPos = _vPos;
}

void CEffect::SetScale(const _vec3* _vScale)
{
    m_vScale = *_vScale;
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

void CEffect::Reset()
{
    m_fDeltaTime = 0.f;
    m_vPos = nullptr;
    m_vScale = { 0.f, 0.f, 0.f };
    m_vPosGap = { 0.f, 0.f, 0.f };
    m_vScaleGap = { 0.f, 0.f, 0.f };

    m_strAnimKey = L"";
    m_fAnimTime = 0.f;
    m_bLoop = false;;
    m_iAlpha = 255;

    m_fAccumMoveTime = 0.f;
    m_fAccumFadeTime = 0.f;

    m_fMoveTime = 0.f;
    m_fFadeTime = 0.f;

    m_bMoving = false;
    m_bFading = false;

    m_vMoveDir = { 0.f, 0.f, 0.f };
    m_bAppear = false;
    m_bAnimation = false;
    m_bTwoTexture = false;
}
