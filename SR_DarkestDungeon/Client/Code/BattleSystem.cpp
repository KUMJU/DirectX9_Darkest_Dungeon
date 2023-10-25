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
	// 새로운 턴 시작시
	if (m_pCurrentCreature == nullptr) StartTurn();

	// 상태에 따른 턴 사이 간격 설정
	if (!m_bNext && !m_bCounting)
	{
		if (!dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->GetIsCorpse() &&
			!dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->GetIsDeath())
		{
			m_fTime = BATTLEINTERVEL;
		}
		else if (dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->GetIsCorpse())
		{
			m_fTime = CORPSEINTERVEL;
		}
		else
		{
			m_fTime = 0.f;
		}
		m_bCounting = true;
	}
	
	if (!m_bNext && m_bCounting)
	{
		m_fTime -= fTimeDelta;
		if (m_fTime < 0.f)
		{
			m_bNext = true;
			m_bCounting = false;
		}
	}

	// 크리처의 턴일때
	if (dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->GetTurn() && m_bNext)
	{
		// 크리처 종류에 따라 행동
		// 출혈, 독뎀 반영, 기절이면 기절 줄어들기, 죽으면 죽도록
		dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->StartCalculate();
		
		// 공격 AI
		if (!dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->GetIsCorpse() &&
			!dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->GetIsDeath() &&
			dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->GetTurn())
		{
			int iNum = rand() % dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->GetSkillNum();
			int iTarget = 0;
			if (m_bHero)
				iTarget = rand() % size(m_vMonsters);
			else
				iTarget = rand() % size(m_vHeroes);

			if (m_bHero)
			{
				dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->AttackCreature
				(dynamic_pointer_cast<CCreature>(m_vMonsters[iTarget]), dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->GetSkill(iNum));
				dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->SetAttacking(true, iNum);
				dynamic_pointer_cast<CCreature>(m_vMonsters[iTarget])->SetHitted(true);
			}
			else
			{
				dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->AttackCreature
				(dynamic_pointer_cast<CCreature>(m_vHeroes[iTarget]), dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->GetSkill(iNum));
				dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->SetAttacking(true, iNum);
				dynamic_pointer_cast<CCreature>(m_vHeroes[iTarget])->SetHitted(true);
			}
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
}

shared_ptr<CGameObject> CBattleSystem::NextCreature()
{
	// 속도를 기준으로 내림차순
	CmpBySpeed(m_vCreatures);
	// 영웅부터 몬스터까지 빠른 속도인 creature를 반환
	for (int i = 0; i < size(m_vCreatures); i++)	
	{
		for (int j = 0; j < size(m_vHeroes); j++)
		{
			if ((dynamic_pointer_cast<CCreature>(m_vHeroes[j])->GetCommonStat().iSpeed ==
				dynamic_pointer_cast<CCreature>(m_vCreatures[i])->GetCommonStat().iSpeed)
				&& !(dynamic_pointer_cast<CCreature>(m_vHeroes[j])->GetDone()))
			{
				dynamic_pointer_cast<CCreature>(m_vHeroes[j])->SetTurn(true);
				m_bHero = true;
				return m_vHeroes[j];
			}
		}
		for (int j = 0; j < size(m_vMonsters); j++)
		{
			if ((dynamic_pointer_cast<CCreature>(m_vMonsters[j])->GetCommonStat().iSpeed ==
				dynamic_pointer_cast<CCreature>(m_vCreatures[i])->GetCommonStat().iSpeed)
				&& !(dynamic_pointer_cast<CCreature>(m_vMonsters[j])->GetDone()))
			{
				dynamic_pointer_cast<CCreature>(m_vMonsters[j])->SetTurn(true);
				m_bHero = false;
				return m_vMonsters[j];
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
	for (auto& iter : m_vHeroes)
	{
		dynamic_pointer_cast<CCreature>(iter)->SetDone(false);
	}
	for (auto& iter : m_vMonsters)
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
		if (!dynamic_pointer_cast<CCreature>(iter)->GetIsDeath())
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
		if (!dynamic_pointer_cast<CCreature>(iter)->GetIsDeath())
		{
			return false;
		}
	}
	return true;
}

void CBattleSystem::CmpBySpeed(vector<shared_ptr<CGameObject>>& _vCreatures)
{
	int size = _vCreatures.size();
	for (int i = 0; i < size; i++) {
		for (int j = 1; j < size - i; j++) {
			if (dynamic_pointer_cast<CCreature>(_vCreatures[j])->GetCommonStat().iSpeed >
				dynamic_pointer_cast<CCreature>(_vCreatures[j-1])->GetCommonStat().iSpeed) {
				shared_ptr<CGameObject> pObj = _vCreatures[j];
				_vCreatures[j] = _vCreatures[j-1];
				_vCreatures[j-1] = pObj;
			}
		}
	}
}