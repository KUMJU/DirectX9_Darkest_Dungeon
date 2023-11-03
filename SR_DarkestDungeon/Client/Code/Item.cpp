#include"pch.h"
#include"Item.h"
#include"Export_Utility.h"
#include"CameraMgr.h"
#include"Export_System.h"
#include"PickingMgr.h"

CItem::CItem(LPDIRECT3DDEVICE9 _pGraphicDev)
	:CGameObject(_pGraphicDev)
{
}

CItem::~CItem()
{
}

HRESULT CItem::ReadyGameObject()
{
	__super::ReadyGameObject();
	m_eCollideID = ECollideID::ITEM;
	if (!m_bOnField) {
		m_pTransCom->SetPosition(m_vPos.x, m_vPos.y, m_vPos.z);
	}

	return S_OK;
}

_int CItem::UpdateGameObject(const _float& fTimeDelta)
{
	m_fTime = fTimeDelta;
	_vec3 vPos;
	m_pTransCom->GetInfo(INFO_POS, &vPos);
	Compute_ViewZ(&vPos);

	if (m_bOnField) {
		AddRenderGroup(RENDERID::RENDER_ALPHA, shared_from_this());
	}

	if(m_bOnStore)
		CPickingMgr::GetInstance()->AddList(shared_from_this());


	_int iExit(0);

	iExit = __super::UpdateGameObject(fTimeDelta);

	return iExit;
}

void CItem::LateUpdateGameObject()
{

	if (m_bOnField)
		FloatingOnField();

	__super::LateUpdateGameObject();
}

void CItem::RenderGameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->GetWorld());

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->SetTexture(0);
	m_pBufCom->RenderBuffer();

	m_pColliderCom->RenderCollider();
}

void CItem::OnCollide(shared_ptr<CGameObject> _pObj)
{


}

void CItem::SetDropItemInfo(_vec3 _vPos, const tstring& _strName, _int _iAmount)
{
	m_vPos = _vPos;
	m_strItemKey = _strName;
	m_iAmount = _iAmount;
}

void CItem::SetScale(_vec3 _vScale)
{
	m_pTransCom->SetScale(_vScale.x, _vScale.y, _vScale.z);
}

void CItem::GetUITextureKeyName(const tstring& _strOriginName)
{

	tstring strKey = L"";

	if (L"Player_Item_Antivenom" == _strOriginName) {
		strKey = L"Item_UI_Antivenom";
		m_eItemState = EHandItem::ANTI_VENOM;
		m_iMoney = 150;
	}
	else if (L"Player_Item_Shovel" == _strOriginName || L"Item_Shovel" == _strOriginName) {
		strKey = L"Item_UI_Shovel";
		m_eItemState = EHandItem::SHOVEL;
		m_iMoney = 250;

	}
	else if (L"Player_Item_Bandage" == _strOriginName) {
		strKey = L"Item_UI_Bandage";
		m_eItemState = EHandItem::BANDAGE;
		m_iMoney = 150;
	}

	else if (L"Item_Food" == _strOriginName) {
		strKey = L"Item_UI_Food";
		m_eItemState = EHandItem::FOOD;
		m_iMoney = 75;
	}

	else if (L"Item_Torch" == _strOriginName) {
		strKey = L"Item_UI_Torch";
		m_eItemState = EHandItem::TORCH;
		m_iMoney = 75;
	}

	else if (L"Item_Key" == _strOriginName) {
		strKey = L"Item_UI_Key";
		m_eItemState = EHandItem::KEYS;
		m_iMoney = 200;
	}

	else if (L"Item_Heirlooms" == _strOriginName) {
		strKey = L"Item_UI_Heirloom";
		m_eItemState = EHandItem::HEIRLOOM;
	}
	else if (L"Item_Gold1" == _strOriginName) {
		strKey = L"Item_UI_Gold0";
		m_eItemState = EHandItem::GOLD;
	}
	else if (L"Item_Gold2" == _strOriginName) {
		strKey = L"Item_UI_Gold0";
		m_eItemState = EHandItem::GOLD;
	}

 	m_strItemKey = strKey;
}

void CItem::PickingObj()
{
	if (m_bOnStore)
	{
		CGameMgr::GetInstance()->GetPlayer()->OnCollide(shared_from_this());
	}

	else
	{
		m_bActive = false;
	}
}

void CItem::AddComponent()
{
	shared_ptr<CComponent> pComponent;

	pComponent = m_pBufCom = make_shared<CRcTex>(m_pGraphicDev);
	m_pBufCom->ReadyBuffer();
	m_mapComponent[ID_STATIC].insert({ L"Com_RcTex",pComponent });

	pComponent = m_pTextureCom = make_shared<CTexture>(m_pGraphicDev);
	m_mapComponent[ID_STATIC].insert({ L"Com_Texture",pComponent });
	m_pTextureCom->SetTextureKey(m_strItemKey, TEXTUREID::TEX_NORMAL);

	pComponent = m_pTransCom = make_shared<CTransform>();
	m_pTransCom->ReadyTransform();
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform",pComponent });
	m_pTransCom->SetPosition(m_vPos.x, m_vPos.y, m_vPos.z);

	pComponent = m_pColliderCom = make_shared<CCollider>(m_pGraphicDev);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Collider",pComponent });
	m_pColliderCom->SetScale({ 2.f, 2.f, 2.f });
	m_pColliderCom->SetPos(m_pTransCom->GetPos());

}

void CItem::FloatingOnField()
{
	_vec3 vPos;

	m_pTransCom->GetInfo(INFO_POS, &vPos);

	m_fActTime += m_fTime;
	m_fTotalHeight += 0.01f * m_CurrentDir * m_fTime;

	if (0.7f < m_fTime + m_fActTime) {

		m_CurrentDir *= -1.f;
		m_fActTime = 0.f;
		m_fTotalHeight = 0.f;
	}

	m_pTransCom->SetPosition(vPos.x, vPos.y + m_fTotalHeight, vPos.z);
}