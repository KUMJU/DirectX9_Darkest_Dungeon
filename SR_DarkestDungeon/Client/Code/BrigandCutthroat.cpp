#include "pch.h"
#include "BrigandCutthroat.h"
#include"Export_Utility.h"

CBrigandCutthroat::CBrigandCutthroat(LPDIRECT3DDEVICE9 pGraphicDev)
	: CMonster(pGraphicDev)
{
}

CBrigandCutthroat::CBrigandCutthroat(const CCreature& rhs)
	: CMonster(rhs)
{
}

CBrigandCutthroat::~CBrigandCutthroat()
{
}

HRESULT CBrigandCutthroat::ReadyGameObject()
{
	__super::ReadyGameObject();

	m_pTransformCom->SetPosition(m_vPos.x, m_vPos.y, m_vPos.z);
	m_pTransformCom->SetScale(2.f, 2.f, 1.f);

	m_pTransformCom->SetAngle(m_vAngle);
	m_pTransformCom->Rotation(ROT_Y, PI / 2.f);

	return E_NOTIMPL;
}

_int CBrigandCutthroat::UpdateGameObject(const _float& fTimeDelta)
{
	_int	iExit = __super::UpdateGameObject(fTimeDelta);

	Engine::AddRenderGroup(RENDER_ALPHA, shared_from_this());

	//³¡
	
	if (GetAsyncKeyState('1') & 0x8000) {
		m_pTextureCom->SetAnimKey(L"Brigand Cutthroat_Combat", 0.02f);
		m_pTransformCom->SetScale(2.f, 2.f, 1.f);
	}

	if (GetAsyncKeyState('2') & 0x8000) {
		m_pTextureCom->SetAnimKey(L"Brigand Cutthroat_Attack1", 0.02f);
		m_pTransformCom->SetScale(2.f * 427.f / 232.f, 2.f * 268.f / 311.f, 1.f);
		//m_pTransformCom->SetScaleRatio(_vec3(2.f, 2.f, 1.f), _vec2(232.f, 311.f), _vec2(427.f, 268.f));
	}
	
	if (GetAsyncKeyState('3') & 0x8000) {
		m_pTextureCom->SetAnimKey(L"Brigand Cutthroat_Attack2", 0.02f);
		m_pTransformCom->SetScale(2.f * 232.f / 232.f, 2.f * 365.f / 311.f, 1.f);
	}

	if (GetAsyncKeyState('4') & 0x8000) {
		m_pTextureCom->SetAnimKey(L"Brigand Cutthroat_Dead", 0.02f);
		m_pTransformCom->SetScale(2.f * 215.f / 232.f, 2.f * 158.f / 311.f, 1.f);
	}

	if (GetAsyncKeyState('5') & 0x8000) {
		m_pTextureCom->SetAnimKey(L"Brigand Cutthroat_Hitted", 0.02f);
		m_pTransformCom->SetScale(2.f * 225.f / 232.f, 2.f * 317.f / 311.f, 1.f);
	}
	
	//if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
	//	m_pTextureCom->SetAnimKey(L"Brigand Cutthroat_Attack1", 0.02f);
	//	m_pTransformCom->SetScale(2.f * 427.f/232.f, 2.f * 268.f / 311.f, 1.f);
	//	m_pTransformCom->GetInfo(INFO_RIGHT, &vDir);
	//	D3DXVec3Normalize(&vDir, &vDir);
	//	m_pTransformCom->MoveForward(&vDir, fTimeDelta, 5.f);
	//}
	//if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
	//	m_pTextureCom->SetAnimKey(L"Brigand Cutthroat_Combat", 0.02f);
	//	m_pTransformCom->SetScale(2.f, 2.f, 1.f);
	//	m_pTransformCom->GetInfo(INFO_RIGHT, &vDir);
	//	D3DXVec3Normalize(&vDir, &vDir);
	//	m_pTransformCom->MoveForward(&vDir, fTimeDelta, -5.f);
	//}
	//if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
	//	m_pTransformCom->GetInfo(INFO_LOOK, &vDir);
	//	D3DXVec3Normalize(&vDir, &vDir);
	//	m_pTransformCom->MoveForward(&vDir, fTimeDelta, -5.f);
	//}
	//
	//if (GetAsyncKeyState(VK_UP) & 0x8000) {
	//	m_pTransformCom->GetInfo(INFO_LOOK, &vDir);
	//	D3DXVec3Normalize(&vDir, &vDir);
	//	m_pTransformCom->MoveForward(&vDir, fTimeDelta, 5.f);
	//}


	return iExit;
}

void CBrigandCutthroat::LateUpdateGameObject()
{
	__super::LateUpdateGameObject();
}

void CBrigandCutthroat::RenderGameObject()
{

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->GetWorld());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	// m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	m_pTextureCom->SetAnimTexture();
	m_pBufCom->RenderBuffer();
	m_pEffectBufCom->RenderBuffer();

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	__super::RenderGameObject();
}

void CBrigandCutthroat::StartTurn()
{
	__super::StartTurn();
}

void CBrigandCutthroat::AttackCreature(shared_ptr<CGameObject> _pCreature, _float _fSkillRatio, ATTACKTYPE _eAttackTYPE, _int _iDotDamage, _int _iTurn)
{
	__super::AttackCreature(_pCreature, _fSkillRatio, _eAttackTYPE, _iDotDamage, _iTurn);
}

void CBrigandCutthroat::EndAttack(shared_ptr<CGameObject> _pCreature)
{
	__super::EndAttack(_pCreature);
}

void CBrigandCutthroat::BlightAttack(_int _iBlightDmg, _int _iTurn)
{
	__super::BlightAttack(_iBlightDmg, _iTurn);
}

void CBrigandCutthroat::BleedAttack(_int _iBleedDmg, _int _iTurn)
{
	__super::BleedAttack(_iBleedDmg, _iTurn);
}

void CBrigandCutthroat::BlightCure()
{
	__super::BlightCure();
}

void CBrigandCutthroat::BleedCure()
{
	__super::BleedCure();
}

void CBrigandCutthroat::AddComponent()
{
	Engine::CreateNewTexture(L"Brigand Cutthroat_Combat", TEX_NORMAL,
		L"../Bin/Resource/Image/Creatures/Monsters/Weald/Brigand Cutthroat/Combat/armatureName_combat_%d.png", 33);
	Engine::CreateNewTexture(L"Brigand Cutthroat_Attack1", TEX_NORMAL,
		L"../Bin/Resource/Image/Creatures/Monsters/Weald/Brigand Cutthroat/1.png", 1);
	Engine::CreateNewTexture(L"Brigand Cutthroat_Attack2", TEX_NORMAL,
		L"../Bin/Resource/Image/Creatures/Monsters/Weald/Brigand Cutthroat/2.png", 1);
	Engine::CreateNewTexture(L"Brigand Cutthroat_Dead", TEX_NORMAL,
		L"../Bin/Resource/Image/Creatures/Monsters/Weald/Brigand Cutthroat/3.png", 1);
	Engine::CreateNewTexture(L"Brigand Cutthroat_Hitted", TEX_NORMAL,
		L"../Bin/Resource/Image/Creatures/Monsters/Weald/Brigand Cutthroat/4.png", 1);

	shared_ptr<CComponent> pComponent;

	_vec3 vPosTemp = { 0.f,0.f,0.f };

	pComponent = m_pTransformCom = make_shared<CTransform>();
	NULL_CHECK_MSG(pComponent, L"Make Player TransformCom Failed");
	m_pTransformCom->ReadyTransform();
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform",pComponent });

	pComponent = m_pTextureCom = make_shared<CAnimator>(m_pGraphicDev);
	m_pTextureCom->SetAnimKey(L"Brigand Cutthroat_Combat", 0.05f);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Animator",pComponent });

	//pComponent = m_pEffectTransformCom = make_shared<CTransform>();
	//NULL_CHECK_MSG(pComponent, L"Make Player TransformCom Failed");
	//m_pEffectTransformCom->ReadyTransform();
	//m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform",pComponent });
	//
	//pComponent = m_pEffectTextureCom = make_shared<CAnimator>(m_pGraphicDev);
	//m_pEffectTextureCom->SetAnimKey(L"Brigand Cutthroat_Combat", 0.05f);
	//m_mapComponent[ID_DYNAMIC].insert({ L"Com_Animator",pComponent });

	__super::AddComponent();
}

void CBrigandCutthroat::ClimbingTerrain()
{
	__super::ClimbingTerrain();
}

void CBrigandCutthroat::Free()
{
	__super::Free();
}
