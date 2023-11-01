#include"pch.h"
#include "HeroStat.h"
#include"Hero.h"

#include"Export_Utility.h"
#include"Export_System.h"

CHeroStat::CHeroStat(LPDIRECT3DDEVICE9 _pGraphicDev)
    :Engine::CUIObj(_pGraphicDev)
{
}

CHeroStat::~CHeroStat()
{
}

HRESULT CHeroStat::ReadyGameObject()
{
    m_vSize = { 345.f , 338.f ,0.f };
    m_vAngle = { 0.f, 0.f, 0.f };
    m_vPos = { 0.f, 0.f, 0.5f };

    m_bVisible = false;
    m_bEnable = true;
    m_bActive = true;

    {
        m_pTransformCom[0]->SetScale(m_vSize.x, m_vSize.y, m_vSize.z );
        m_pTransformCom[0]->SetPosition(m_vPos.x, m_vPos.y, m_vPos.z);
            
        // 추천 위치
        for (int i = 1; i < 5; ++i)
        {
            m_pTransformCom[i]->SetScale(25.f, 25.f, 1.f);
            m_pTransformCom[i]->SetPosition(-128.f + 47.f * i, 67.f, m_vPos.z);
        }

        // 추천 목표
        for (int i = 5; i < 9; ++i)
        {
            m_pTransformCom[i]->SetScale(25.f, 25.f, 1.f);
            m_pTransformCom[i]->SetPosition(-104.f + 47.f * i, 67.f, m_vPos.z);
        }

        // 스킬 이미지
        for (int i = 9; i < 15; ++i)
        {
            m_pTransformCom[i]->SetScale(33.f, 33.f, 1.f);
            m_pTransformCom[i]->SetPosition(-81.f + 71.f * (i - 9), -10.f, m_vPos.z);
        }

        // 활성 스킬 표시
        for (int i = 15; i < 19; ++i)
        {
            m_pTransformCom[i]->SetScale(38.f, 38.f, 1.f);
            m_pTransformCom[i]->SetPosition(-81.f + 71.f * (i - 15), -10.f, m_vPos.z);
        }
    }

    CUIObj::ReadyGameObject();

    return S_OK;
}

_int CHeroStat::UpdateGameObject(const _float& fTimeDelta)
{
    int iExit(0);

    if (m_pHero.lock() && m_bVisible)
    {
        /*_vec3 vHeroPos = m_pHero.lock()->GetPos();
        m_vPos = { vHeroPos.x + 50.f, vHeroPos.y, vHeroPos.z };*/

        iExit = CGameObject::UpdateGameObject(fTimeDelta);
        AddRenderGroup(RENDER_UI, shared_from_this());

        if (GetAsyncKeyState('X') & 0x8000)
        {
            m_bVisible = false;

            // 플레이어 행동 풀기
            CGameMgr::GetInstance()->SetGameState(EGameState::PRCESS);
        }
    }

    return iExit;
}

void CHeroStat::LateUpdateGameObject()
{
    if (m_pHero.lock() && m_bVisible)
    {
        __super::LateUpdateGameObject();
    }
}

void CHeroStat::RenderGameObject()
{

    if (m_pHero.lock() && m_bVisible)
    {
        m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
        m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
        m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
        m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
        m_pGraphicDev->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);

        m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[0]->GetWorld());
        m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
        m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
        m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, FALSE);

        m_pTextureCom[0]->SetTexture(0);
        m_pRCTexCom[0]->RenderBuffer();

        // 추천 위치
        for (int i = 1; i < 5; ++i)
        {
            m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[i]->GetWorld());

            m_pTextureCom[i]->SetTexture(0);
            m_pRCTexCom[i]->RenderBuffer();
        }

        // 추천 목표
        for (int i = 5; i < 9; ++i)
        {
            m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[i]->GetWorld());

            m_pTextureCom[i]->SetTexture(0);
            m_pRCTexCom[i]->RenderBuffer();
        }

        // 스킬 이미지
        for (int i = 9; i < 15; ++i)
        {
            m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[i]->GetWorld());

            m_pTextureCom[i]->SetTexture(0);
            m_pRCTexCom[i]->RenderBuffer();
        }

        // 활성 스킬 표시
        for (int i = 15; i < 19; ++i)
        {
            m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[i]->GetWorld());

            m_pTextureCom[15]->SetTexture(0);
            m_pRCTexCom[i]->RenderBuffer();
        }

        _int iCount = 0;

        m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
        m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
        m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

        m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    }
}


void CHeroStat::PickingUI(LONG _fX, LONG _fY)
{


}

void CHeroStat::AddComponent()
{
    shared_ptr<CComponent> pComponent;

    //CUIObj::AddComponent();

    switch (m_pHero.lock()->GetHeroType())
    {
    case EHeroType::SHILEDBREAKER:
        m_strBackKey = L"UI_HeroStat_Shieldbreaker_Base";
        break;

    case EHeroType::HIGHWAYMAN:
        m_strBackKey = L"UI_HeroStat_Highwayman_Base";
        break;

    case EHeroType::JESTER:
        m_strBackKey = L"UI_HeroStat_Jester_Base";
        break;

    case EHeroType::VESTAL:
        m_strBackKey = L"UI_HeroStat_Vestal_Base";
        break;

    case EHeroType::ENUM_END:
        m_strBackKey = L"UI_HeroStat_Base";
        break;

    default:
        m_strBackKey = L"UI_HeroStat_Base";
        break;
    }

    for (int i = 0; i < 19; ++i) {

        shared_ptr<CComponent> pComponent;

        //Transform 생성
        pComponent = m_pTransformCom[i] = make_shared<CTransform>();
        m_pTransformCom[i]->ReadyTransform();

        tstring strCurNum = to_wstring(i);
        tstring strComName = L"Com_Transform" + strCurNum;
        tstring strComName2 = L"Com_RcTex" + strCurNum;

        m_mapComponent[ID_DYNAMIC].insert({ strComName ,pComponent });
        m_pTransformCom[i]->SetScale(1.f, 1.f, 1.f);
        m_pTransformCom[i]->SetPosition(m_vPos.x, m_vPos.y, m_vPos.z);

        //RcTex생성
        pComponent = m_pRCTexCom[i] = make_shared<CRcTex>(m_pGraphicDev);
        m_pRCTexCom[i]->ReadyBuffer();
        m_mapComponent[ID_STATIC].insert({ strComName2,pComponent });

    }


    for (int i = 0; i < 17; ++i) {

        tstring strCurNum = to_wstring(i);
        tstring strComName = L"Com_Texture" + strCurNum;

        shared_ptr<CComponent> pComponent;
        pComponent = m_pTextureCom[i] = make_shared<CTexture>(m_pGraphicDev);
        m_mapComponent[ID_STATIC].insert({ strComName,pComponent });

    }

    m_pVecSkill = m_pHero.lock()->GetSkillVector();

    {
        /*
        // 0: 틀
        // 1~4 : 추천 위치1~4
        // 5~8 : 추천 목표1~4
        // 9~14: 스킬 이미지
        // 15 : 스킬 활성 표시
        */

        //ImageSet
        m_pTextureCom[0]->SetTextureKey(m_strBackKey, TEXTUREID::TEX_NORMAL);
        
        m_pTextureCom[1]->SetTextureKey(L"UI_HeroStat_Recommend_Position_1", TEXTUREID::TEX_NORMAL);
        m_pTextureCom[2]->SetTextureKey(L"UI_HeroStat_Recommend_Position_2", TEXTUREID::TEX_NORMAL);
        m_pTextureCom[3]->SetTextureKey(L"UI_HeroStat_Recommend_Position_3", TEXTUREID::TEX_NORMAL);
        m_pTextureCom[4]->SetTextureKey(L"UI_HeroStat_Recommend_Position_4", TEXTUREID::TEX_NORMAL);
        
        m_pTextureCom[5]->SetTextureKey(L"UI_HeroStat_Recommend_Target_1", TEXTUREID::TEX_NORMAL);
        m_pTextureCom[6]->SetTextureKey(L"UI_HeroStat_Recommend_Target_2", TEXTUREID::TEX_NORMAL);
        m_pTextureCom[7]->SetTextureKey(L"UI_HeroStat_Recommend_Target_3", TEXTUREID::TEX_NORMAL);
        m_pTextureCom[8]->SetTextureKey(L"UI_HeroStat_Recommend_Target_4", TEXTUREID::TEX_NORMAL);
        
        m_pTextureCom[9]->SetTextureKey((*m_pVecSkill)[0]->GetImgKey(), TEXTUREID::TEX_NORMAL);
        m_pTextureCom[10]->SetTextureKey((*m_pVecSkill)[1]->GetImgKey(), TEXTUREID::TEX_NORMAL);
        m_pTextureCom[11]->SetTextureKey((*m_pVecSkill)[2]->GetImgKey(), TEXTUREID::TEX_NORMAL);
        m_pTextureCom[12]->SetTextureKey((*m_pVecSkill)[3]->GetImgKey(), TEXTUREID::TEX_NORMAL);
        m_pTextureCom[13]->SetTextureKey((*m_pVecSkill)[4]->GetImgKey(), TEXTUREID::TEX_NORMAL);
        m_pTextureCom[14]->SetTextureKey((*m_pVecSkill)[5]->GetImgKey(), TEXTUREID::TEX_NORMAL);
        
        m_pTextureCom[15]->SetTextureKey(L"UI_HeroStat_Selected_Skill", TEXTUREID::TEX_NORMAL);
        m_pTextureCom[16]->SetTextureKey(L"UI_HeroStat_Locked_Skill", TEXTUREID::TEX_NORMAL);
    }
}
