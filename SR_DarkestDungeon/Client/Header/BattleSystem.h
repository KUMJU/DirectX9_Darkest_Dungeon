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

	// ���� �� ������ �ڷ� ������
	void	DeadCheck();

	// �������� ��ġ
	void	FormBattlePosition(vector<shared_ptr<CGameObject>>& _pHeroes, vector<shared_ptr<CGameObject>>& _pMonsters,
		_float _fAngle, _vec3 _vOrigin);

protected:
	void CmpBySpeed(vector<shared_ptr<CGameObject>>& _vCreatures);

private:
	vector<shared_ptr<CGameObject>> m_vCreatures;		// ����, ���� ���� ����Ʈ
	vector<shared_ptr<CGameObject>> m_vHeroes;			// ���� ����Ʈ		0: ���� ��
	vector<shared_ptr<CGameObject>> m_vMonsters;		// ���� ����Ʈ

	_int m_iTurn = 1;	// ���� ��
	shared_ptr<CGameObject>			m_pCurrentCreature = nullptr;

	_float  m_fBattleTime = BATTLEINTERVEL;		// �� ũ��ó �ϻ��� ����
	_float	m_fMovingTime = MOVINGINTERVEL;		// ũ��ó ������ �ű�� �ð�
	_bool	m_bNext = false;
	_bool	m_bDeathMoving = false;
	_bool	m_bHero = false;			// ���� ��ü�� ��������?
	_bool	m_bCounting = false;

};

