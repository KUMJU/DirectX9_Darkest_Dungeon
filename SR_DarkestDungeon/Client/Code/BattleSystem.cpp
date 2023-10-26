#include "pch.h"
#include "BattleSystem.h"
#include "Creature.h"
#include "Export_System.h"
#include "Export_Utility.h"

CBattleSystem::CBattleSystem()
{
}

CBattleSystem::~CBattleSystem()
{
}

void CBattleSystem::Update(const _float& fTimeDelta)
{
	// ������ ������ ��ġ ����
	if (m_bDeathMoving)
	{
		m_fDeathMovingTime -= fTimeDelta;
		for (auto& iter : m_vHeroes)
		{
			if (dynamic_pointer_cast<CCreature>(iter)->GetMoving())
			{
				dynamic_pointer_cast<CCreature>(iter)->MovePos(dynamic_pointer_cast<CCreature>(iter)->GetTargetPos(),
					fTimeDelta, dynamic_pointer_cast<CCreature>(iter)->GetMovingSpeed());
			}
		}
		for (auto& iter : m_vMonsters)
		{
			if (dynamic_pointer_cast<CCreature>(iter)->GetMoving())
			{
				dynamic_pointer_cast<CCreature>(iter)->MovePos(dynamic_pointer_cast<CCreature>(iter)->GetTargetPos(),
					fTimeDelta, dynamic_pointer_cast<CCreature>(iter)->GetMovingSpeed());
			}
		}

		if (m_fDeathMovingTime < 0.f)
		{
			m_bDeathMoving = false;
			m_fDeathMovingTime = DEATHMOVINGINTERVEL;
			for (auto& iter : m_vHeroes)
			{
				dynamic_pointer_cast<CCreature>(iter)->SetMoving(false);
			}
			for (auto& iter : m_vMonsters)
			{
				dynamic_pointer_cast<CCreature>(iter)->SetMoving(false);
			}
		}
	}

	// �����̴� ��ų ��ġ ����
	if (m_bSkillMoving)
	{
		m_fSkillMovingTime -= fTimeDelta;
		for (auto& iter : m_vHeroes)
		{
			if (dynamic_pointer_cast<CCreature>(iter)->GetMoving())
			{
				dynamic_pointer_cast<CCreature>(iter)->MovePos(dynamic_pointer_cast<CCreature>(iter)->GetTargetPos(),
					fTimeDelta, dynamic_pointer_cast<CCreature>(iter)->GetMovingSpeed());
			}
		}
		for (auto& iter : m_vMonsters)
		{
			if (dynamic_pointer_cast<CCreature>(iter)->GetMoving())
			{
				dynamic_pointer_cast<CCreature>(iter)->MovePos(dynamic_pointer_cast<CCreature>(iter)->GetTargetPos(),
					fTimeDelta, dynamic_pointer_cast<CCreature>(iter)->GetMovingSpeed());
			}
		}

		if (m_fSkillMovingTime < 0.f)
		{
			m_bSkillMoving = false;
			m_fSkillMovingTime = SKILLMOVINGINTERVEL;
			for (auto& iter : m_vHeroes)
			{
				dynamic_pointer_cast<CCreature>(iter)->SetMoving(false);
			}
			for (auto& iter : m_vMonsters)
			{
				dynamic_pointer_cast<CCreature>(iter)->SetMoving(false);
			}
		}
	}

	// ���ο� �� ���۽�
	if (m_pCurrentCreature == nullptr) StartTurn();

	// ���¿� ���� �� ���� ���� ����
	if (!m_bNext && !m_bCounting)
	{
		if (!dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->GetIsCorpse() &&
			!dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->GetIsDeath())
		{
			m_fBattleTime = BATTLEINTERVEL;
		}
		else if (dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->GetIsCorpse())
		{
			m_fBattleTime = CORPSEINTERVEL;
		}
		else
		{
			m_fBattleTime = 0.f;
		}
		m_bCounting = true;
	}

	if (!m_bNext && m_bCounting)
	{
		m_fBattleTime -= fTimeDelta;
		if (m_fBattleTime < 0.f)
		{
			m_bNext = true;
			m_bCounting = false;
		}
	}

	// ũ��ó�� ���϶�
	if (dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->GetTurn() && m_bNext && !m_bDeathMoving)
	{
		DeadCheck();
		// ũ��ó ������ ���� �ൿ
		// ����, ���� �ݿ�, �����̸� ���� �پ���, ������ �������·�
		dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->StartCalculate();

		// ���� ����
		if (HeroesAllDead() || MonstersAllDead())
			EndBattle();

		// ���� AI
		if (!dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->GetIsCorpse() &&
			!dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->GetIsDeath() &&
			dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->GetTurn())
		{
			int iNum = rand() % dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->GetSkillNum();
			int iTarget = 0;

			// ���� �ֵ� ���ֱ�
			int iDeathHeroes = 0;
			for (auto& iter : m_vHeroes)
			{
				if (dynamic_pointer_cast<CCreature>(iter)->GetIsDeath())
				{
					iDeathHeroes++;
				}
			}
			int iDeathMonsters = 0;
			for (auto& iter : m_vMonsters)
			{
				if (dynamic_pointer_cast<CCreature>(iter)->GetIsDeath())
				{
					iDeathMonsters++;
				}
			}

			// ������� ���� ���� �������ϱ�
			if (m_bHero)
			{
				while (1)
				{
					if (HeroesAllDead() || MonstersAllDead())
					{
						EndBattle();
						break;
					}
					iTarget = rand() % (size(m_vMonsters) - iDeathMonsters);
					if (!dynamic_pointer_cast<CCreature>(m_vMonsters[iTarget])->GetIsDeath() &&
						dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->GetSkill(iNum)->GetTargetPos()[iTarget])
					{
						break;
					}
				}
			}
			else
			{
				while (1)
				{
					if (HeroesAllDead() || MonstersAllDead())
					{
						EndBattle();
						break;
					}
					iTarget = rand() % (size(m_vHeroes) - iDeathHeroes);
					if (!dynamic_pointer_cast<CCreature>(m_vHeroes[iTarget])->GetIsDeath() &&
						dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->GetSkill(iNum)->GetTargetPos()[iTarget])
					{
						break;
					}
				}
			}

			if (m_bHero)
			{
				// ������ ������ ���
				if (dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->GetSkill(iNum)->IsTargetAll())
				{
					for (int i = 0; i < size(m_vMonsters); i++)
					{
						dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->AttackCreature
						(dynamic_pointer_cast<CCreature>(m_vMonsters[i]), dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->GetSkill(iNum));
						dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->SetAttacking(true, iNum);
						dynamic_pointer_cast<CCreature>(m_vMonsters[i])->SetHitted(true);
					}
				}
				// �̵� ������ ���
				else if (dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->GetSkill(iNum)->GetMoveCnt() != 0)
				{
					dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->AttackCreature
					(dynamic_pointer_cast<CCreature>(m_vMonsters[iTarget]), dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->GetSkill(iNum));
					dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->SetAttacking(true, iNum);
					dynamic_pointer_cast<CCreature>(m_vMonsters[iTarget])->SetHitted(true);

					int iMovePos = dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->GetSkill(iNum)->GetMoveCnt();
					if (dynamic_pointer_cast<CCreature>(m_vMonsters[iTarget + iMovePos]) &&
						!dynamic_pointer_cast<CCreature>(m_vMonsters[iTarget + iMovePos])->GetIsDeath() &&
						!dynamic_pointer_cast<CCreature>(m_vMonsters[iTarget])->GetIsDeath())
					{
						dynamic_pointer_cast<CCreature>(m_vMonsters[iTarget])->SetMoving(true);
						dynamic_pointer_cast<CCreature>(m_vMonsters[iTarget])->SetTargetPos(m_vMonsters[iTarget + iMovePos]->GetPos());
						dynamic_pointer_cast<CCreature>(m_vMonsters[iTarget])->SetMovingSpeed(
							dynamic_pointer_cast<CCreature>(m_vMonsters[iTarget])->MovingSpeed(m_vMonsters[iTarget + iMovePos]->GetPos(), SKILLMOVINGINTERVEL));

						dynamic_pointer_cast<CCreature>(m_vMonsters[iTarget + iMovePos])->SetMoving(true);
						dynamic_pointer_cast<CCreature>(m_vMonsters[iTarget + iMovePos])->SetTargetPos(m_vMonsters[iTarget]->GetPos());
						dynamic_pointer_cast<CCreature>(m_vMonsters[iTarget + iMovePos])->SetMovingSpeed(
							dynamic_pointer_cast<CCreature>(m_vMonsters[iTarget + iMovePos])->MovingSpeed(m_vMonsters[iTarget]->GetPos(), SKILLMOVINGINTERVEL));
						m_bSkillMoving = true;
						shared_ptr<CGameObject> pObj = m_vMonsters[iTarget];
						m_vMonsters[iTarget] = m_vMonsters[iTarget + iMovePos];
						m_vMonsters[iTarget + iMovePos] = pObj;
					}
				}
				else
				{
					dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->AttackCreature
					(dynamic_pointer_cast<CCreature>(m_vMonsters[iTarget]), dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->GetSkill(iNum));
					dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->SetAttacking(true, iNum);
					dynamic_pointer_cast<CCreature>(m_vMonsters[iTarget])->SetHitted(true);
				}
			}
			else
			{
				// ������ ������ ���
				if (dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->GetSkill(iNum)->IsTargetAll())
				{
					for (int i = 0; i < size(m_vHeroes); i++)
					{
						dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->AttackCreature
						(dynamic_pointer_cast<CCreature>(m_vHeroes[i]), dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->GetSkill(iNum));
						dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->SetAttacking(true, iNum);
						dynamic_pointer_cast<CCreature>(m_vHeroes[i])->SetHitted(true);
					}
				}
				// �̵� ������ ���
				else if (dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->GetSkill(iNum)->GetMoveCnt() != 0)
				{
					dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->AttackCreature
					(dynamic_pointer_cast<CCreature>(m_vHeroes[iTarget]), dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->GetSkill(iNum));
					dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->SetAttacking(true, iNum);
					dynamic_pointer_cast<CCreature>(m_vHeroes[iTarget])->SetHitted(true);

					int iMovePos = dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->GetSkill(iNum)->GetMoveCnt();
					if (dynamic_pointer_cast<CCreature>(m_vHeroes[iTarget + iMovePos]) &&
						!dynamic_pointer_cast<CCreature>(m_vHeroes[iTarget + iMovePos])->GetIsDeath() &&
						!dynamic_pointer_cast<CCreature>(m_vHeroes[iTarget])->GetIsDeath())
					{
						dynamic_pointer_cast<CCreature>(m_vHeroes[iTarget])->SetMoving(true);
						dynamic_pointer_cast<CCreature>(m_vHeroes[iTarget])->SetTargetPos(m_vHeroes[iTarget + iMovePos]->GetPos());
						dynamic_pointer_cast<CCreature>(m_vHeroes[iTarget])->SetMovingSpeed(
							dynamic_pointer_cast<CCreature>(m_vHeroes[iTarget])->MovingSpeed(m_vHeroes[iTarget + iMovePos]->GetPos(), SKILLMOVINGINTERVEL));

						dynamic_pointer_cast<CCreature>(m_vHeroes[iTarget + iMovePos])->SetMoving(true);
						dynamic_pointer_cast<CCreature>(m_vHeroes[iTarget + iMovePos])->SetTargetPos(m_vHeroes[iTarget]->GetPos());
						dynamic_pointer_cast<CCreature>(m_vHeroes[iTarget + iMovePos])->SetMovingSpeed(
							dynamic_pointer_cast<CCreature>(m_vHeroes[iTarget + iMovePos])->MovingSpeed(m_vHeroes[iTarget]->GetPos(), SKILLMOVINGINTERVEL));
						m_bSkillMoving = true;
						shared_ptr<CGameObject> pObj = m_vHeroes[iTarget];
						m_vHeroes[iTarget] = m_vHeroes[iTarget + iMovePos];
						m_vHeroes[iTarget + iMovePos] = pObj;
					}
				}
				else
				{
					dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->AttackCreature
					(dynamic_pointer_cast<CCreature>(m_vHeroes[iTarget]), dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->GetSkill(iNum));
					dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->SetAttacking(true, iNum);
					dynamic_pointer_cast<CCreature>(m_vHeroes[iTarget])->SetHitted(true);
				}
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
	// ������ üũ
	DeadCheck();
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
	// ���� ���� ��ü ���� ���ֱ�
	if (HeroesAllDead())
	{
		for (auto& iter : m_vHeroes)
		{
			dynamic_pointer_cast<CCreature>(iter)->SetHp(-100.f);
		}
	}
	else
	{
		for (auto& iter : m_vMonsters)
		{
			dynamic_pointer_cast<CCreature>(iter)->SetHp(-100.f);
		}
	}
	
	int a = 3;
}

_bool CBattleSystem::HeroesAllDead()
{
	for (auto& iter : m_vHeroes)
	{
		if (!((dynamic_pointer_cast<CCreature>(iter)->GetIsCorpse()) ||
			(dynamic_pointer_cast<CCreature>(iter)->GetIsDeath())))
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
		if (!((dynamic_pointer_cast<CCreature>(iter)->GetIsCorpse()) ||
			(dynamic_pointer_cast<CCreature>(iter)->GetIsDeath())))
		{
			return false;
		}
	}
	return true;
}

void CBattleSystem::DeadCheck()
{
	// ������ ������ ������ ����ֱ�
	for (int i = size(m_vHeroes) - 2; i >= 0; i--)
	{
		if (dynamic_pointer_cast<CCreature>(m_vHeroes[i])->GetIsDeath())
		{
			for (int j = i + 1; j < size(m_vHeroes); j++)
			{
				dynamic_pointer_cast<CCreature>(m_vHeroes[j])->SetMoving(true);
				dynamic_pointer_cast<CCreature>(m_vHeroes[j])->SetTargetPos(m_vHeroes[j - 1]->GetPos());
				dynamic_pointer_cast<CCreature>(m_vHeroes[j])->SetMovingSpeed(
					dynamic_pointer_cast<CCreature>(m_vHeroes[j])->MovingSpeed(m_vHeroes[j - 1]->GetPos(), DEATHMOVINGINTERVEL));
			}

			for (int j = i + 1; j < size(m_vHeroes); j++)
			{
				shared_ptr<CGameObject> pObj = m_vHeroes[j - 1];
				m_vHeroes[j - 1] = m_vHeroes[j];
				m_vHeroes[j] = pObj;
			}

			m_bDeathMoving = true;
		}
	}

	for (int i = size(m_vMonsters) - 2; i >= 0; i--)
	{
		if (dynamic_pointer_cast<CCreature>(m_vMonsters[i])->GetIsDeath())
		{
			for (int j = i + 1; j < size(m_vMonsters); j++)
			{
				dynamic_pointer_cast<CCreature>(m_vMonsters[j])->SetMoving(true);
				dynamic_pointer_cast<CCreature>(m_vMonsters[j])->SetTargetPos(m_vMonsters[j - 1]->GetPos());
				dynamic_pointer_cast<CCreature>(m_vMonsters[j])->SetMovingSpeed(
					dynamic_pointer_cast<CCreature>(m_vMonsters[j])->MovingSpeed(m_vMonsters[j - 1]->GetPos(), DEATHMOVINGINTERVEL));
			}

			for (int j = i + 1; j < size(m_vMonsters); j++)
			{
				shared_ptr<CGameObject> pObj = m_vMonsters[j - 1];
				m_vMonsters[j - 1] = m_vMonsters[j];
				m_vMonsters[j] = pObj;
			}

			m_bDeathMoving = true;
		}
	}
}

void CBattleSystem::FormBattlePosition(vector<shared_ptr<CGameObject>>& _vHeroes, vector<shared_ptr<CGameObject>>& _vMonsters, _float _fAngle, _vec3 _vOrigin)
{
	for (int i = 0; i < size(_vHeroes); i++)
	{
		switch (i)
		{
		case 0:
			dynamic_pointer_cast<CCreature>(_vHeroes[0])->SetPos(_vec3(_vOrigin.x - 6.f, _vOrigin.y, _vOrigin.z));
			dynamic_pointer_cast<CCreature>(_vHeroes[0])->SetAngle(_vec3(0.f, _fAngle, 0.f));
			break;
		case 1:
			dynamic_pointer_cast<CCreature>(_vHeroes[1])->SetPos(_vec3(_vOrigin.x - 8.f, _vOrigin.y, _vOrigin.z - 2.f));
			dynamic_pointer_cast<CCreature>(_vHeroes[1])->SetAngle(_vec3(0.f, _fAngle, 0.f));
			break;
		case 2:
			dynamic_pointer_cast<CCreature>(_vHeroes[2])->SetPos(_vec3(_vOrigin.x - 14.f, _vOrigin.y, _vOrigin.z + 4.f));
			dynamic_pointer_cast<CCreature>(_vHeroes[2])->SetAngle(_vec3(0.f, _fAngle, 0.f));
			break;
		case 3:
			dynamic_pointer_cast<CCreature>(_vHeroes[3])->SetPos(_vec3(_vOrigin.x - 16.f, _vOrigin.y, _vOrigin.z + 2.f));
			dynamic_pointer_cast<CCreature>(_vHeroes[3])->SetAngle(_vec3(0.f, _fAngle, 0.f));
			break;
		}
	}

	for (int i = 0; i < size(_vMonsters); i++)
	{
		switch (i)
		{
		case 0:
			dynamic_pointer_cast<CCreature>(_vMonsters[0])->SetPos(_vec3(_vOrigin.x + 6.f, _vOrigin.y, _vOrigin.z));
			dynamic_pointer_cast<CCreature>(_vMonsters[0])->SetAngle(_vec3(0.f, _fAngle + PI, 0.f));
			break;
		case 1:
			dynamic_pointer_cast<CCreature>(_vMonsters[1])->SetPos(_vec3(_vOrigin.x + 8.f, _vOrigin.y, _vOrigin.z - 2.f));
			dynamic_pointer_cast<CCreature>(_vMonsters[1])->SetAngle(_vec3(0.f, _fAngle + PI, 0.f));
		case 2:
			dynamic_pointer_cast<CCreature>(_vMonsters[2])->SetPos(_vec3(_vOrigin.x + 14.f, _vOrigin.y, _vOrigin.z + 4.f));
			dynamic_pointer_cast<CCreature>(_vMonsters[2])->SetAngle(_vec3(0.f, _fAngle + PI, 0.f));
			break;
		case 3:
			dynamic_pointer_cast<CCreature>(_vMonsters[3])->SetPos(_vec3(_vOrigin.x + 16.f, _vOrigin.y, _vOrigin.z + 2.f));
			dynamic_pointer_cast<CCreature>(_vMonsters[3])->SetAngle(_vec3(0.f, _fAngle + PI, 0.f));
			break;
		}
	}
}

void CBattleSystem::CmpBySpeed(vector<shared_ptr<CGameObject>>& _vCreatures)
{
	int size = _vCreatures.size();
	for (int i = 0; i < size; i++) {
		for (int j = 1; j < size - i; j++) {
			if (dynamic_pointer_cast<CCreature>(_vCreatures[j])->GetCommonStat().iSpeed >
				dynamic_pointer_cast<CCreature>(_vCreatures[j - 1])->GetCommonStat().iSpeed) {
				shared_ptr<CGameObject> pObj = _vCreatures[j];
				_vCreatures[j] = _vCreatures[j - 1];
				_vCreatures[j - 1] = pObj;
			}
		}
	}
}