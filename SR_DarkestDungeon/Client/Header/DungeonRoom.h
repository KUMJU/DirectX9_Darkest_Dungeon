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

	// 전투시 카메라 위치 반환 함수
	_vec3	GetBattleCameraPos() { return m_vBattleCameraPos; }
	void	SetBattleCameraPos(_vec3 _vBattleCameraPos) { m_vBattleCameraPos = _vBattleCameraPos; }

public:
	void	MakeDisableAllObject();
	void	MakeAbleAllObject();
	void	BattleUpdate(const _float& fTimeDelta);		// 싸움
	void	BattleReady();	// 싸움 직전 자리잡아주기

private:
	vector<shared_ptr<CGameObject>>		m_vGameObject = {};
	vector<shared_ptr<CGameObject>>		m_vHeroes = {};
	vector<shared_ptr<CGameObject>>		m_vMonsters = {};
	shared_ptr<CBattleSystem>			m_pBattleSystem = nullptr;

	_vec3 m_vBattleCameraPos;		// 던전 방 싸움 위치
};

