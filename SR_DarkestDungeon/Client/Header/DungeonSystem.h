#pragma once
#include"GameObject.h"

class CDungeonRoom;

class CDungeonSystem
{
public:
	explicit CDungeonSystem();
	virtual ~CDungeonSystem();

public:
	void	CurrentRoom(int _iNum);

public:
	vector<shared_ptr<CDungeonRoom>>	GetDungeonRoomVector() { return m_vDungeonRoom; }

public:
	void	PushDungeonRoomVector(vector<shared_ptr<CDungeonRoom>>& _vVec)
	{
		m_vDungeonRoom = _vVec;
	}



private:
	vector<shared_ptr<CDungeonRoom>>	m_vDungeonRoom = {};

};

