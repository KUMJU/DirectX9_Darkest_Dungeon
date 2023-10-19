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
