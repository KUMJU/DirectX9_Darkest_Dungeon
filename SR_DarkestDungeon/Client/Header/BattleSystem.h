#pragma once
#include "Creature.h"

class BattleSystem
{
public:
	BattleSystem();
	~BattleSystem();

public:
	vector<shared_ptr<CCreature>> GetHerolist() { return m_vHero; }
	vector<shared_ptr<CCreature>> GetEnemylist() { return m_vEnemy; }
	void SetHerolist(vector<shared_ptr<CCreature>> _HeroList)
	{
		m_vHero = _HeroList;
	}
	void SetEnemylist(vector<shared_ptr<CCreature>> _EnemyList)
	{
		m_vEnemy = _EnemyList;
	}

public:
	void Update();

public:
	// 매턴 시작시 호출
	_bool IsHeroFirst();	// 턴게임 시작 진영이 영웅진영인지

	// 매 크리처 턴 종료시 호출
	void NextCreatureTurn(); // 차례인 Creature

	void NextTurn();	// 다음 턴

	void EndBattle();

	_int TurnCreature(vector<shared_ptr<CCreature>> _vCreatures);

	_bool HeroesAllDead();
	_bool MonstersAllDead();

private:
	vector<shared_ptr<CCreature>> m_vHero;		// 파티 리스트		0: 제일 앞
	vector<shared_ptr<CCreature>> m_vEnemy;		// 적들 리스트

	_int m_iTurn = 1;	// 현재 턴
	_int m_iTurnHero = 0; // 차례인 영웅
	_int m_iTurnEnemy = 0; // 차례인 몬스터
	_bool m_bHeroTrun = true; // 히어로 차례인지
};

