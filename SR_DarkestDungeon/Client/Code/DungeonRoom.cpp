#include "pch.h"
#include "DungeonRoom.h"
#include "Creature.h"
#include "BattleSystem.h"

CDungeonRoom::CDungeonRoom()
{
}

CDungeonRoom::~CDungeonRoom()
{
}

void CDungeonRoom::MakeDisableAllObject()
{
	if (size(m_vGameObject) == 0) return;
	for (auto& iter : m_vGameObject) {
		if(iter->GetIsEnable()) iter->SetEnable(false);
	}
}

void CDungeonRoom::MakeAbleAllObject()
{
	if (size(m_vGameObject) == 0) return;
	for (auto& iter : m_vGameObject) {
		if (!iter->GetIsEnable()) iter->SetEnable(true);
	}
}

void CDungeonRoom::BattleUpdate(const _float& fTimeDelta)
{
	//FormBattlePosition(m_vHeroes, m_vMonsters, -PI / 2.f, _vec3(WALLSIZEX + PATHSIZEX, WALLSIZEX * 14.f, 0.f));

	m_pBattleSystem->Update(fTimeDelta);
}

void CDungeonRoom::FormBattlePosition(vector<shared_ptr<CGameObject>>& _vHeroes, vector<shared_ptr<CGameObject>>& _vMonsters, _float _fAngle, _vec3 _vOrigin)
{
	// 좌측
	// 3열
	dynamic_pointer_cast<CCreature>(_vHeroes[2])->SetPos(_vec3(_vOrigin.x - 4.f, 3.f, _vOrigin.y + 8.f));
	dynamic_pointer_cast<CCreature>(_vHeroes[2])->SetAngle(_vec3(0.f, _fAngle, 0.f));
	
	// 4열
	dynamic_pointer_cast<CCreature>(_vHeroes[3])->SetPos(_vec3(_vOrigin.x - 6.f, 3.f, _vOrigin.y + 6.f));
	dynamic_pointer_cast<CCreature>(_vHeroes[3])->SetAngle(_vec3(0.f, _fAngle, 0.f));

	// 1열
	dynamic_pointer_cast<CCreature>(_vHeroes[0])->SetPos(_vec3(_vOrigin.x + 4.f, 3.f, _vOrigin.y + 4.f));
	dynamic_pointer_cast<CCreature>(_vHeroes[0])->SetAngle(_vec3(0.f, _fAngle, 0.f));

	// 2열
	dynamic_pointer_cast<CCreature>(_vHeroes[1])->SetPos(_vec3(_vOrigin.x + 2.f, 3.f, _vOrigin.y + 2.f));
	dynamic_pointer_cast<CCreature>(_vHeroes[1])->SetAngle(_vec3(0.f, _fAngle, 0.f));

	// 우측
	// 3열
	dynamic_pointer_cast<CCreature>(_vMonsters[2])->SetPos(_vec3(_vOrigin.x + 24.f, 3.f, _vOrigin.y + 8.f));
	dynamic_pointer_cast<CCreature>(_vMonsters[2])->SetAngle(_vec3(0.f, _fAngle + PI, 0.f));

	// 4열
	dynamic_pointer_cast<CCreature>(_vMonsters[3])->SetPos(_vec3(_vOrigin.x + 26.f, 3.f, _vOrigin.y + 6.f));
	dynamic_pointer_cast<CCreature>(_vMonsters[3])->SetAngle(_vec3(0.f, _fAngle + PI, 0.f));

	// 1열
	dynamic_pointer_cast<CCreature>(_vMonsters[0])->SetPos(_vec3(_vOrigin.x + 16.f, 3.f, _vOrigin.y + 4.f));
	dynamic_pointer_cast<CCreature>(_vMonsters[0])->SetAngle(_vec3(0.f, _fAngle + PI, 0.f));

	// 2열
	dynamic_pointer_cast<CCreature>(_vMonsters[1])->SetPos(_vec3(_vOrigin.x + 18.f, 3.f, _vOrigin.y + 2.f));
	dynamic_pointer_cast<CCreature>(_vMonsters[1])->SetAngle(_vec3(0.f, _fAngle + PI, 0.f));
}
