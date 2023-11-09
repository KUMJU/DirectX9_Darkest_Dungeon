#include"pch.h"
#include "StatView.h"
#include"Export_Utility.h"
#include"Export_System.h"

CStatView::CStatView(LPDIRECT3DDEVICE9 _pGraphicDev)
	:CGameObject(_pGraphicDev)
{
}

CStatView::~CStatView()
{
	
}

HRESULT CStatView::ReadyGameObject()
{
	return S_OK;
}

_int CStatView::UpdateGameObject(const _float& fTimeDelta)
{
	ChangeActiveUI(fTimeDelta);
	__super::UpdateGameObject(fTimeDelta);

	
	return 0;
}

void CStatView::LateUpdateGameObject()
{
}

void CStatView::RenderGameObject()
{

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	
	if (m_bHero) {
		//Stress Render
		for (int i = 0; i < 10; ++i) {

			m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[i]->GetWorld());

			if (m_iStressNum > i) {
				m_pTextureCom[1]->SetTexture(0);
			}
			else {
				m_pTextureCom[0]->SetTexture(0);
			}

			m_pRCTexCom[i]->RenderBuffer();

		}
	}
	_int iTexNum = 2;

	if (m_bHpGap) {
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[26]->GetWorld());
		m_pTextureCom[18]->SetTexture(0);
		m_pRCTexCom[26]->RenderBuffer();
	}


	//HPBar Render
	for (int i = 10; i < 12; ++i) {
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[i]->GetWorld());
		m_pTextureCom[iTexNum]->SetTexture(0);
		m_pRCTexCom[i]->RenderBuffer();
		++iTexNum;
	}

	// CorpseHPBar Render
	if (m_bCorpse)
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[20]->GetWorld());
		m_pTextureCom[12]->SetTexture(0);
		m_pRCTexCom[20]->RenderBuffer();
	}

	// CursorUI Render
	if (m_bTurn && m_bHero)
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[21]->GetWorld());
		m_pTextureCom[13]->SetTexture(0);
		m_pRCTexCom[21]->RenderBuffer();
	}

	if (m_bTurn && !m_bHero)
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[22]->GetWorld());
		m_pTextureCom[14]->SetTexture(0);
		m_pRCTexCom[22]->RenderBuffer();
	}

	// TargetUI
	if (m_bTargetEnemy)
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[22]->GetWorld());
		m_pTextureCom[14]->SetTexture(0);
		m_pRCTexCom[22]->RenderBuffer();
	}

	if (m_bTargetEnemies)
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[23]->GetWorld());
		m_pTextureCom[15]->SetTexture(0);
		m_pRCTexCom[23]->RenderBuffer();
	}

	if (m_bTargetTeam)
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[24]->GetWorld());
		m_pTextureCom[16]->SetTexture(0);
		m_pRCTexCom[24]->RenderBuffer();
	}

	if (m_bTargetTeams)
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[25]->GetWorld());
		m_pTextureCom[17]->SetTexture(0);
		m_pRCTexCom[25]->RenderBuffer();
	}

	// Turn
	if (!m_bTurnOff)
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[26]->GetWorld());
		m_pTextureCom[3]->SetTexture(0);
		m_pRCTexCom[26]->RenderBuffer();
	}

	//TCHAR buf[64];
	//// 壶唱皑
	//_vec2 vPos = { m_vPos.x + WINCX * 0.5f - 175.f, (m_vPos.y * -1.f) + WINCY * 0.5f + 31.f };
	//tstring m_strHeroName = L"壶唱皑";
	//Engine::Render_Font(L"Font_Point_Small", m_strHeroName.c_str(), &vPos, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
	//if (m_bTurn && !m_bHero)
	//{
	//	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[22]->GetWorld());
	//	m_pTextureCom[14]->SetTexture(0);
	//	m_pRCTexCom[22]->RenderBuffer();
	//}

	//Battle Attribute Render

	iTexNum = 4;
	_int iArrNum = 12;

	for (int i = 0; i < 8; ++i) {

		if (true == m_bAttributeArr[i]) {
			m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[iArrNum]->GetWorld());
			m_pTextureCom[iTexNum]->SetTexture(0);
			m_pRCTexCom[iArrNum]->RenderBuffer();

			++iArrNum;
		}
		++iTexNum;

	}


	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);


}

void CStatView::AddComponent()
{
	for (int i = 0; i < 27; ++i) {

		shared_ptr<CComponent> pComponent;

		//Transform 积己
		pComponent = m_pTransformCom[i] = make_shared<CTransform>();
		m_pTransformCom[i]->ReadyTransform();

		tstring strCurNum = to_wstring(i);
		tstring strComName = L"Com_Transform" + strCurNum;
		tstring strComName2 = L"Com_RcTex" + strCurNum;

		m_mapComponent[ID_DYNAMIC].insert({ strComName ,pComponent });
		m_pTransformCom[i]->SetScale(1.f, 1.f, 1.f);
		m_pTransformCom[i]->SetPosition(m_vPos.x, m_vPos.y, m_vPos.z);

		//RcTex积己
		pComponent = m_pRCTexCom[i] = make_shared<CRcTex>(m_pGraphicDev);
		m_pRCTexCom[i]->ReadyBuffer();
		m_mapComponent[ID_STATIC].insert({strComName2,pComponent });

	}


	for (int i = 0; i < 19; ++i) {

		tstring strCurNum = to_wstring(i);
		tstring strComName = L"Com_Texture" + strCurNum;

		shared_ptr<CComponent> pComponent;
		pComponent = m_pTextureCom[i] = make_shared<CTexture>(m_pGraphicDev);
		m_mapComponent[ID_STATIC].insert({ strComName,pComponent });

	}

	{
		//ImageSet
		m_pTextureCom[0]->SetTextureKey(L"Stat_StressEmpty", TEXTUREID::TEX_NORMAL);
		m_pTextureCom[1]->SetTextureKey(L"Stat_StressFull", TEXTUREID::TEX_NORMAL);
		m_pTextureCom[2]->SetTextureKey(L"Stat_HealthEmpty", TEXTUREID::TEX_NORMAL);
		m_pTextureCom[3]->SetTextureKey(L"Stat_HealthFull", TEXTUREID::TEX_NORMAL);
		m_pTextureCom[4]->SetTextureKey(L"Stat_Attribute_Poison", TEXTUREID::TEX_NORMAL);
		m_pTextureCom[5]->SetTextureKey(L"Stat_Attribute_Bleed", TEXTUREID::TEX_NORMAL);
		m_pTextureCom[6]->SetTextureKey(L"Stat_Attribute_Stun", TEXTUREID::TEX_NORMAL);
		m_pTextureCom[7]->SetTextureKey(L"Stat_Attribute_Virtue", TEXTUREID::TEX_NORMAL);
		m_pTextureCom[8]->SetTextureKey(L"Stat_Attribute_Stress", TEXTUREID::TEX_NORMAL);
		m_pTextureCom[9]->SetTextureKey(L"Stat_Attribute_Buff", TEXTUREID::TEX_NORMAL);
		m_pTextureCom[10]->SetTextureKey(L"Stat_Attribute_Debuff", TEXTUREID::TEX_NORMAL);
		m_pTextureCom[11]->SetTextureKey(L"Stat_Attribute_Death", TEXTUREID::TEX_NORMAL);
		m_pTextureCom[12]->SetTextureKey(L"Stat_CorpseFull", TEXTUREID::TEX_NORMAL);
		m_pTextureCom[13]->SetTextureKey(L"Target_TurnCreature", TEXTUREID::TEX_NORMAL);
		m_pTextureCom[14]->SetTextureKey(L"Target_AttackCreature", TEXTUREID::TEX_NORMAL);
		m_pTextureCom[15]->SetTextureKey(L"Target_EnemyPlus", TEXTUREID::TEX_NORMAL);
		m_pTextureCom[16]->SetTextureKey(L"Target_TeamCreature", TEXTUREID::TEX_NORMAL);
		m_pTextureCom[17]->SetTextureKey(L"Target_TeamPlus", TEXTUREID::TEX_NORMAL);
		m_pTextureCom[18]->SetTextureKey(L"Stat_HealthGap", TEXTUREID::TEX_NORMAL);


	}	
}

void CStatView::ChangeActiveUI(_float fTimeDelta)
{
	m_fHpRegemTime += fTimeDelta;
	if (m_fHpRegemTime < 0.06f)
		return;


	if (m_bHpGap) {
		 
		m_iGapHp -= 0.001f * fTimeDelta * 0.5f;
		m_fHpGapRange = static_cast<float>(m_iGapHp) / static_cast<float>(m_iMaxHp);
		if (m_iGapHp <= m_iHp) {
			m_bHpGap = false;
		}
	}

	m_fHpRegemTime = 0.f;

}



void CStatView::SettingInit(_vec3 _vCreaturePos, _int _iHp, _int _iMaxHp , _bool _isHero)
{
	m_bHero = _isHero;
	m_iHp = _iHp;
	m_iMaxHp = _iMaxHp;
	m_fHpBarRange = static_cast<float>(m_iHp / m_iMaxHp);

	SettingPos(_vCreaturePos, true);

}

void CStatView::SettingPos(_vec3 _vPos, _bool _bFront)
{

	_vec3 CurPos = { 0.f, 4.f , 0.f };
	m_vCenterPos = _vPos + CurPos;
	_float iInitPos = -0.8f;

	_float fFrontGap = 0.01f;
	if (_bFront)
	{
		fFrontGap = 0.01f;
	}
	else
		fFrontGap = -0.01f;

	if (m_bHero) {
		//Setting Stress Info
		for (int i = 0; i < 10; ++i) {
			m_pTransformCom[i]->SetPosition(m_vCenterPos.x + iInitPos, m_vCenterPos.y, m_vCenterPos.z);
			m_pTransformCom[i]->SetAngle({ 0.f, 0.f, 0.f });
			m_pTransformCom[i]->SetScale(0.1f, 0.1f, 1.f);
			//Scale
			iInitPos += 0.2f;
		}
	}

	iInitPos = -1.f;

	//Empty HpBar
	for (int i = 10; i < 11; ++i) {
		m_pTransformCom[i]->SetPosition(m_vCenterPos.x + 0.1f, m_vCenterPos.y + 0.3f, m_vCenterPos.z);
		m_pTransformCom[i]->SetAngle({ 0.f, 0.f, 0.f });
		m_pTransformCom[i]->SetScale(1.f, 0.1f, 1.f);
	}

	//Full HpBar
	if (!m_bCorpse)
	{
		for (int i = 11; i < 12; ++i) {
			m_pTransformCom[i]->SetPosition(m_vCenterPos.x + (-1.f + m_fHpBarRange) + 0.1f * m_fHpBarRange, m_vCenterPos.y + 0.3f, m_vCenterPos.z - fFrontGap * 2);
			m_pTransformCom[i]->SetAngle({ 0.f, 0.f, 0.f });
			m_pTransformCom[i]->SetScale(1.f * m_fHpBarRange, 0.1f, 1.f);
		}
	}
	//Full CorpseHpBar
	for (int i = 20; i < 21; ++i) {
		m_pTransformCom[i]->SetPosition(m_vCenterPos.x + (-1.f + m_fHpBarRange) + 0.1f * m_fHpBarRange, m_vCenterPos.y + 0.3f, m_vCenterPos.z - fFrontGap);
		m_pTransformCom[i]->SetAngle({ 0.f, 0.f, 0.f });
		m_pTransformCom[i]->SetScale(1.f * m_fHpBarRange, 0.1f, 1.f);
	}

	// Target Cursor
	for (int i = 21; i < 23; ++i) {
		m_pTransformCom[i]->SetPosition(m_vCenterPos.x + 0.1f, m_vCenterPos.y + 1.4f, m_vCenterPos.z);
		m_pTransformCom[i]->SetAngle({ 0.f, 0.f, 0.f });
		m_pTransformCom[i]->SetScale(1.5f, 1.f, 1.f);
	}

	for (int i = 24; i < 25; ++i) {
		m_pTransformCom[i]->SetPosition(m_vCenterPos.x + 0.1f, m_vCenterPos.y + 1.4f, m_vCenterPos.z);
		m_pTransformCom[i]->SetAngle({ 0.f, 0.f, 0.f });
		m_pTransformCom[i]->SetScale(1.5f, 1.f, 1.f);
	}

	// Target Plus
	for (int i = 23; i < 24; ++i) {
		m_pTransformCom[i]->SetPosition(m_vCenterPos.x + 1.5f, m_vCenterPos.y + 0.4f, m_vCenterPos.z);
		m_pTransformCom[i]->SetAngle({ 0.f, 0.f, 0.f });
		m_pTransformCom[i]->SetScale(0.8f, 0.8f, 1.f);
	}

	for (int i = 25; i < 26; ++i) {
		m_pTransformCom[i]->SetPosition(m_vCenterPos.x + 1.5f, m_vCenterPos.y + 0.4f, m_vCenterPos.z);
		m_pTransformCom[i]->SetAngle({ 0.f, 0.f, 0.f });
		m_pTransformCom[i]->SetScale(0.8f, 0.8f, 1.f);
	}

	// Turn
	for (int i = 26; i < 27; ++i) {
		m_pTransformCom[i]->SetPosition(m_vCenterPos.x + 1.25f, m_vCenterPos.y + 0.35f, m_vCenterPos.z);
		m_pTransformCom[i]->SetAngle({ 0.f, 0.f, 0.f });
		m_pTransformCom[i]->SetScale(0.05f, 0.2f, 1.f);
	}
	
	iInitPos = -0.8f;

	//Battle Attribute
	for (int i = 12; i < 20; ++i) {
		m_pTransformCom[i]->SetPosition(m_vCenterPos.x + iInitPos, m_vCenterPos.y + 0.6f, m_vCenterPos.z);
		m_pTransformCom[i]->SetAngle({ 0.f, 0.f, 0.f });
		m_pTransformCom[i]->SetScale(0.2f, 0.2f, 1.f);
		iInitPos += 0.3f;
	}
	//HPGap
	for (int i = 26; i < 27; ++i) {
		m_pTransformCom[i]->SetPosition(m_vCenterPos.x + (-1.f + m_fHpGapRange) + 0.1f * m_fHpGapRange, m_vCenterPos.y + 0.3f, m_vCenterPos.z - fFrontGap);
		m_pTransformCom[i]->SetAngle({ 0.f, 0.f, 0.f });
		m_pTransformCom[i]->SetScale(1.f * m_fHpGapRange, 0.1f, 1.f);
	}

}                  

void CStatView::SetStress(int _iStress)
{
	m_iStress = _iStress;

	if (m_iStress < 0) {
		m_iStress = 0;
	}
	m_iStressNum =  (int)(m_iStress / 10);
}

void CStatView::SetHp(int _iHp)
{
	//Stat_HealthGap

	if (_iHp != m_iHp && _iHp < m_iHp) {
		m_fHpGapRange = static_cast<float>(m_iHp) / static_cast<float>(m_iMaxHp);
		m_iGapHp = m_iHp;
		m_bHpGap = true;
	}

	m_iHp = _iHp;

	if (m_iHp < 0) {
		m_iHp = 0;
	}
	m_fHpBarRange = static_cast<float>(m_iHp) / static_cast<float>(m_iMaxHp);
}

