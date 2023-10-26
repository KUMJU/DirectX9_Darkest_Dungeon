#include "pch.h"
#include "BattleSystem.h"
#include "Creature.h"
#include "Export_System.h"

CBattleSystem::CBattleSystem()
{
}

CBattleSystem::~CBattleSystem()
{
}

void CBattleSystem::Update(const _float& fTimeDelta)
{
	// 턴 시작시
	if (m_pCurrentCreature == nullptr) StartTurn();
	
	if (!m_bNext)
	{
		m_fTime -= fTimeDelta;
		if (m_fTime < 0.f)
		{
			m_fTime = BATTLEINTERVEL;
			m_bNext = true;
		}
	}

	// 크리처의 턴일때
	if (dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->GetTurn() && m_bNext)
	{
		// 크리처 종류에 따라 행동
		// 시체일때
		if (dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->GetIsCorpse())
		{
			
		}
		// 출혈, 독뎀 반영, 기절이면 기절 줄어들기, 죽으면 죽도록
		dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->StartCalculate();
		
		// 공격 AI
		int iNum = rand() % 2;
		int iTarget = rand() % 4;
		switch (iNum)
		{
		case 0:
			if (m_bHero)
			{
				dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->AttackCreature
				(dynamic_pointer_cast<CCreature>(m_vMonsters[iTarget]), dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->GetSkill(0));
				dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->SetAttacking1(true);
				dynamic_pointer_cast<CCreature>(m_vMonsters[iTarget])->SetHitted(true);
			}
			else
			{
				dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->AttackCreature
				(dynamic_pointer_cast<CCreature>(m_vHeroes[iTarget]), dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->GetSkill(0));
				dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->SetAttacking1(true);
				dynamic_pointer_cast<CCreature>(m_vHeroes[iTarget])->SetHitted(true);
			}
			break;

		case 1:
			if (m_bHero)
			{
				dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->AttackCreature
				(dynamic_pointer_cast<CCreature>(m_vMonsters[iTarget]), dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->GetSkill(1));
				dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->SetAttacking2(true);
				dynamic_pointer_cast<CCreature>(m_vMonsters[iTarget])->SetHitted(true);
			}
			else
			{
				dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->AttackCreature
				(dynamic_pointer_cast<CCreature>(m_vHeroes[iTarget]), dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->GetSkill(1));
				dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->SetAttacking2(true);
				dynamic_pointer_cast<CCreature>(m_vHeroes[iTarget])->SetHitted(true);
			}
			break;
		}
		// 크리처 턴 엔드
		CreatureTurnEnd();
		m_bNext = false;
	}

	//if (dynamic_pointer_cast<CCreature>(m_vHeroes[0])->GetCommonStat().iHp < 0)
	//	int c = 5;

	// 종료 조건
	if (HeroesAllDead() || MonstersAllDead())
		EndBattle();
}

void CBattleSystem::StartTurn()
{
	// 턴 시작시 나오는 UI등 그런 것들
	m_pCurrentCreature = NextCreature();
	//_float fTurnInterver = 0.f;
	//fTurnInterver += Engine::Get_TimeDelta(L"Timer_Immediate");
	//// 크리처 턴 시작
	//if (fTurnInterver > 1.f)
	//{
	//	fTurnInterver = 0.f;
	//	m_pCurrentCreature = NextCreature();
	//}
}

shared_ptr<CGameObject> CBattleSystem::NextCreature()
{
	// 속도를 기준으로 내림차순
	CmpBySpeed(m_vCreatures);

	// 영웅부터 몬스터까지 빠른 속도인 creature를 반환
	for (int i = 0; i < size(m_vCreatures); i++)
	{
		for (auto& iter : m_vHeroes)
		{
			if ((dynamic_pointer_cast<CCreature>(iter)->GetCommonStat().iSpeed ==
				dynamic_pointer_cast<CCreature>(m_vCreatures[i])->GetCommonStat().iSpeed)
				&& !dynamic_pointer_cast<CCreature>(m_vCreatures[i])->GetDone())
			{

				dynamic_pointer_cast<CCreature>(iter)->SetTurn(true);

				dynamic_pointer_cast<CCreature>(m_vCreatures[i])->SetTurn(true);
				m_bHero = true;
				return m_vCreatures[i];
			}
		}
		for (auto& iter : m_vMonsters)
		{
			if ((dynamic_pointer_cast<CCreature>(iter)->GetCommonStat().iSpeed ==
				dynamic_pointer_cast<CCreature>(m_vCreatures[i])->GetCommonStat().iSpeed)
				&& !dynamic_pointer_cast<CCreature>(m_vCreatures[i])->GetDone())
			{
				dynamic_pointer_cast<CCreature>(m_vCreatures[i])->SetTurn(true);
				m_bHero = false;
				return m_vCreatures[i];
			}
		}
	}
	
	// 모두 다 했을시에 다음 턴으로
	NextTurn();

	return shared_ptr<CGameObject>();
}

void CBattleSystem::CreatureTurnEnd()
{
	dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->SetTurn(false);
	dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->SetDone(true);
	m_pCurrentCreature = NextCreature();
}

void CBattleSystem::NextTurn()
{
	// 모두 초기화
	for (auto& iter : m_vCreatures)
	{
		dynamic_pointer_cast<CCreature>(iter)->SetDone(false);
	}
	m_pCurrentCreature = nullptr;

	m_iTurn++;
}

void CBattleSystem::EndBattle()
{
}

_bool CBattleSystem::HeroesAllDead()
{
	for (auto& iter : m_vHeroes)
	{
		if (dynamic_pointer_cast<CCreature>(iter)->GetCommonStat().iHp > 0)
		{
			return false;
		}
	}
	return true;
}

_bool CBattleSystem::MonstersAllDead()
{
	for (auto& iter : m_vMonsters)
	{
		if (dynamic_pointer_cast<CCreature>(iter)->GetCommonStat().iHp > 0)
		{
			return false;
		}
	}
	return true;
}

void CBattleSystem::CmpBySpeed(vector<shared_ptr<CGameObject>>& _vCreatures)
{
	
	//sort(_vCreatures.begin(), _vCreatures.end());
	
}