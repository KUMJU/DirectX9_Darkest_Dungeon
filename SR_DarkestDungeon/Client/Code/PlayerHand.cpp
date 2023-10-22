#include"pch.h"
#include "PlayerHand.h"
#include"Export_Utility.h"

CPlayerHand::CPlayerHand(LPDIRECT3DDEVICE9 _pGraphicDev)
	:CGameObject(_pGraphicDev)
{
}

CPlayerHand::~CPlayerHand()
{
}

HRESULT CPlayerHand::ReadyGameObject()
{
	m_bWalking = false;
	m_bEnable = false;

	m_pPlrTransmCom = dynamic_pointer_cast<CTransform>(Engine::Get_Component(L"Layer_GameObj", L"Obj_Player", L"Com_Transform", COMPONENTID::ID_DYNAMIC));
	
	return S_OK;
}

_int CPlayerHand::UpdateGameObject(const _float& fTimeDelta)
{
	if (!m_bEnable)
		return 0;

	int iExit(0);
	iExit = __super::UpdateGameObject(fTimeDelta);
	AddRenderGroup(RENDER_ALPHA, shared_from_this());
	m_fTime = fTimeDelta;
	return iExit;
}

void CPlayerHand::LateUpdateGameObject()
{
	if (!m_bEnable)
		return;

	_matrix matWorld;
	_vec3 vPos, vItemPos;
	_vec3 vLook, vRight , vUp;
	_matrix mat1;

	matWorld = *m_pPlrTransmCom->GetWorld();
	m_pPlrTransmCom->GetInfo(INFO_LOOK, &vLook);
	m_pPlrTransmCom->GetInfo(INFO_POS, &vPos);
	m_pPlrTransmCom->GetInfo(INFO_RIGHT, &vRight);
	m_pPlrTransmCom->GetInfo(INFO_UP, &vUp);

	m_pItemTransmCom->GetInfo(INFO_POS, &vItemPos);

	D3DXVec3Normalize(&vLook, &vLook);
	D3DXVec3Normalize(&vRight, &vRight);

	vPos = vPos + ((vLook* 4.5f + vRight * -3.f));

	if (m_bWalking) {
		m_fActTime += m_fTime;
		m_fTotalHeight += 0.3f * m_CurrentDir * m_fTime;
	

		if (0.4f < m_fTime + m_fActTime) {
			
			m_CurrentDir *= -1.f;
			m_fActTime = 0.f;
		}

	}


	m_pItemTransmCom->SetPosition(vPos.x, vPos.y + m_fTotalHeight, vPos.z);

}

void CPlayerHand::RenderGameObject()
{
	if (!m_bEnable)
		return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pItemTransmCom->GetWorld());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, FALSE);

	m_pTextureCom->SetTexture(0);
	m_pItemBufCom->RenderBuffer();

	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CPlayerHand::SetCurrentItem(EHandItem _handItem)
{
	switch (_handItem)
	{
	case EHandItem::SHOVEL:
		m_pTextureCom->SetTextureKey(L"Player_Item_Shovel", TEX_NORMAL);
		m_pItemTransmCom->SetScale(1.5f, 1.5f, 1.f);
		m_pItemTransmCom->SetAngle({ D3DXToRadian(50.f),0.f, D3DXToRadian(-10.f) });

		m_bEnable = true;
		break;
	case EHandItem::ANTI_VENOM:
		m_pTextureCom->SetTextureKey(L"Player_Item_Antivenom", TEX_NORMAL);
		m_pItemTransmCom->SetScale(1.5f, 1.5f, 1.f);
		m_pItemTransmCom->SetAngle({ 0.f,0.f, D3DXToRadian(-10.f) });
		m_bEnable = true;
		break;
	case EHandItem::KEYS:
		m_pTextureCom->SetTextureKey(L"Player_Item_Shovel", TEX_NORMAL);
		m_bEnable = true;
		break;
	case EHandItem::BANDAGE:
		m_pTextureCom->SetTextureKey(L"Player_Item_Bandage", TEX_NORMAL);
		m_pItemTransmCom->SetScale(1.5f, 1.5f, 1.f);
		m_pItemTransmCom->SetAngle({ 0.f ,0.f, D3DXToRadian(-10.f) });
		m_bEnable = true;
		break;
	case EHandItem::ENUM_END:
		m_bEnable = false;
		break;
	default:
		break;
	}

}

void CPlayerHand::AddComponent()
{
	shared_ptr<CComponent> pComponent;

	pComponent = m_pItemBufCom = make_shared<CRcTex>(m_pGraphicDev);
	m_pItemBufCom->ReadyBuffer();
	m_mapComponent[ID_STATIC].insert({ L"Com_PlrItemRcTex",pComponent });

	pComponent = m_pTextureCom = make_shared<CTexture>(m_pGraphicDev);
	m_mapComponent[ID_STATIC].insert({ L"Com_PlrItemTex",pComponent });

	pComponent = m_pItemTransmCom = make_shared<CTransform>();
	NULL_CHECK_MSG(pComponent, L"Make Player Item TransformCom Failed");
	m_pItemTransmCom->ReadyTransform();
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_PlrItemTrans",pComponent });

}

