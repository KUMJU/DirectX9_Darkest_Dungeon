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
	// ���� ���۽� ȣ��
	_bool IsHeroFirst();	// �ϰ��� ���� ������ ������������

	// �� ũ��ó �� ����� ȣ��
	void NextCreatureTurn(); // ������ Creature

	void NextTurn();	// ���� ��

	void EndBattle();

	_int TurnCreature(vector<shared_ptr<CCreature>> _vCreatures);

	_bool HeroesAllDead();
	_bool MonstersAllDead();

private:
	vector<shared_ptr<CCreature>> m_vHero;		// ��Ƽ ����Ʈ		0: ���� ��
	vector<shared_ptr<CCreature>> m_vEnemy;		// ���� ����Ʈ

	_int m_iTurn = 1;	// ���� ��
	_int m_iTurnHero = 0; // ������ ����
	_int m_iTurnEnemy = 0; // ������ ����
	_bool m_bHeroTrun = true; // ����� ��������
};

