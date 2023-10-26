#include "pch.h"
#include "BattleSystem.h"
#include "Creature.h"
#include "Export_System.h"

CBattleSystem::CBattleSystem()
{
}

CBattleSystem::~CBattleSystem()
{
}

void CBattleSystem::Update(const _float& fTimeDelta)
{
	// �� ���۽�
	if (m_pCurrentCreature == nullptr) StartTurn();
	
	if (!m_bNext)
	{
		m_fTime -= fTimeDelta;
		if (m_fTime < 0.f)
		{
			m_fTime = BATTLEINTERVEL;
			m_bNext = true;
		}
	}

	// ũ��ó�� ���϶�
	if (dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->GetTurn() && m_bNext)
	{
		// ũ��ó ������ ���� �ൿ
		// ��ü�϶�
		if (dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->GetIsCorpse())
		{
			
		}
		// ����, ���� �ݿ�, �����̸� ���� �پ���, ������ �׵���
		dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->StartCalculate();
		
		// ���� AI
		int iNum = rand() % 2;
		int iTarget = rand() % 4;
		switch (iNum)
		{
		case 0:
			if (m_bHero)
			{
				dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->AttackCreature
				(dynamic_pointer_cast<CCreature>(m_vMonsters[iTarget]), dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->GetSkill(0));
				dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->SetAttacking1(true);
				dynamic_pointer_cast<CCreature>(m_vMonsters[iTarget])->SetHitted(true);
			}
			else
			{
				dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->AttackCreature
				(dynamic_pointer_cast<CCreature>(m_vHeroes[iTarget]), dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->GetSkill(0));
				dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->SetAttacking1(true);
				dynamic_pointer_cast<CCreature>(m_vHeroes[iTarget])->SetHitted(true);
			}
			break;

		case 1:
			if (m_bHero)
			{
				dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->AttackCreature
				(dynamic_pointer_cast<CCreature>(m_vMonsters[iTarget]), dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->GetSkill(1));
				dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->SetAttacking2(true);
				dynamic_pointer_cast<CCreature>(m_vMonsters[iTarget])->SetHitted(true);
			}
			else
			{
				dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->AttackCreature
				(dynamic_pointer_cast<CCreature>(m_vHeroes[iTarget]), dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->GetSkill(1));
				dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->SetAttacking2(true);
				dynamic_pointer_cast<CCreature>(m_vHeroes[iTarget])->SetHitted(true);
			}
			break;
		}
		// ũ��ó �� ����
		CreatureTurnEnd();
		m_bNext = false;
	}

	//if (dynamic_pointer_cast<CCreature>(m_vHeroes[0])->GetCommonStat().iHp < 0)
	//	int c = 5;

	// ���� ����
	if (HeroesAllDead() || MonstersAllDead())
		EndBattle();
}

void CBattleSystem::StartTurn()
{
	// �� ���۽� ������ UI�� �׷� �͵�
	m_pCurrentCreature = NextCreature();
	//_float fTurnInterver = 0.f;
	//fTurnInterver += Engine::Get_TimeDelta(L"Timer_Immediate");
	//// ũ��ó �� ����
	//if (fTurnInterver > 1.f)
	//{
	//	fTurnInterver = 0.f;
	//	m_pCurrentCreature = NextCreature();
	//}
}

shared_ptr<CGameObject> CBattleSystem::NextCreature()
{
	// �ӵ��� �������� ��������
	CmpBySpeed(m_vCreatures);

	// �������� ���ͱ��� ���� �ӵ��� creature�� ��ȯ
	for (int i = 0; i < size(m_vCreatures); i++)
	{
		for (auto& iter : m_vHeroes)
		{
			if ((dynamic_pointer_cast<CCreature>(iter)->GetCommonStat().iSpeed ==
				dynamic_pointer_cast<CCreature>(m_vCreatures[i])->GetCommonStat().iSpeed)
				&& !dynamic_pointer_cast<CCreature>(m_vCreatures[i])->GetDone())
			{

				dynamic_pointer_cast<CCreature>(iter)->SetTurn(true);

				dynamic_pointer_cast<CCreature>(m_vCreatures[i])->SetTurn(true);
				m_bHero = true;
				return m_vCreatures[i];
			}
		}
		for (auto& iter : m_vMonsters)
		{
			if ((dynamic_pointer_cast<CCreature>(iter)->GetCommonStat().iSpeed ==
				dynamic_pointer_cast<CCreature>(m_vCreatures[i])->GetCommonStat().iSpeed)
				&& !dynamic_pointer_cast<CCreature>(m_vCreatures[i])->GetDone())
			{
				dynamic_pointer_cast<CCreature>(m_vCreatures[i])->SetTurn(true);
				m_bHero = false;
				return m_vCreatures[i];
			}
		}
	}
	
	// ��� �� �����ÿ� ���� ������
	NextTurn();

	return shared_ptr<CGameObject>();
}

void CBattleSystem::CreatureTurnEnd()
{
	dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->SetTurn(false);
	dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->SetDone(true);
	m_pCurrentCreature = NextCreature();
}

void CBattleSystem::NextTurn()
{
	// ��� �ʱ�ȭ
	for (auto& iter : m_vCreatures)
	{
		dynamic_pointer_cast<CCreature>(iter)->SetDone(false);
	}
	m_pCurrentCreature = nullptr;

	m_iTurn++;
}

void CBattleSystem::EndBattle()
{
}

_bool CBattleSystem::HeroesAllDead()
{
	for (auto& iter : m_vHeroes)
	{
		if (dynamic_pointer_cast<CCreature>(iter)->GetCommonStat().iHp > 0)
		{
			return false;
		}
	}
	return true;
}

_bool CBattleSystem::MonstersAllDead()
{
	for (auto& iter : m_vMonsters)
	{
		if (dynamic_pointer_cast<CCreature>(iter)->GetCommonStat().iHp > 0)
		{
			return false;
		}
	}
	return true;
}

void CBattleSystem::CmpBySpeed(vector<shared_ptr<CGameObject>>& _vCreatures)
{
	
	//sort(_vCreatures.begin(), _vCreatures.end());
	
}