#pragma once
#include"GameObject.h"

class CBattleSystem
{
public:
	CBattleSystem();
	~CBattleSystem();

public:
	vector<shared_ptr<CGameObject>> GetHeroesVector() { return m_vHeroes; }
	vector<shared_ptr<CGameObject>> GetMonstersVector() { return m_vMonsters; }
	void SetCurrentCreature(shared_ptr<CGameObject> _pCrea) { m_pCurrentCreature = _pCrea; }
	void PushCreaturesVector(vector<shared_ptr<CGameObject>>& _vVec)
	{
		m_vCreatures = _vVec;
	}
	void PushHeroesVector(vector<shared_ptr<CGameObject>>& _vVec)
	{
		m_vHeroes = _vVec;
	}
	void PushMonstersVector(vector<shared_ptr<CGameObject>>& _vVec)
	{
		m_vMonsters = _vVec;
	}

public:
	void Update(const _float& fTimeDelta); // �� ������

public:
	// �� ���۽� ȣ��
	void StartTurn();

	// �� ũ��ó �� ���۽� ȣ��
	shared_ptr<CGameObject> NextCreature(); // �������� ���ͱ��� ���� �ӵ��� creature�� ��ȯ

	// �� ũ��ó �� ����� ȣ��
	void CreatureTurnEnd();

	// �� ����� ȣ��
	void NextTurn();	// ���� ��

	// ��Ʋ ��
	void EndBattle();	// ���� ����

	_bool HeroesAllDead();
	_bool MonstersAllDead();

protected:
	void CmpBySpeed(vector<shared_ptr<CGameObject>>& _vCreatures);

private:
	vector<shared_ptr<CGameObject>> m_vCreatures;		// ����, ���� ���� ����Ʈ
	vector<shared_ptr<CGameObject>> m_vHeroes;			// ���� ����Ʈ		0: ���� ��
	vector<shared_ptr<CGameObject>> m_vMonsters;		// ���� ����Ʈ

	_int m_iTurn = 1;	// ���� ��
	shared_ptr<CGameObject>			m_pCurrentCreature = nullptr;

	_float  m_fTime = BATTLEINTERVEL;	// �� ũ��ó �ϻ��� ����
	_bool	m_bNext = false;
	_bool	m_bHero = false;			// ���� ��ü�� ��������?
	_bool	m_bCounting = false;
};

