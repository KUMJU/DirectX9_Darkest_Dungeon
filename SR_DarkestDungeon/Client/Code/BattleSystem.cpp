#include "pch.h"
#include "BattleSystem.h"

BattleSystem::BattleSystem()
{
}

BattleSystem::~BattleSystem()
{
}

void BattleSystem::Update()
{
	// 턴
	while (1)
	{
		if (IsHeroFirst())
		{
			m_bHeroTrun = true;
		}
		else
		{
			m_bHeroTrun = false;
		}

		// 턴 안의 크리처 턴
		while (1)
		{
			if (m_bHeroTrun)
			{
				m_iTurnHero = TurnCreature(m_vHero);
			}
			else
			{

			}

			
		}
		// 두 진영 중 한 진영이 전멸했을때
		if (HeroesAllDead() || MonstersAllDead())
		{
			EndBattle();
			break;
		}
		NextTurn();
	}
}

_bool BattleSystem::IsHeroFirst()
{
	_int iFastHeroSpeed = 0;
	_int iFastEnemySpeed = 0;
	for (auto& iter : m_vHero)
	{
		if (iter->GetCommonStat().iSpeed > iFastHeroSpeed)
		{
			iFastHeroSpeed = iter->GetCommonStat().iSpeed;
		}
	}
	for (auto& iter : m_vEnemy)
	{
		if (iter->GetCommonStat().iSpeed > iFastEnemySpeed)
		{
			iFastEnemySpeed = iter->GetCommonStat().iSpeed;
		}
	}

	if (iFastHeroSpeed >= iFastEnemySpeed)
		return true;
	else
		return false;
}

void BattleSystem::NextCreatureTurn()
{
	
}

void BattleSystem::NextTurn()
{
	m_iTurn++;
}

void BattleSystem::EndBattle()
{
}

_int BattleSystem::TurnCreature(vector<shared_ptr<CCreature>> _vCreatures)
{
	_int iSpeed = 0;
	_int iIndex = 0;
	for (_int i = 0; i < sizeof(_vCreatures); i++)
	{
		if (_vCreatures[i]->GetCommonStat().iSpeed > iSpeed && _vCreatures[i]->GetAbleAct())
		{
			iSpeed = _vCreatures[i]->GetCommonStat().iSpeed;
			iIndex = i;
		}
	}

	// 모두의 턴을 돌았을때
	if (iSpeed == 0)
	{
		return 10;
	}

	_vCreatures[iIndex]->SetAbleAct(false);

	return iIndex;
}

_bool BattleSystem::HeroesAllDead()
{
	for (auto& iter : m_vHero)
	{
		if (iter->GetCommonStat().iHp > 0)
		{
			return false;
		}
	}
}

_bool BattleSystem::MonstersAllDead()
{
	for (auto& iter : m_vEnemy)
	{
		if (iter->GetCommonStat().iHp > 0)
		{
			return false;
		}
	}
}
