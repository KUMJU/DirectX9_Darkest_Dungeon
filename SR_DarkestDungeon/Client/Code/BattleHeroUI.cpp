#include"pch.h"
#include "BattleHeroUI.h"
#include"Export_Utility.h"
#include"Export_System.h"
#include"Skill.h"

CBattleHeroUI::CBattleHeroUI(LPDIRECT3DDEVICE9 _pGraphicDev)
    :CUIObj(_pGraphicDev)
{
}

CBattleHeroUI::~CBattleHeroUI()
{
}

HRESULT CBattleHeroUI::ReadyGameObject()
{
    m_vSize = { 256.5f, 51.f , 0.f};
    m_vAngle = { 0.f, 0.f, 0.f };
    m_vPos = { -250.f, -250.f, 0.f };

    m_bVisible = true;
    m_bEnable = true;
    m_bActive = true;


    m_pTransCom->SetScale(m_vSize.x, m_vSize.y, m_vSize.z);
    m_pTransCom->SetPosition(m_vPos.x, m_vPos.y, m_vPos.z);
    m_pTransCom->SetAngle(m_vAngle);

    CUIObj::ReadyGameObject();

    SetIconPos();

    return S_OK;
}

_int CBattleHeroUI::UpdateGameObject(const _float& fTimeDelta)
{
    if (!m_bVisible)
        return 0;


    int iExit(0);
    CGameObject::UpdateGameObject(fTimeDelta);
    AddRenderGroup(RENDER_UI, shared_from_this());

    return iExit;
}

void CBattleHeroUI::LateUpdateGameObject()
{

}

void CBattleHeroUI::RenderGameObject()
{
    if (!m_bVisible)
        return;

    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->GetWorld());
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
    m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, FALSE);

    m_pTextureCom->SetTexture(0);
    m_pRcTexCom->RenderBuffer();

    TCHAR buf[64];

    //Hp

    _vec2 vPos = { m_vPos.x + WINCX *0.5f - 200.f, (m_vPos.y * -1.f) + WINCY * 0.5f};
    

    if (m_iHP < 10) {
        _stprintf_s(buf, TEXT("    %d"), m_iHP);

    }
    else if (m_iHP < 100) {
        _stprintf_s(buf, TEXT("  %d"), m_iHP);
    }

    Engine::Render_Font(L"Font_Default_Small", buf, &vPos, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

    vPos = { m_vPos.x + WINCX * 0.5f - 180.f, (m_vPos.y * -1.f) + WINCY * 0.5f };
    _stprintf_s(buf, TEXT(" /"));
    Engine::Render_Font(L"Font_Default_Small", buf, &vPos, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

   vPos = { m_vPos.x + WINCX * 0.5f - 170.f, (m_vPos.y * -1.f) + WINCY * 0.5f };
   _stprintf_s(buf, TEXT("%d"), m_iMaxHP);
   Engine::Render_Font(L"Font_Default_Small", buf, &vPos, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));


   //Stress

   vPos = { m_vPos.x + WINCX * 0.5f - 200.f, (m_vPos.y * -1.f) + WINCY * 0.5f +20.f };

   //자릿수 띄워주기
   if (m_iStress < 10) {
       _stprintf_s(buf, TEXT("    %d"), m_iStress);

   }
   else if (m_iStress < 100) {
       _stprintf_s(buf, TEXT("  %d"), m_iStress);
   }

   Engine::Render_Font(L"Font_Default_Small", buf, &vPos, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

   vPos = { m_vPos.x + WINCX * 0.5f - 180.f, (m_vPos.y * -1.f) + WINCY * 0.5f + 20.f };
   _stprintf_s(buf, TEXT(" /"));
   Engine::Render_Font(L"Font_Default_Small", buf, &vPos, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

   vPos = { m_vPos.x + WINCX * 0.5f - 170.f, (m_vPos.y * -1.f) + WINCY * 0.5f + 20.f };
   _stprintf_s(buf, TEXT("%d"), 100);
   Engine::Render_Font(L"Font_Default_Small", buf, &vPos, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

   //HeroName

   vPos = { m_vPos.x + WINCX * 0.5f - 238.f, (m_vPos.y * -1.f) + WINCY * 0.5f - 30.f };

   if (L"VESTAL" == m_strHeroName || L"JESTER" == m_strHeroName) {
       Engine::Render_Font(L"Font_Point", m_strHeroName.c_str(), &vPos, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
   }
   else {
       Engine::Render_Font(L"Font_Point_Small", m_strHeroName.c_str(), &vPos, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
   }

   if (m_pSkillVec) {
       for (int i = 0; i < 6; ++i) {
           m_pGraphicDev->SetTransform(D3DTS_WORLD, m_arrSkillTransform[i]->GetWorld());
           m_arrSkillTexture[i]->SetTexture(0);
           m_arrSkillRcTex[i]->RenderBuffer();
       }
   }

    m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
    m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CBattleHeroUI::SettingHeroInfo(_int _iMaxHP, _int _iHP, _int _iStress, tstring _strHeroName, vector<shared_ptr<CSkill>>* _vecSkill)
{
    m_strHeroName = _strHeroName;
    m_iMaxHP = _iMaxHP;
    m_iHP = _iHP;
    m_iStress = _iStress;
    m_pSkillVec = _vecSkill;

    for (int i = 0; i < 4; ++i) {
        m_arrSkillTexture[i]->SetTextureKey(m_pSkillVec->at(i)->GetImgKey(), TEX_NORMAL);
    }
}

void CBattleHeroUI::PickingUI(LONG _fX, LONG _fY)
{
    //Y축검사
    if (_fY > (m_vPos.y * -1.f) + WINCY * 0.5f - 5.f || _fY < (m_vPos.y * -1.f) + WINCY * 0.5f - 60.f)
        return;


    if (m_vPos.x + WINCX * 0.5f- 80.f < _fX && m_vPos.x + WINCX * 0.5f - 80.f + 57.f >= _fX) {
        //Skill1 버튼 클릭 트리거

    }else if (m_vPos.x + WINCX * 0.5f - 80.f + 57.f < _fX && m_vPos.x + WINCX * 0.5f - 80.f + 57.f * 2 >= _fX) {
        //Skill2 버튼 클릭 트리거

    }else if (m_vPos.x + WINCX * 0.5f - 80.f + 57.f * 2 < _fX && m_vPos.x + WINCX * 0.5f - 80.f + 57.f * 3 >= _fX) {
        //Skill3 버튼 클릭 트리거

    }else if (m_vPos.x + WINCX * 0.5f - 80.f + 57.f * 3 < _fX && m_vPos.x + WINCX * 0.5f - 80.f + 57.f * 4 >= _fX) {
        //Skill4 버튼 클릭 트리거

    }else if (m_vPos.x + WINCX * 0.5f - 80.f + 57.f * 4 < _fX && m_vPos.x + WINCX * 0.5f - 80.f + 57.f * 5 >= _fX) {

        //자리 스왑 버튼 클릭 트리거 
    }
    else if (m_vPos.x + WINCX * 0.5f - 80.f + 57.f * 5 < _fX && m_vPos.x + WINCX * 0.5f - 120.f + 57.f * 6 >= _fX) {
        //행동 취소(턴 넘기기) 버튼 클릭 트리거

    }




}

void CBattleHeroUI::AddComponent()
{
    shared_ptr<CComponent> pComponent;

    CUIObj::AddComponent();

    pComponent = m_pTextureCom = make_shared<CTexture>(m_pGraphicDev);
    m_pTextureCom->SetTextureKey(L"UI_Battle_HeroPanel", TEX_NORMAL);
    m_mapComponent[ID_STATIC].insert({ L"Com_Texture",pComponent });

    //Skill Icon

    for (int i = 0; i < 6; ++i) {

        shared_ptr<CComponent> pComponent;

        //Transform 생성
        pComponent = m_arrSkillTransform[i] = make_shared<CTransform>();
        m_arrSkillTransform[i]->ReadyTransform();

        tstring strCurNum = to_wstring(i);
        tstring strComName = L"Com_Transform" + strCurNum;
        tstring strComName2 = L"Com_RcTex" + strCurNum;
        tstring strComName3 = L"Com_Texture" + strCurNum;


        m_mapComponent[ID_DYNAMIC].insert({ strComName ,pComponent });

        if(i == 5)
            m_arrSkillTransform[i]->SetScale(8.f, 30.f, 1.f);
        else
            m_arrSkillTransform[i]->SetScale(28.f, 28.f, 1.f);

        //RcTex생성
        pComponent = m_arrSkillRcTex[i] = make_shared<CRcTex>(m_pGraphicDev);
        m_arrSkillRcTex[i]->ReadyBuffer();
        m_mapComponent[ID_STATIC].insert({ strComName2,pComponent });

        //Texture 생성

        pComponent = m_arrSkillTexture[i] = make_shared<CTexture>(m_pGraphicDev);
        m_mapComponent[ID_STATIC].insert({ strComName3,pComponent });

    }

    m_arrSkillTexture[4]->SetTextureKey(L"UI_ability_move", TEX_NORMAL);
    m_arrSkillTexture[5]->SetTextureKey(L"UI_ability_pass", TEX_NORMAL);



}

void CBattleHeroUI::SetIconPos()
{
    for (int i = 0; i < 5; ++i) {
        m_arrSkillTransform[i]->SetPosition(m_vPos.x- 45.f + 57.f * i, m_vPos.y, m_vPos.z);
    }

    m_arrSkillTransform[5]->SetPosition(m_vPos.x + 220.f, m_vPos.y, m_vPos.z);

}
