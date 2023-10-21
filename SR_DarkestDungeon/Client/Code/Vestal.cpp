#include "pch.h"
//#include "Vestal.h"
//
//CVestal::CVestal(LPDIRECT3DDEVICE9 pGraphicDev)
//	: CHero(pGraphicDev)
//{
//}
//
//CVestal::CVestal(const CVestal& rhs)
//	: CHero(rhs)
//{
//}
//
//CVestal::~CVestal()
//{
//}
//
//HRESULT CVestal::ReadyGameObject()
//{
//	__super::ReadyGameObject();
//
//	// 스킬 세팅
//
//	// 변수들에 값 넣어주기
//
//	return S_OK;
//}
//
//_int CVestal::UpdateGameObject(const _float& fTimeDelta)
//{
//	return _int();
//}
//
//void CVestal::LateUpdateGameObject()
//{
//	for (int i = 0; i < (*m_pVecHeroState).size(); ++i)
//	{
//		switch (*(*m_pVecHeroState)[i])
//		{
//		case EHeroState::IDLE:
//			m_strAnimKey = L"";
//			break;
//		case EHeroState::WALK:
//			break;
//		case EHeroState::COMBAT:
//			break;
//		case EHeroState::DEATH_DOOR:
//			break;
//		case EHeroState::AFFLICTION:
//			break;
//		case EHeroState::VIRTUE:
//			break;
//		case EHeroState::STUN:
//			break;
//		case EHeroState::BLEED:
//			break;
//		case EHeroState::BLIGHT:
//			break;
//		case EHeroState::DEATH:
//			break;
//		default:
//			break;
//		}
//	}
//}
//
//void CVestal::RenderGameObject()
//{
//}