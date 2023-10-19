#include "pch.h"
#include "Monster.h"
#include"Export_Utility.h"

CMonster::CMonster(LPDIRECT3DDEVICE9 pGraphicDev)
	:CCreature(pGraphicDev)
{
}

CMonster::CMonster(const CCreature& rhs)
	: CCreature(rhs)
{
}

CMonster::~CMonster()
{
}

HRESULT CMonster::ReadyGameObject()
{
	__super::ReadyGameObject();

	return E_NOTIMPL;
}

_int CMonster::UpdateGameObject(const _float& fTimeDelta)
{
	__super::UpdateGameObject(fTimeDelta);
	
	return _int();
}

void CMonster::LateUpdateGameObject()
{
	__super::LateUpdateGameObject();
}

void CMonster::RenderGameObject()
{
	__super::RenderGameObject();
}

void CMonster::StartTurn()
{
	__super::StartTurn();
}

void CMonster::AttackCreature(CCreature* _pCreature, _float _fSkillRatio, ATTACKTYPE _eAttackTYPE, _int _iDotDamage, _int _iTurn)
{
	__super::AttackCreature(_pCreature, _fSkillRatio, _eAttackTYPE, _iDotDamage, _iTurn);
}

void CMonster::EndAttack(CCreature* _pCreature)
{
	__super::EndAttack(_pCreature);
}

void CMonster::BlightAttack(_int _iBlightDmg, _int _iTurn)
{
	__super::BlightAttack(_iBlightDmg, _iTurn);
}

void CMonster::BleedAttack(_int _iBleedDmg, _int _iTurn)
{
	__super::BleedAttack(_iBleedDmg, _iTurn);
}

void CMonster::BlightCure()
{
	__super::BlightCure();
}

void CMonster::BleedCure()
{
	__super::BleedCure();
}

void CMonster::AddComponent()
{
	__super::AddComponent();
}

void CMonster::ClimbingTerrain()
{
	__super::ClimbingTerrain();
}

void CMonster::Free()
{
	__super::Free();
}
