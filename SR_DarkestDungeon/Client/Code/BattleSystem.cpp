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
	// ���ο� �� ���۽�
	if (m_pCurrentCreature == nullptr) StartTurn();

	// ���¿� ���� �� ���� ���� ����
	if (!m_bNext && !m_bCounting)
	{
		if (!dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->GetIsCorpse() &&
			!dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->GetIsDeath())
		{
			m_fTime = BATTLEINTERVEL;
		}
		else if (dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->GetIsCorpse())
		{
			m_fTime = CORPSEINTERVEL;
		}
		else
		{
			m_fTime = 0.f;
		}
		m_bCounting = true;
	}
	
	if (!m_bNext && m_bCounting)
	{
		m_fTime -= fTimeDelta;
		if (m_fTime < 0.f)
		{
			m_bNext = true;
			m_bCounting = false;
		}
	}

	// ũ��ó�� ���϶�
	if (dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->GetTurn() && m_bNext)
	{
		// ũ��ó ������ ���� �ൿ
		// ����, ���� �ݿ�, �����̸� ���� �پ���, ������ �׵���
		dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->StartCalculate();
		
		// ���� AI
		if (!dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->GetIsCorpse() &&
			!dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->GetIsDeath() &&
			dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->GetTurn())
		{
			int iNum = rand() % dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->GetSkillNum();
			int iTarget = 0;
			if (m_bHero)
				iTarget = rand() % size(m_vMonsters);
			else
				iTarget = rand() % size(m_vHeroes);

			if (m_bHero)
			{
				dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->AttackCreature
				(dynamic_pointer_cast<CCreature>(m_vMonsters[iTarget]), dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->GetSkill(iNum));
				dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->SetAttacking(true, iNum);
				dynamic_pointer_cast<CCreature>(m_vMonsters[iTarget])->SetHitted(true);
			}
			else
			{
				dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->AttackCreature
				(dynamic_pointer_cast<CCreature>(m_vHeroes[iTarget]), dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->GetSkill(iNum));
				dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->SetAttacking(true, iNum);
				dynamic_pointer_cast<CCreature>(m_vHeroes[iTarget])->SetHitted(true);
			}
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
}

shared_ptr<CGameObject> CBattleSystem::NextCreature()
{
	// �ӵ��� �������� ��������
	CmpBySpeed(m_vCreatures);
	// �������� ���ͱ��� ���� �ӵ��� creature�� ��ȯ
	for (int i = 0; i < size(m_vCreatures); i++)	
	{
		for (int j = 0; j < size(m_vHeroes); j++)
		{
			if ((dynamic_pointer_cast<CCreature>(m_vHeroes[j])->GetCommonStat().iSpeed ==
				dynamic_pointer_cast<CCreature>(m_vCreatures[i])->GetCommonStat().iSpeed)
				&& !(dynamic_pointer_cast<CCreature>(m_vHeroes[j])->GetDone()))
			{
				dynamic_pointer_cast<CCreature>(m_vHeroes[j])->SetTurn(true);
				m_bHero = true;
				return m_vHeroes[j];
			}
		}
		for (int j = 0; j < size(m_vMonsters); j++)
		{
			if ((dynamic_pointer_cast<CCreature>(m_vMonsters[j])->GetCommonStat().iSpeed ==
				dynamic_pointer_cast<CCreature>(m_vCreatures[i])->GetCommonStat().iSpeed)
				&& !(dynamic_pointer_cast<CCreature>(m_vMonsters[j])->GetDone()))
			{
				dynamic_pointer_cast<CCreature>(m_vMonsters[j])->SetTurn(true);
				m_bHero = false;
				return m_vMonsters[j];
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
	for (auto& iter : m_vHeroes)
	{
		dynamic_pointer_cast<CCreature>(iter)->SetDone(false);
	}
	for (auto& iter : m_vMonsters)
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
		if (!dynamic_pointer_cast<CCreature>(iter)->GetIsDeath())
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
		if (!dynamic_pointer_cast<CCreature>(iter)->GetIsDeath())
		{
			return false;
		}
	}
	return true;
}

void CBattleSystem::CmpBySpeed(vector<shared_ptr<CGameObject>>& _vCreatures)
{
	int size = _vCreatures.size();
	for (int i = 0; i < size; i++) {
		for (int j = 1; j < size - i; j++) {
			if (dynamic_pointer_cast<CCreature>(_vCreatures[j])->GetCommonStat().iSpeed >
				dynamic_pointer_cast<CCreature>(_vCreatures[j-1])->GetCommonStat().iSpeed) {
				shared_ptr<CGameObject> pObj = _vCreatures[j];
				_vCreatures[j] = _vCreatures[j-1];
				_vCreatures[j-1] = pObj;
			}
		}
	}
}