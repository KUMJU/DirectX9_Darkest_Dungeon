#include "pch.h"
#include "DungeonSystem.h"
#include "DungeonRoom.h"

CDungeonSystem::CDungeonSystem()
{
}

CDungeonSystem::~CDungeonSystem()
{
}

void CDungeonSystem::CurrentRoom(int _iNum)
{
	if (size(m_vDungeonRoom) == 0) return;
	for (int i = 0; i < size(m_vDungeonRoom); i++)
	{
		if (i == _iNum)
		{
			(m_vDungeonRoom)[i]->MakeAbleAllObject();
		}
		else
		{
			(m_vDungeonRoom)[i]->MakeDisableAllObject();
		}
	}
}
