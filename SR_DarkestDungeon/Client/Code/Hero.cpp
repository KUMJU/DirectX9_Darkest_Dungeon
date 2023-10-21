#include "pch.h"
//#include "Hero.h"
//#include"Export_Utility.h"
//
//CHero::CHero(LPDIRECT3DDEVICE9 pGraphicDev)
//	: CCreature(pGraphicDev)
//{
//}
//
//CHero::CHero(const CHero& rhs)
//	: CCreature(rhs)
//{
//}
//
//CHero::~CHero()
//{
//
//}
//
//HRESULT CHero::ReadyGameObject()
//{
//	__super::ReadyGameObject();
//
//	return S_OK;
//}
//
//_int CHero::UpdateGameObject(const _float& fTimeDelta)
//{
//	Engine::AddRenderGroup(RENDER_ALPHA, shared_from_this());
//	_int	iExit = CGameObject::UpdateGameObject(fTimeDelta);
//
//	
//
//	return iExit;
//}
//
//void CHero::LateUpdateGameObject()
//{
//}
//
//void CHero::RenderGameObject()
//{	
//	// m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->GetWorld());
//	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
//
//	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
//	// m_pGraphicDev->SetRenderState(D3DRS_ , D3DFILL_WIREFRAME);
//
//	m_pBufCom->RenderBuffer();
//	m_pEffectBufCom->RenderBuffer();
//
//	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
//
//	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
//}
//
//void CHero::SelectSkill(_int _iSkillID)
//{
//	m_pSelectedSkill = (*m_pVecSkill)[_iSkillID];
//
//	// �������� �ʾҰų� ��� �� ���� ��ų�̸� ���� �Ұ�
//	if (!m_pSelectedSkill->IsEquipped() || !m_pSelectedSkill->IsUnlocked())
//	{
//		m_pSelectedSkill = nullptr;
//		return;
//	}
//
//	// ���� ��ġ���� ����� �� ���� ��ų�̸� ���� �Ұ�
//	if (!m_pSelectedSkill->IsActivatePos(m_tCommonStat.iOrder))
//	{
//		m_pSelectedSkill = nullptr;
//		return;
//	}
//
//	// �� ���� �� ��� Ƚ���� �� �� ��ų�̸� ���� �Ұ�
//	if (!m_pSelectedSkill->CanActivateCnt())
//	{
//		m_pSelectedSkill = nullptr;
//		return;
//	}
//}
//
//void CHero::AttackCreature(_int _iEnemyPos, shared_ptr<CCreature> _pCreature)
//{
//	// ��ų�� ���õ��� ���� ���
//	if (!m_pSelectedSkill)
//		return;
//
//	// ������ ��ų�� ������ ��븦 ������ �� ���� ���
//	if (!m_pSelectedSkill->IsTargetPos(_iEnemyPos))
//		return;
//
//	_int iAbilityCnt = m_pSelectedSkill->GetAbliltyCnt();
//	
//	EAttackType* eAttackType;
//	eAttackType = m_pSelectedSkill->GetListAbility();
//
//	for (_int i = 0; i < iAbilityCnt; ++i)
//	{
//		switch (eAttackType[i])
//		{
//		case EAttackType::ATTACK:
//			break;
//		case EAttackType::BLIGHT:
//			_pCreature->BlightAttack(m_pSelectedSkill->GetDotDamage()[0], m_pSelectedSkill->GetDotDamage()[1]);
//			break;
//		case EAttackType::BLEED:
//			_pCreature->BleedAttack(m_pSelectedSkill->GetDotDamage()[0], m_pSelectedSkill->GetDotDamage()[1]);
//			break;
//		case EAttackType::STUN:
//			break;
//		case EAttackType::MOVE:
//			break;
//		case EAttackType::HEAL:
//			break;
//		default:
//			break;
//		}
//	}
//}
//
//void CHero::AddComponent()
//{
//	shared_ptr<CComponent> pComponent;
//
//	pComponent = m_pBufCom = make_shared <CRcTex>(m_pGraphicDev);
//	m_pBufCom->ReadyBuffer();
//	m_mapComponent[ID_STATIC].insert({ L"Com_RCTex",pComponent });
//
//	pComponent = m_pEffectBufCom = make_shared<CRcTex>(m_pGraphicDev);
//	m_pEffectBufCom->ReadyBuffer();
//	m_mapComponent[ID_STATIC].insert({ L"Com_EffectRCTex",pComponent });
//	
//	pComponent = m_pTextureCom = make_shared<CAnimator>(m_pGraphicDev);
//	m_pTextureCom->SetAnimKey(m_strAnimKey, 0.05f);
//	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Animator", pComponent });
//}
//
//void CHero::Free()
//{
//}
