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

void CDungeonSystem::CurrentRoom(int _iNum)
{
	if (size(m_vDungeonRoom) == 0) return;
	for (int i = 0; i < size(m_vDungeonRoom); i++)
	{
		if (i == (_iNum - 1))
		{
			// Object�� Update Ȱ��ȭ
			(m_vDungeonRoom)[i]->MakeAbleAllObject();
		}
		else
		{
			(m_vDungeonRoom)[i]->MakeDisableAllObject();
		}
	}
}
