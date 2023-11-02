#include"pch.h"
#include "Player.h"
#include"Export_Utility.h"
#include"Export_System.h"
#include"Inventory.h"
#include "Wall.h"
#include"UIMgr.h"
#include"CameraMgr.h"
#include "PickingMgr.h"
#include "Hero.h"
#include"GoodsUI.h"


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

	m_eCollideID = ECollideID::PLAYER;
	m_bColliding = true;

	return S_OK;
}

_int CPlayer::UpdateGameObject(const _float& fTimeDelta)
{
	m_pPlayerHand->StopShakingHand();
	if (!m_bLock)
		KeyInput(fTimeDelta);
	_int	iExit = CGameObject::UpdateGameObject(fTimeDelta);
	Engine::AddRenderGroup(RENDER_ALPHA, shared_from_this());

	for (int i = 0; i < 4; ++i)
	{
		m_bMoveLock[i] = false;
	}

	return iExit;
}

void CPlayer::LateUpdateGameObject()
{
	MouseInput();
	m_bPrevMouse = m_bCurMouse;

	CGameObject::LateUpdateGameObject();
}

void CPlayer::RenderGameObject()
{
	m_pColliderCom->RenderCollider();
}

void CPlayer::SetInventory(shared_ptr<CInventory> _pInventory)
{
	m_pInventory = _pInventory;
	m_pInventory->SetPlayer(dynamic_pointer_cast<CPlayer>(shared_from_this()));
}

void CPlayer::SetCurrentItem(EHandItem _eItem)
{
	if (m_pPlayerHand) {
		m_pPlayerHand->SetCurrentItem(_eItem);
		m_eCurrentItem = _eItem;
	}
}

void CPlayer::SetGold(_int _iNum, _bool _bIsEarn)
{
	if (!_bIsEarn) {
		_iNum *= -1;
	}

	m_iGold += _iNum;

	shared_ptr<CUIObj> pGoodsUI = CUIMgr::GetInstance()->FindUI(L"Obj_GoodsUI");
	//재화 UI 세팅
	if (nullptr != pGoodsUI) {
		dynamic_pointer_cast<CGoodsUI>(pGoodsUI)->SetGoldNum(m_iGold);
	}

}

void CPlayer::SetHeirloom(_int _iNum, _bool _bIsEarn)
{
	if (!_bIsEarn) {
		_iNum *= -1;
	}

	m_iHeirlooms += _iNum;

	shared_ptr<CUIObj> pGoodsUI = CUIMgr::GetInstance()->FindUI(L"Obj_GoodsUI");
	//재화 UI 세팅
	if (nullptr != pGoodsUI) {
		dynamic_pointer_cast<CGoodsUI>(pGoodsUI)->SetHeirloomNum(m_iHeirlooms);
	}
}

void CPlayer::AddComponent()
{
	shared_ptr<CComponent> pComponent;

	_vec3 vPosTemp = { 0.f,0.f,0.f };

	pComponent = m_pTransformCom = make_shared<CTransform>();
	m_pTransformCom->ReadyTransform();
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform",pComponent });
	m_pTransformCom->SetScale(1.f, 1.f, 1.f);
	m_pTransformCom->SetPosition(m_vPos.x, m_vPos.y, m_vPos.z);

	pComponent = m_pColliderCom = make_shared<CCollider>(m_pGraphicDev);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Collider",pComponent });
	m_pColliderCom->SetScale({ 3.f, 3.f, 3.f });
	m_pColliderCom->SetPos(m_pTransformCom->GetPos());

}

void CPlayer::KeyInput(const _float& fTimeDelta)
{

	_vec3		vDir;

	if (GetAsyncKeyState('P') & 0x8000) {
		CCameraMgr::GetInstance()->CameraRotation(ECameraMode::ROTATION, 180.f);
	}

	if (GetAsyncKeyState('O') & 0x8000) {
		CCameraMgr::GetInstance()->CameraRotation(ECameraMode::ROTATION, -180.f);
	}
	
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
		if (m_bMoveLock[1])
			return;
		m_pTransformCom->GetInfo(INFO_RIGHT, &vDir);
		D3DXVec3Normalize(&vDir, &vDir);
		m_pTransformCom->MoveForward(&vDir, fTimeDelta, m_fSpeed);
		ShakingHand();
		m_fDeltaTime = fTimeDelta;
		m_eLastMove = EPlayerMove::RIGHT;
	}

	if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
		if (m_bMoveLock[3])
			return;
		m_pTransformCom->GetInfo(INFO_RIGHT, &vDir);
		D3DXVec3Normalize(&vDir, &vDir);
		m_pTransformCom->MoveForward(&vDir, fTimeDelta, -m_fSpeed);
		ShakingHand();
		m_fDeltaTime = fTimeDelta;
		m_eLastMove = EPlayerMove::LEFT;
	}

	if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
		if (m_bMoveLock[2])
			return;
		m_pTransformCom->GetInfo(INFO_LOOK, &vDir);
		D3DXVec3Normalize(&vDir, &vDir);
		m_pTransformCom->MoveForward(&vDir, fTimeDelta, -m_fSpeed);
		ShakingHand();
		m_fDeltaTime = fTimeDelta;
		m_eLastMove = EPlayerMove::DOWN;
	}


	if (GetAsyncKeyState(VK_UP) & 0x8000) {
		if (m_bMoveLock[0])
			return;
		m_pTransformCom->GetInfo(INFO_LOOK, &vDir);
		D3DXVec3Normalize(&vDir, &vDir);
		m_pTransformCom->MoveForward(&vDir, fTimeDelta, m_fSpeed);
		ShakingHand();
		m_fDeltaTime = fTimeDelta;
		m_eLastMove = EPlayerMove::UP;
	}

}

void CPlayer::MouseInput()
{
	//마우스 픽킹
	if (Engine::Get_DIMouseState(MOUSEKEYSTATE::DIM_LB)) {

		m_bCurMouse = true;

		if (m_bCurMouse == m_bPrevMouse)
			return;

		POINT	ptMouse{};
		GetCursorPos(&ptMouse);
		ScreenToClient(g_hWnd, &ptMouse);

		_bool result = CUIMgr::GetInstance()->PickingUI(ptMouse.x, ptMouse.y);

		if (!result) {
			CPickingMgr::GetInstance()->RayPicking(ptMouse.x, ptMouse.y);
		}
	}

	else
		m_bCurMouse = false;
}

void CPlayer::ClimbingTerrain()
{
}

void CPlayer::ShakingHand()
{
	if (EHandItem::ENUM_END != m_eCurrentItem)
		m_pPlayerHand->ShakingHand();

}

void CPlayer::InsertItem(shared_ptr<CItem> _pItem)
{
	m_pInventory->InsertItem(_pItem);
}

void CPlayer::OnCollide(shared_ptr<CGameObject> _pObj)
{
	// ITEM 충돌
	if (ECollideID::ITEM == _pObj->GetColType())
	{
		shared_ptr<CItem> pNewItem = make_shared<CItem>(m_pGraphicDev);
 		pNewItem->GetUITextureKeyName(dynamic_pointer_cast<CItem>(_pObj)->GetItemKeyName());
		pNewItem->SetAmount(dynamic_pointer_cast<CItem>(_pObj)->GetAmount());
		pNewItem->SetOnField(false);
		InsertItem(pNewItem);

		_pObj->SetActive(false);
	}
}

void CPlayer::OnCollide(shared_ptr<CGameObject> _pObj, _float _fGap, EDirection _eDir)
{
	// WALL 충돌
	if (ECollideID::WALL == _pObj->GetColType() || ECollideID::ENVIRONMENT == _pObj->GetColType())
	{
		_vec3		vPlayerPos;
		_vec3		vPlayerScale;
		_vec3		vWallPos;
		_vec3		vWallColScale;


		shared_ptr<CCollider> pWallCollider = dynamic_pointer_cast<CCollider>(_pObj->GetComponent(L"Com_Collider", ID_DYNAMIC));
		vWallPos = *pWallCollider->GetPos();
		vWallColScale = *pWallCollider->GetScale();
		vPlayerPos = *m_pColliderCom->GetPos();
		vPlayerScale = *m_pColliderCom->GetScale();

		_bool bHorizontal = dynamic_pointer_cast<CGameObject>(_pObj)->IsHorizontal();

		if (bHorizontal)
		{
			switch (_eDir)
			{
			case EDirection::TOP:
				m_pTransformCom->SetPosition(vPlayerPos.x, vPlayerPos.y, vPlayerPos.z - _fGap);
				m_bMoveLock[0] = true;
				break;
			case EDirection::BOTTOM:
				m_pTransformCom->SetPosition(vPlayerPos.x, vPlayerPos.y, vPlayerPos.z + _fGap);
				m_bMoveLock[2] = true;
				break;
			default:
				break;
			}
		}

		else
		{
			switch (_eDir)
			{
			case EDirection::RIGHT:
				m_pTransformCom->SetPosition(vPlayerPos.x - _fGap, vPlayerPos.y, vPlayerPos.z);
				m_bMoveLock[1] = true;
				break;
			case EDirection::LEFT:
				m_pTransformCom->SetPosition(vPlayerPos.x + _fGap, vPlayerPos.y, vPlayerPos.z);
				m_bMoveLock[3] = true;
				break;
			default:
				break;
			}
		}
	}
}

void CPlayer::Free()
{
}
