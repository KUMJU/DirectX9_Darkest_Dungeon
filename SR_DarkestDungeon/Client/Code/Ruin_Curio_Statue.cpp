#include "pch.h"
#include "Ruin_Curio_Statue.h"
#include "Export_Utility.h"

#include"Item.h"

#include"GameMgr.h"
#include "Player.h"

CRuin_Curio_Statue::CRuin_Curio_Statue(LPDIRECT3DDEVICE9 _pGraphicDev, StatueState _eAnswerState)
	:CInteractionObj(_pGraphicDev), m_eAnswerState(_eAnswerState)
{
}

CRuin_Curio_Statue::~CRuin_Curio_Statue()
{
}

HRESULT CRuin_Curio_Statue::ReadyGameObject()
{
	__super::ReadyGameObject();

	m_bInteractionKey = L"C";
	m_bInteractionInfo = L"º¸¼® »ðÀÔÇÏ±â";

    return S_OK;
}

_int CRuin_Curio_Statue::UpdateGameObject(const _float& fTimeDelta)
{
	_int iExit = CInteractionObj::UpdateGameObject(fTimeDelta);

	if (m_bDebounce) {

		m_fDebounceTime += fTimeDelta;

		if (m_fDebounceTime > 0.5f) {
			m_bDebounce = false;
			m_fDebounceTime = 0.f;
		}

	}


	_matrix matWorld;

	matWorld = *m_pTransformCom->GetWorld();
	SetBillBoard(matWorld);
	m_pTransformCom->SetWorld(&matWorld);

    return iExit;
}

void CRuin_Curio_Statue::LateUpdateGameObject()
{
}

void CRuin_Curio_Statue::RenderGameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->GetWorld());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->SetTexture(static_cast<_uint>(m_eCurrentState));
	m_pBufferCom->RenderBuffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CRuin_Curio_Statue::AddComponent()
{
	shared_ptr<CComponent> pComponent;

	pComponent = m_pBufferCom = make_shared<CRcTex>(m_pGraphicDev);
	dynamic_pointer_cast<CRcTex>(m_pBufferCom)->ReadyBuffer();
	m_mapComponent[ID_STATIC].insert({ L"Com_RcTex", pComponent });

	pComponent = m_pTextureCom = make_shared<CTexture>(m_pGraphicDev);
	m_mapComponent[ID_STATIC].insert({ L"Com_Texture", pComponent });
	m_pTextureCom->SetTextureKey(L"Ruin_OldOne_Statue", TEXTUREID::TEX_NORMAL);

	pComponent = m_pTransformCom = make_shared<CTransform>();
	m_pTransformCom->ReadyTransform();
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });
	m_pTransformCom->SetPosition(m_vPos.x, m_vPos.y, m_vPos.z);
	m_pTransformCom->SetScale(m_vScale.x, m_vScale.y, m_vScale.x);

}

void CRuin_Curio_Statue::GetInteractionKey(const _float& fTimeDelta)
{

	if (GetAsyncKeyState('C') & 0x8000)
	{
		if (m_bTabInteractionKey)
			return;


		if (m_bDebounce) 
			return;
		

		m_bActive = true;

		Interaction();
	}

}

void CRuin_Curio_Statue::Interaction()
{

	if (StatueState::STATUE_RED == m_eCurrentState || StatueState::STATUE_BLUE == m_eCurrentState || StatueState::STATUE_GREEN == m_eCurrentState) {

		tstring strUIKeyName;

		if (StatueState::STATUE_RED == m_eCurrentState) {
			strUIKeyName = L"Player_Item_RedGem";
		}
		else if (StatueState::STATUE_BLUE == m_eCurrentState) {
			strUIKeyName = L"Player_Item_BlueGem";
		}
		else if (StatueState::STATUE_GREEN == m_eCurrentState) {
			strUIKeyName = L"Player_Item_GreenGem";

		}

		shared_ptr<CItem> pItem = make_shared<CItem>(m_pGraphicDev);
		pItem->GetUITextureKeyName(strUIKeyName);
		pItem->SetOnField(false);

		dynamic_pointer_cast<CPlayer>(m_pPlayer)->InsertItem(pItem);
		m_eCurrentState = StatueState::STATUE_EMPTY;

		m_bInteractionKey = L"C";
		m_bInteractionInfo = L"º¸¼® »ðÀÔÇÏ±â";

	}
	

	if (m_pPlayer) {

		EHandItem eItem = dynamic_pointer_cast<CPlayer>(m_pPlayer)->GetCurrentItem();

		if (EHandItem::RED_GEM == eItem) {
			m_eCurrentState = StatueState::STATUE_RED;
			dynamic_pointer_cast<CPlayer>(m_pPlayer)->SetCurrentItem(EHandItem::ENUM_END);
			dynamic_pointer_cast<CPlayer>(m_pPlayer)->DeleteItem(L"Item_UI_RedGem");

			m_bInteractionKey = L"C";
			m_bInteractionInfo = L"º¸¼® »©±â";

		}
		else if (EHandItem::GREEN_GEM == eItem) {
			m_eCurrentState = StatueState::STATUE_GREEN;
			dynamic_pointer_cast<CPlayer>(m_pPlayer)->SetCurrentItem(EHandItem::ENUM_END);
			dynamic_pointer_cast<CPlayer>(m_pPlayer)->DeleteItem(L"Item_UI_GreenGem");

			m_bInteractionKey = L"C";
			m_bInteractionInfo = L"º¸¼® »©±â";
		}
		else if (EHandItem::BLUE_GEM == eItem) {
			m_eCurrentState = StatueState::STATUE_BLUE;
			dynamic_pointer_cast<CPlayer>(m_pPlayer)->SetCurrentItem(EHandItem::ENUM_END);
			dynamic_pointer_cast<CPlayer>(m_pPlayer)->DeleteItem(L"Item_UI_BlueGem");

			m_bInteractionKey = L"C";
			m_bInteractionInfo = L"º¸¼® »©±â";
		}


		if (m_eCurrentState == m_eAnswerState) {

			m_bIsAnswer = true;

		}

	}
	

	m_bDebounce = true;
}

_bool CRuin_Curio_Statue::IsFinish()
{
	return _bool();
}

void CRuin_Curio_Statue::ChangeTexture()
{
}
