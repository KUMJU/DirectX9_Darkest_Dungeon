#include"pch.h"
#include "Player.h"
#include"Export_Utility.h"

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

	return S_OK;
}

_int CPlayer::UpdateGameObject(const _float& fTimeDelta)
{
	KeyInput(fTimeDelta);
	Engine::AddRenderGroup(RENDER_ALPHA, shared_from_this());
	_int	iExit = CGameObject::UpdateGameObject(fTimeDelta);

	return iExit;
}

void CPlayer::LateUpdateGameObject()
{
	CGameObject::LateUpdateGameObject();
}

void CPlayer::RenderGameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->GetWorld());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->SetAnimTexture();
	m_pBufCom->RenderBuffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

}

void CPlayer::AddComponent()
{
	shared_ptr<CComponent> pComponent;

	_vec3 vPosTemp = { 0.f,0.f,0.f };

	pComponent = m_pTransformCom = make_shared<CTransform>();
	NULL_CHECK_MSG(pComponent, L"Make Player TransformCom Failed");
	m_pTransformCom->ReadyTransform();
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform",pComponent });
	m_pTransformCom->SetScale(2.f, 2.f, 1.f);

	pComponent = m_pBufCom = make_shared<CRcTex>(m_pGraphicDev);
	m_pBufCom->ReadyBuffer();
	m_mapComponent[ID_STATIC].insert({ L"Com_RCTex",pComponent });

	//¿©±â
	pComponent = m_pTextureCom = make_shared<CAnimator>(m_pGraphicDev);
	m_pTextureCom->SetAnimKey(L"Highwayman_Idle", 0.05f);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Animator",pComponent });



}

void CPlayer::KeyInput(const _float& fTimeDelta)
{
	_vec3		vDir;

	if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
		m_pTextureCom->SetAnimKey(L"Highwayman_Walk", 0.02f);
		m_pTransformCom->GetInfo(INFO_RIGHT, &vDir);
		D3DXVec3Normalize(&vDir, &vDir);
		m_pTransformCom->MoveForward(&vDir, fTimeDelta, m_fSpeed);
	}

	if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
		m_pTransformCom->GetInfo(INFO_RIGHT, &vDir);
		D3DXVec3Normalize(&vDir, &vDir);
		m_pTransformCom->MoveForward(&vDir, fTimeDelta, -m_fSpeed);
	}

	if (GetAsyncKeyState(VK_DOWN) & 0x8000){
		m_pTransformCom->GetInfo(INFO_LOOK, &vDir);
		D3DXVec3Normalize(&vDir, &vDir);
		m_pTransformCom->MoveForward(&vDir, fTimeDelta, -m_fSpeed);
	}


	if (GetAsyncKeyState(VK_UP) & 0x8000) {
		m_pTransformCom->GetInfo(INFO_LOOK, &vDir);
		D3DXVec3Normalize(&vDir, &vDir);
		m_pTransformCom->MoveForward(&vDir, fTimeDelta, m_fSpeed);
	}


}

void CPlayer::ClimbingTerrain()
{
}

void CPlayer::Free()
{
}
