#include"pch.h"
#include "Player.h"
#include"Export_Utility.h"
#include"Inventory.h"


CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CPlayer::~CPlayer()
{
}

HRESULT CPlayer::ReadyGameObject()
{
	__super::ReadyGameObject();

	m_pTransformCom->SetPosition(4.f, 0.f, 0.f);

	return S_OK;
}



_int CPlayer::UpdateGameObject(const _float& fTimeDelta)
{
	if(!m_bLock)
		KeyInput(fTimeDelta);
	m_pPlayerHand->StopShakingHand();
	KeyInput(fTimeDelta);
	_int	iExit = CGameObject::UpdateGameObject(fTimeDelta);
	Engine::AddRenderGroup(RENDER_ALPHA, shared_from_this());
	
	return iExit;
}

void CPlayer::LateUpdateGameObject()
{
	CGameObject::LateUpdateGameObject();
}

void CPlayer::RenderGameObject()
{

}

void CPlayer::AddComponent()
{
	shared_ptr<CComponent> pComponent;

	_vec3 vPosTemp = { 0.f,0.f,0.f };

	pComponent = m_pTransformCom = make_shared<CTransform>();
	NULL_CHECK_MSG(pComponent, L"Make Player TransformCom Failed");
	m_pTransformCom->ReadyTransform();
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform",pComponent });
	m_pTransformCom->SetScale(1.f, 1.f, 1.f);

}

void CPlayer::KeyInput(const _float& fTimeDelta)
{
	
	_vec3		vDir;

	if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
		m_pTransformCom->GetInfo(INFO_RIGHT, &vDir);
		D3DXVec3Normalize(&vDir, &vDir);
		m_pTransformCom->MoveForward(&vDir, fTimeDelta, m_fSpeed);
		ShakingHand();

	}

	if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
		m_pTransformCom->GetInfo(INFO_RIGHT, &vDir);
		D3DXVec3Normalize(&vDir, &vDir);
		m_pTransformCom->MoveForward(&vDir, fTimeDelta, -m_fSpeed);
		ShakingHand();

	}

	if (GetAsyncKeyState(VK_DOWN) & 0x8000){
		m_pTransformCom->GetInfo(INFO_LOOK, &vDir);
		D3DXVec3Normalize(&vDir, &vDir);
		m_pTransformCom->MoveForward(&vDir, fTimeDelta, -m_fSpeed);
		ShakingHand();

	}


	if (GetAsyncKeyState(VK_UP) & 0x8000) {
		m_pTransformCom->GetInfo(INFO_LOOK, &vDir);
		D3DXVec3Normalize(&vDir, &vDir);
		m_pTransformCom->MoveForward(&vDir, fTimeDelta, m_fSpeed);
		ShakingHand();

	}

	if (GetAsyncKeyState('1') & 0x8000) {

		m_pPlayerHand->SetCurrentItem(EHandItem::SHOVEL);
		m_eCurrentItem = EHandItem::SHOVEL;

	}

	if (GetAsyncKeyState('2') & 0x8000) {

		m_pPlayerHand->SetCurrentItem(EHandItem::ANTI_VENOM);
		m_eCurrentItem = EHandItem::ANTI_VENOM;


	}

	if (GetAsyncKeyState('3') & 0x8000) {

		m_pPlayerHand->SetCurrentItem(EHandItem::BANDAGE);
		m_eCurrentItem = EHandItem::BANDAGE;
	}

}

void CPlayer::ClimbingTerrain()
{
}

void CPlayer::ShakingHand()
{
	if(EHandItem::ENUM_END != m_eCurrentItem )
		m_pPlayerHand->ShakingHand();

}

void CPlayer::InsertItem(shared_ptr<CItem> _pItem)
{
	m_pInventory->InsertItem(_pItem);
}

void CPlayer::OnCollide(shared_ptr<CGameObject> _pObj)
{

	//item
	
	shared_ptr<CItem> pNewItem = make_shared<CItem>(m_pGraphicDev);
	pNewItem->SetDropItemInfo({ -182.f, -320.f ,0.f }, L"Item_UI_Antivenom");

	pNewItem->AwakeGameObject();
	pNewItem->ReadyGameObject();
	pNewItem->SetOnField(false);
	//임시로 고정값으로 해둠
	pNewItem->SetScale({ 20.f, 38.f, 1.f });

	InsertItem(pNewItem);

	_pObj->SetActive(false);

}

void CPlayer::Free()
{
}
