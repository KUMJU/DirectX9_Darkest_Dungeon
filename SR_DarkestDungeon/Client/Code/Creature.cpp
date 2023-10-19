#include "pch.h"
#include "Creature.h"

#include "Export_System.h"
#include"Export_Utility.h"

CCreature::CCreature(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
	memset(&m_tCommonStat, NULL, sizeof(STAT));
}

CCreature::CCreature(const CCreature& rhs)
	: Engine::CGameObject(rhs)
{
}

CCreature::~CCreature()
{
}

HRESULT CCreature::ReadyGameObject()
{
	__super::ReadyGameObject();

	return E_NOTIMPL;
}

_int CCreature::UpdateGameObject(const _float& fTimeDelta)
{
	Engine::AddRenderGroup(RENDER_ALPHA, shared_from_this());
	_int	iExit = CGameObject::UpdateGameObject(fTimeDelta);

	return _int();
}

void CCreature::LateUpdateGameObject()
{
	ClimbingTerrain();
}

void CCreature::RenderGameObject()
{
}

void CCreature::StartTurn()
{
	// 도트뎀 계산
	m_tCommonStat.iHp -= (m_bBlightDot[0] + m_bBleedDot[0]);
	for (int i = 1; i < 3; i++)
	{
		m_bBlightDot[i - 1] = m_bBlightDot[i];
		m_bBleedDot[i - 1] = m_bBleedDot[i];
	}
	m_bBlightDot[3] = 0;
	m_bBleedDot[3] = 0;
}

void CCreature::AttackCreature(CCreature* _pCreature, _float _fSkillRatio, ATTACKTYPE _eAttackTYPE, _int _iDotDamage, _int _iTurn)
{
	if (!_pCreature) return;

	switch (_eAttackTYPE)
	{
	case ATTACKTYPE::ATTACK_NORMAL:
		break;
	case ATTACKTYPE::ATTACK_BLIGHT:
		_pCreature->BlightAttack(_iDotDamage, _iTurn);
		break;
	case ATTACKTYPE::ATTACK_BLEED:
		_pCreature->BleedAttack(_iDotDamage, _iTurn);
		break;
	case ATTACKTYPE::ATTACK_STRESS:
		break;
	}

	// 상대
	_pCreature->m_tCommonStat.iHp -= m_tCommonStat.iAttackPower * _fSkillRatio;
	_pCreature->SetHitted(true);
	_pCreature->SetEffectOn(true);

	// 나
	m_bEffectOn = true;
}

void CCreature::EndAttack(CCreature* _pCreature)
{
	if (!_pCreature) return;

	// 상대
	_pCreature->SetHitted(false);
	_pCreature->SetEffectOn(false);

	// 나
	m_bEffectOn = false;

	// 턴종료
	m_bMyTurn = false;
}

void CCreature::BlightAttack(_int _iBlightDmg, _int _iTurn)
{
	for (int i = 0; i < _iTurn; i++)
	{
		m_bBlightDot[i] + _iBlightDmg;
	}
}

void CCreature::BleedAttack(_int _iBleedDmg, _int _iTurn)
{
	for (int i = 0; i < _iTurn; i++)
	{
		m_bBleedDot[i] + _iBleedDmg;
	}
}

void CCreature::BlightCure()
{
	m_bBlighting = false;
	for (int i = 0; i < 4; i++) m_bBlightDot[i] = 0;
}

void CCreature::BleedCure()
{
	m_bBleeding = false;
	for (int i = 0; i < 4; i++) m_bBleedDot[i] = 0;
}

void CCreature::AddComponent()
{
	shared_ptr<CComponent> pComponent;

	pComponent = m_pBufCom = make_shared<CRcTex>(m_pGraphicDev);
	m_pBufCom->ReadyBuffer();
	m_mapComponent[ID_STATIC].insert({ L"Com_RCTex",pComponent });

	pComponent = m_pEffectBufCom = make_shared<CRcTex>(m_pGraphicDev);
	m_pEffectBufCom->ReadyBuffer();
	m_mapComponent[ID_STATIC].insert({ L"Com_EffectRCTex",pComponent });
}

void CCreature::ClimbingTerrain()
{
	//_vec3		vPos;
	//m_pTransformCom->GetInfo(INFO_POS, &vPos);
	//
	//CTerrainTex* pTerrainBufferCom = dynamic_cast<CTerrainTex*>(Engine::Get_Component(L"Layer_Environment", L"OBJ_Terrain", L"Proto_TerrainTex", ID_STATIC));
	//NULL_CHECK(pTerrainBufferCom);
	//
	//_float		fHeight = m_pCalculatorCom->Compute_HeightOnTerrain(&vPos,
	//	pTerrainBufferCom->Get_VtxPos(),
	//	pTerrainBufferCom->Get_VtxCntX(),
	//	pTerrainBufferCom->Get_VtxCntZ());
	//
	//m_pTransformCom->Set_Pos(vPos.x, fHeight + 0.5f, vPos.z);
}

void CCreature::Free()
{
}
