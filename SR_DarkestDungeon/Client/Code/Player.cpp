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
#include "TavernUI.h"

#include "Description.h"


CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CPlayer::~CPlayer()
{
}

HRESULT CPlayer::ReadyGameObject()
{
	if (m_bReady)
		return S_OK;

	__super::ReadyGameObject();

	m_eCollideID = ECollideID::PLAYER;
	m_bColliding = true;

	m_bReady = true;
	SetGold(1000, true);
	SetHeirloom(5, true);

	//m_pLight = CLightMgr::GetInstance()->InitPointLight(m_pGraphicDev);
	//m_pLight2 = CLightMgr::GetInstance()->InitPointLight(m_pGraphicDev);
	//m_pLight3 = CLightMgr::GetInstance()->InitPointLight(m_pGraphicDev);

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

	// 뒤 돌아봤을때 영웅 보이게 하기
	if (m_bInDungeon && m_bSeeBack && !m_bInBattle)
	{
		ShowHeroesBack();
	}

	// 전투 중이 아니고 뒤 돌아본 상태가 아니일때 영웅들 안보이게
	if (m_bInDungeon && !m_bSeeBack && !m_bInBattle)
	{
		for (int i = 0; i < size(m_pVecHero); i++)
		{
			dynamic_pointer_cast<CCreature>(m_pVecHero[i])->SetFront(true);
			shared_ptr<CTransform> pTransform = dynamic_pointer_cast<CTransform>(
				m_pVecHero[i]->GetComponent(L"Com_Transform", ID_DYNAMIC));
			pTransform->SetPosition(m_pTransformCom->GetPos()->x - 3.f, m_pTransformCom->GetPos()->y - 400.f, m_pTransformCom->GetPos()->z - 9.f - 5.f * i);
		}
	}


	for (auto& iter : m_vecPickingObject)
	{
		CPickingMgr::GetInstance()->AddList(iter);
	}

	if (m_pTavernUI)
		m_pTavernUI->UpdateGameObject(fTimeDelta);

	//_vec3* vPos = m_pTransformCom->GetPos();
	//m_pLight->SetPosition({ vPos ->x, vPos->y + 7.f, vPos->z + 4.f});
	/*m_pLight2->SetPosition({vPos->x, vPos->y + 7.f, vPos->z + 4.f});
	m_pLight3->SetPosition({ vPos ->x, vPos->y + 7.f, vPos->z + 4.f});*/


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

void CPlayer::SetTavernUI(shared_ptr<CTavernUI> _pTavernUI)
{
	m_pTavernUI = _pTavernUI;
}

void CPlayer::ShowTavernUI(shared_ptr<CHero> _pHero)
{
	for (auto& iter : m_pVecHero)
		dynamic_pointer_cast<CHero>(iter)->SetSelected(false);

	m_pTavernUI->SetHero(_pHero);
	m_pTavernUI->SetVisible(true);
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

void CPlayer::SettingLight()
{
	


}

void CPlayer::SetPlayerMode(EPlayerMode _ePlrMode)
{
	if (EPlayerMode::DEFAULT == _ePlrMode) {

		CCameraMgr::GetInstance()->SetFPSMode();


	}
	else if(EPlayerMode::BOSS_FIELD == _ePlrMode) {

		CCameraMgr::GetInstance()->SetVillageMode();

		m_eCurrentItem = EHandItem::SPELLHAND_FIRE;
		m_ePlayerMode = EPlayerMode::BOSS_FIELD;

		CUIMgr::GetInstance()->SwitchFPSMode();

		if (m_pPlayerHand) {
			m_pPlayerHand->SetCurrentItem(m_eCurrentItem);
			m_pPlayerHand->ShakingHand();
		}
		//FPS Player UI 세팅
		//PlayerHandSetting

	}



}

void CPlayer::AddComponent()
{
	if (m_bReady)
		return;

	shared_ptr<CComponent> pComponent;

	_vec3 vPosTemp = { 0.f,0.f,0.f };

	if (m_pTransformCom) return;

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

	//기본 던전 플레이어 카메라 move
	if (m_ePlayerMode == EPlayerMode::DEFAULT) {

		if (GetAsyncKeyState('P') & 0x8000 && m_bInDungeon) {
			CCameraMgr::GetInstance()->CameraRotation(ECameraMode::LOOKBACK, 180.f);
			m_bSeeBack = true;
		}

		if (GetAsyncKeyState('O') & 0x8000) {
			CCameraMgr::GetInstance()->CameraRotation(ECameraMode::LOOKBACK, -180.f);
			m_bSeeBack = false;
		}

		if (GetAsyncKeyState('E') & 0x8000) {
			SetCurrentItem(EHandItem::ENUM_END);
		}

		if (GetAsyncKeyState('K') & 0x8000) {
			CCameraMgr::GetInstance()->CameraRotation(ECameraMode::ROTATION, 90.f);
		}
	}

	if (GetAsyncKeyState('D') & 0x8000) {
		if (m_bMoveLock[1])
			return;
		m_pTransformCom->GetInfo(INFO_RIGHT, &vDir);
		D3DXVec3Normalize(&vDir, &vDir);
		m_pTransformCom->MoveForward(&vDir, fTimeDelta, m_fSpeed);
		ShakingHand();
		m_fDeltaTime = fTimeDelta;
		m_eLastMove = EPlayerMove::RIGHT;
	}

	if (GetAsyncKeyState('A') & 0x8000) {
		if (m_bMoveLock[3])
			return;
		m_pTransformCom->GetInfo(INFO_RIGHT, &vDir);
		D3DXVec3Normalize(&vDir, &vDir);
		m_pTransformCom->MoveForward(&vDir, fTimeDelta, -m_fSpeed);
		ShakingHand();
		m_fDeltaTime = fTimeDelta;
		m_eLastMove = EPlayerMove::LEFT;
	}

	if (GetAsyncKeyState('S') & 0x8000) {
		if (m_bMoveLock[2])
			return;
		m_pTransformCom->GetInfo(INFO_LOOK, &vDir);
		D3DXVec3Normalize(&vDir, &vDir);
		m_pTransformCom->MoveForward(&vDir, fTimeDelta, -m_fSpeed);
		ShakingHand();
		m_fDeltaTime = fTimeDelta;
		m_eLastMove = EPlayerMove::DOWN;
	}


	if (GetAsyncKeyState('W') & 0x8000) {
		if (m_bMoveLock[0])
			return;
		m_pTransformCom->GetInfo(INFO_LOOK, &vDir);
		D3DXVec3Normalize(&vDir, &vDir);
		m_pTransformCom->MoveForward(&vDir, fTimeDelta, m_fSpeed);
		ShakingHand();
		m_fDeltaTime = fTimeDelta;
		m_eLastMove = EPlayerMove::UP;
	}


	//기본 던전 카메라
	if (GetAsyncKeyState('1') & 0x8000) {
		CCameraMgr::GetInstance()->SetFPSMode();
	}

	//빌리지 카메라 & FPS 보스 카메라
	if (GetAsyncKeyState('2') & 0x8000) {
		SetPlayerMode(EPlayerMode::BOSS_FIELD);
	//	CCameraMgr::GetInstance()->SetVillageMode();
	}

}

void CPlayer::MouseInput()
{

	POINT	ptMouse{};
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	if (m_ePlayerMode == EPlayerMode::DEFAULT) {

		//마우스 픽킹
		if (Engine::Get_DIMouseState(MOUSEKEYSTATE::DIM_LB)) {

			m_bCurMouse = true;

			if (m_bCurMouse == m_bPrevMouse)
				return;

			_bool result = CUIMgr::GetInstance()->PickingUI(ptMouse.x, ptMouse.y);

			if (!result) {
				CPickingMgr::GetInstance()->RayPicking(ptMouse.x, ptMouse.y);
			}
		}

		else
			m_bCurMouse = false;

		//UI Hover
		{
			CUIMgr::GetInstance()->HoverUI(ptMouse.x, ptMouse.y);

		}
	}
	else if (m_ePlayerMode == EPlayerMode::BOSS_FIELD) 
	{
		if (Engine::Get_DIMouseState(MOUSEKEYSTATE::DIM_LB)) {
			m_bCurMouse = true;

			if (m_bCurMouse == m_bPrevMouse)
				return;

			m_pPlayerHand->CreateProjection();
		}
		else
			m_bCurMouse = false;

	}

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

void CPlayer::DeleteItem(tstring _strItmeName)
{
	m_pInventory->DeleteItem(_strItmeName);
}

void CPlayer::OnCollide(shared_ptr<CGameObject> _pObj)
{

	// ITEM 충돌
	if (ECollideID::ITEM == _pObj->GetColType())
	{
		shared_ptr<CItem> pItem = dynamic_pointer_cast<CItem>(_pObj);
		shared_ptr<CItem> pNewItem = make_shared<CItem>(m_pGraphicDev);
		pNewItem->GetUITextureKeyName(pItem->GetItemKeyName());

		if (pItem->IsOnStore())
		{
			if (m_iGold < pNewItem->GetMoney())
				return;

			SetGold(pNewItem->GetMoney(), false);
		}

		pNewItem->SetAmount(pItem->GetAmount());
		pNewItem->SetOnField(false);
		InsertItem(pNewItem);

		if (!pItem->IsOnStore())
		{
			_pObj->SetActive(false);
		}
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

void CPlayer::ShowHeroesBack()
{
	for (int i = 0; i < size(m_pVecHero); i++)
	{
		dynamic_pointer_cast<CCreature>(m_pVecHero[i])->SetFront(false);
		shared_ptr<CTransform> pTransform = dynamic_pointer_cast<CTransform>(
			m_pVecHero[i]->GetComponent(L"Com_Transform", ID_DYNAMIC));

		const _vec3* vPlrPos = m_pTransformCom->GetPos();

		pTransform->SetPosition(m_pTransformCom->GetPos()->x - 5.f, m_pTransformCom->GetPos()->y + 3.f, m_pTransformCom->GetPos()->z - 9.f - 4.f * i);
		pTransform->SetAngle(_vec3(0.f,0.f,0.f));
	}
}

void CPlayer::ShowHeroesBackVillage()
{
	for (int i = 0; i < size(m_pVecHero); i++)
	{
		dynamic_pointer_cast<CCreature>(m_pVecHero[i])->SetFront(false);
		shared_ptr<CTransform> pTransform = dynamic_pointer_cast<CTransform>(
			m_pVecHero[i]->GetComponent(L"Com_Transform", ID_DYNAMIC));
		pTransform->SetPosition(m_pTransformCom->GetPos()->x + 5.f - 3.f * i, m_pTransformCom->GetPos()->y + 3.f, m_pTransformCom->GetPos()->z - 9.f - 2.f * i);
		pTransform->SetAngle(_vec3(0.f, 0.f, 0.f));

		dynamic_pointer_cast<CHero>(m_pVecHero[i])->SetHired(true);

		m_vecPickingObject.push_back(m_pVecHero[i]);
	}

}

void CPlayer::HideHeroesBackVillage()
{
	for (int i = 0; i < size(m_pVecHero); i++)
	{
		dynamic_pointer_cast<CHero>(m_pVecHero[i])->SetFront(true);
		shared_ptr<CTransform> pTransform = dynamic_pointer_cast<CTransform>(
			m_pVecHero[i]->GetComponent(L"Com_Transform", ID_DYNAMIC));
		pTransform->SetPosition(m_pTransformCom->GetPos()->x - 3.f, m_pTransformCom->GetPos()->y - 400.f, m_pTransformCom->GetPos()->z - 9.f - 5.f * i);
		dynamic_pointer_cast<CHero>(m_pVecHero[i])->SetSelected(false);
	}

	m_vecPickingObject.clear();
}

void CPlayer::Free()
{
}
