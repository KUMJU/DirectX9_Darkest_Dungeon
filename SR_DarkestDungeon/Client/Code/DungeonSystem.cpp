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
			// 전투 준비
			if ((m_vDungeonRoom)[i]->GetBattleSystem())
			{
				(m_vDungeonRoom)[i]->BattleReady();
				dynamic_pointer_cast<CPlayer>(CGameMgr::GetInstance()->GetPlayer())->SetInBattle(true);
			}
			// Object들 Update 활성화
			(m_vDungeonRoom)[i]->MakeAbleAllObject();
		}
		else
		{
			(m_vDungeonRoom)[i]->MakeDisableAllObject();
		}
	}
}
