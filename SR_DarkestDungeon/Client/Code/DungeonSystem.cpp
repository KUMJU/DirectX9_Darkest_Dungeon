#include "pch.h"
#include "DungeonSystem.h"
#include "DungeonRoom.h"

#include "Player.h"

#include"GameMgr.h"

CDungeonSystem::CDungeonSystem()
{
}

CDungeonSystem::~CDungeonSystem()
{
}

void CDungeonSystem::AbleRoom(int _iNum)
{
	if (size(m_vDungeonRoom) == 0) return;
	for (int i = 0; i < size(m_vDungeonRoom); i++)
	{
		if (i == (_iNum - 1))
		{
			// Object들 Update 활성화
			(m_vDungeonRoom)[i]->MakeAbleAllObject();
		}
	}
}

void CDungeonSystem::DisableRoom(int _iNum)
{
	if (size(m_vDungeonRoom) == 0) return;
	for (int i = 0; i < size(m_vDungeonRoom); i++)
	{
		if (i == (_iNum - 1))
		{
			(m_vDungeonRoom)[i]->MakeDisableAllObject();
		}
	}
}

void CDungeonSystem::DisableAllRoom()
{
	if (size(m_vDungeonRoom) == 0) return;
	for (int i = 0; i < size(m_vDungeonRoom); i++)
	{
		(m_vDungeonRoom)[i]->MakeDisableAllObject();
	}
}
