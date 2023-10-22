#pragma once
#include"GameObject.h"

class CBattleSystem;

class CDungeonRoom
{
public:
	explicit CDungeonRoom();
	virtual ~CDungeonRoom();

public:
	vector<shared_ptr<CGameObject>>	GetGameObjectVector() { return m_vGameObject; }
	vector<shared_ptr<CGameObject>>	GetHeroesVector() { return m_vHeroes; }
	vector<shared_ptr<CGameObject>>	GetMonstersVector() { return m_vMonsters; }
	shared_ptr<CBattleSystem>		GetBattleSystem() { return m_pBattleSystem; }
	void	SetBattleSystem(shared_ptr<CBattleSystem> _pBattleSystem) { m_pBattleSystem = _pBattleSystem; }

public:
	void	PushGameObjectVector(vector<shared_ptr<CGameObject>>& _vVec)
	{
		m_vGameObject = _vVec;
	}
	void	PushHeroesVector(vector<shared_ptr<CGameObject>>& _vVec)
	{
		m_vHeroes = _vVec;
	}
	void	PushMonstersVector(vector<shared_ptr<CGameObject>>& _vVec)
	{
		m_vMonsters = _vVec;
	}

	// ������ ī�޶� ��ġ ��ȯ �Լ�
	_vec3	GetBattleCameraPos() { return _vec3(WALLSIZEX + PATHSIZEX + 10.f, 3.f, WALLSIZEX * 14.f + 4.f); }

public:
	void	MakeDisableAllObject();
	void	MakeAbleAllObject();
	void	BattleUpdate(const _float& fTimeDelta);		// �ο�

public:
	// �������� ��ġ
	void	FormBattlePosition(vector<shared_ptr<CGameObject>>& _pHeroes, vector<shared_ptr<CGameObject>>& _pMonsters,
		_float _fAngle, _vec3 _vOrigin);

private:
	vector<shared_ptr<CGameObject>>		m_vGameObject = {};
	vector<shared_ptr<CGameObject>>		m_vHeroes = {};
	vector<shared_ptr<CGameObject>>		m_vMonsters = {};
	shared_ptr<CBattleSystem>			m_pBattleSystem = nullptr;
};

