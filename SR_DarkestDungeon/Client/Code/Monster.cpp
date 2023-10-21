#include "pch.h"
#include "Monster.h"
#include"Export_Utility.h"

CMonster::CMonster(LPDIRECT3DDEVICE9 pGraphicDev)
	:CCreature(pGraphicDev)
{
}

CMonster::CMonster(LPDIRECT3DDEVICE9 pGraphicDev, STAT _tCommonStat, _int _iPosition,
	shared_ptr<vector<shared_ptr<CSkill>>> _pVecSkill)
	: CCreature(pGraphicDev, _tCommonStat, _iPosition, _pVecSkill)
{
}

CMonster::CMonster(LPDIRECT3DDEVICE9 pGraphicDev, STAT _tCommonStat)
	: CCreature(pGraphicDev, _tCommonStat)
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
	_int	iExit = __super::UpdateGameObject(fTimeDelta);

	return iExit;
}

void CMonster::LateUpdateGameObject()
{
	__super::LateUpdateGameObject();
}

void CMonster::RenderGameObject()
{
	__super::RenderGameObject();
}

void CMonster::Free()
{
	__super::Free();
}
