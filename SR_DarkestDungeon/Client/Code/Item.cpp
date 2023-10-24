#include"pch.h"
#include"Item.h"
#include"Export_Utility.h"

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
	else 
	{
		AddRenderGroup(RENDERID::RENDER_UI, shared_from_this());
	}

	_int iExit(0);

	iExit = __super::UpdateGameObject(fTimeDelta);

	return iExit;
}

void CItem::LateUpdateGameObject()
{

	if(m_bOnField)
		FloatingOnField();

	__super::LateUpdateGameObject();
}

void CItem::RenderGameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->GetWorld());

	if (m_bOnField) {
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	}
	else {
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, FALSE);
	}

	m_pTextureCom->SetTexture(0);
	m_pBufCom->RenderBuffer();

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->GetWorld());

	if (m_bOnField) {
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	}
	else {
		m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
		m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	}
	m_pColliderCom->RenderCollider();
}

void CItem::OnCollide(shared_ptr<CGameObject> _pObj)
{
	_int a = 3;
}

void CItem::SetDropItemInfo(_vec3 _vPosition, const tstring& _strName )
{
	m_vPosition = _vPosition;
	m_strItemKey = _strName;
}

void CItem::SetScale(_vec3 _vScale)
{
	m_pTransCom->SetScale(_vScale.x, _vScale.y, _vScale.z);
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
	NULL_CHECK_MSG(pComponent, L"Make Player Item TransformCom Failed");
	m_pTransCom->ReadyTransform();
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform",pComponent });
	m_pTransCom->SetPosition(m_vPosition.x, m_vPosition.y, m_vPosition.z);

	pComponent = m_pColliderCom = make_shared<CCollider>(m_pGraphicDev);
	NULL_CHECK_MSG(pComponent, L"Make Player Item ColliderCom Failed");
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