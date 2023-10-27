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
	// �����ϸ� �����̴� ��ų �̵���
	if (m_bAttackSkillMoving)
	{
		m_fAttackSkillMovingTime -= fTimeDelta;
		for (auto& iter : m_vHeroes)
		{
			if (dynamic_pointer_cast<CCreature>(iter)->GetAttackMoving())
			{
				dynamic_pointer_cast<CCreature>(iter)->MovePos(dynamic_pointer_cast<CCreature>(iter)->GetTargetPos(),
					fTimeDelta, dynamic_pointer_cast<CCreature>(iter)->GetMovingSpeed());
			}
		}
		for (auto& iter : m_vMonsters)
		{
			if (dynamic_pointer_cast<CCreature>(iter)->GetAttackMoving())
			{
				dynamic_pointer_cast<CCreature>(iter)->MovePos(dynamic_pointer_cast<CCreature>(iter)->GetTargetPos(),
					fTimeDelta, dynamic_pointer_cast<CCreature>(iter)->GetMovingSpeed());
			}
		}

		if (m_fAttackSkillMovingTime < 0.f)
		{
			m_bAttackSkillMoving = false;
			m_bWhileAttack = true;
			m_fAttackSkillMovingTime = ATTACKSKILLMOVINGINTERVEL;

			// ���� ��ġ�� ���ư��� �ӵ� ����ֱ�
			for (auto& iter : m_vHeroes)
			{
				if (dynamic_pointer_cast<CCreature>(iter)->GetAttackMoving())
				{
					dynamic_pointer_cast<CCreature>(iter)->SetMovingSpeed(
						dynamic_pointer_cast<CCreature>(iter)->MovingSpeed
						(dynamic_pointer_cast<CCreature>(iter)->GetTargetPos2(), MOVINGBACKINTERVEL));
				}
			}
			for (auto& iter : m_vMonsters)
			{
				if (dynamic_pointer_cast<CCreature>(iter)->GetAttackMoving())
				{
					dynamic_pointer_cast<CCreature>(iter)->SetMovingSpeed(
						dynamic_pointer_cast<CCreature>(iter)->MovingSpeed
						(dynamic_pointer_cast<CCreature>(iter)->GetTargetPos2(), MOVINGBACKINTERVEL));
				}
			}
		}
	}

	// ���缭 �����ϴ� �ð�
	if (m_bWhileAttack)
	{
		m_fWhileAttackingTime -= fTimeDelta;
		if (m_fWhileAttackingTime < 0.f)
		{
			m_bWhileAttack = false;
			m_bBackMoving = true;
			m_fWhileAttackingTime = WHILEATTACKINTERVEL;
		}
	}

	// �����ϸ� �����̴� ��ų �̵� ���Ŀ� ���ư���
	if (m_bBackMoving)
	{
		m_fBackMovingTime -= fTimeDelta;
		for (auto& iter : m_vHeroes)
		{
			if (dynamic_pointer_cast<CCreature>(iter)->GetAttackMoving())
			{
				dynamic_pointer_cast<CCreature>(iter)->MovePos(dynamic_pointer_cast<CCreature>(iter)->GetTargetPos2(),
					fTimeDelta, dynamic_pointer_cast<CCreature>(iter)->GetMovingSpeed());
			}
		}
		for (auto& iter : m_vMonsters)
		{
			if (dynamic_pointer_cast<CCreature>(iter)->GetAttackMoving())
			{
				dynamic_pointer_cast<CCreature>(iter)->MovePos(dynamic_pointer_cast<CCreature>(iter)->GetTargetPos2(),
					fTimeDelta, dynamic_pointer_cast<CCreature>(iter)->GetMovingSpeed());
			}
		}

		if (m_fBackMovingTime < 0.f)
		{
			m_bBackMoving = false;
			m_fBackMovingTime = MOVINGBACKINTERVEL;
			for (auto& iter : m_vHeroes)
			{
				dynamic_pointer_cast<CCreature>(iter)->SetAttackMoving(false);
			}
			for (auto& iter : m_vMonsters)
			{
				dynamic_pointer_cast<CCreature>(iter)->SetAttackMoving(false);
			}
		}
	}

	// ���� �ֵ� üũ
	if (m_bDeadCheck && !m_bAttackSkillMoving && !m_bWhileAttack)
	{
		DeadCheck();
		m_bDeadCheck = false;
	}

	// ������ ������ ��ġ ����
	if (m_bDeathMoving && !m_bAttackSkillMoving && !m_bWhileAttack)
	{
		m_fDeathMovingTime -= fTimeDelta;
		for (auto& iter : m_vHeroes)
		{
			if (dynamic_pointer_cast<CCreature>(iter)->GetMoving())
			{
				dynamic_pointer_cast<CCreature>(iter)->MovePos(dynamic_pointer_cast<CCreature>(iter)->GetTargetPos() - _vec3(0.f, 0.f, 1.f),
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
	if (m_bSkillMoving && !m_bAttackSkillMoving && !m_bWhileAttack)
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
	if (dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->GetTurn()
		&& m_bNext && !m_bDeathMoving && !m_bAttackSkillMoving && !m_bWhileAttack)
	{
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
				// �ٰ����� ��ų�� ��쿡 �̵��ϰ�
				if (dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->GetSkill(iNum)->IsApproach())
				{
					dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->SetAttackMoving(true);
					dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->SetTargetPos(m_vMonsterLocation[iTarget] + m_vApproachingGapR);
					dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->SetTargetPos2(m_vHeroLocation[iCurrentHeroIndex]);
					dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->SetMovingSpeed(
						dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->MovingSpeed(m_vMonsters[iTarget]->GetPos() + m_vApproachingGapR, ATTACKSKILLMOVINGINTERVEL));
					m_bAttackSkillMoving = true;
				}

				// ������ ������ ���
				if (dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->GetSkill(iNum)->IsTargetAll())
				{
					for (int i = 0; i < size(m_vMonsters); i++)
					{
						dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->AttackCreature
						(dynamic_pointer_cast<CCreature>(m_vMonsters[i]), dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->GetSkill(iNum));
						dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->SetAttacking(true, iNum);
					}
				}
				// �̵� ������ ���
				else if (dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->GetSkill(iNum)->GetMoveCnt() != 0)
				{
					dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->AttackCreature
					(dynamic_pointer_cast<CCreature>(m_vMonsters[iTarget]), dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->GetSkill(iNum));
					dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->SetAttacking(true, iNum);

					if (dynamic_pointer_cast<CCreature>(m_vMonsters[iTarget])->GetHitted())
					{
						int iMovePos = dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->GetSkill(iNum)->GetMoveCnt();
						if (dynamic_pointer_cast<CCreature>(m_vMonsters[iTarget + iMovePos]) &&
							!dynamic_pointer_cast<CCreature>(m_vMonsters[iTarget + iMovePos])->GetIsDeath() &&
							!dynamic_pointer_cast<CCreature>(m_vMonsters[iTarget])->GetIsDeath())
						{
							dynamic_pointer_cast<CCreature>(m_vMonsters[iTarget])->SetMoving(true);
							dynamic_pointer_cast<CCreature>(m_vMonsters[iTarget])->SetTargetPos(m_vMonsterLocation[iTarget + iMovePos]);
							dynamic_pointer_cast<CCreature>(m_vMonsters[iTarget])->SetMovingSpeed(
								dynamic_pointer_cast<CCreature>(m_vMonsters[iTarget])->MovingSpeed(m_vMonsters[iTarget + iMovePos]->GetPos(), SKILLMOVINGINTERVEL));

							dynamic_pointer_cast<CCreature>(m_vMonsters[iTarget + iMovePos])->SetMoving(true);
							dynamic_pointer_cast<CCreature>(m_vMonsters[iTarget + iMovePos])->SetTargetPos(m_vMonsterLocation[iTarget]);
							dynamic_pointer_cast<CCreature>(m_vMonsters[iTarget + iMovePos])->SetMovingSpeed(
								dynamic_pointer_cast<CCreature>(m_vMonsters[iTarget + iMovePos])->MovingSpeed(m_vMonsters[iTarget]->GetPos(), SKILLMOVINGINTERVEL));
							m_bSkillMoving = true;
							shared_ptr<CGameObject> pObj = m_vMonsters[iTarget];
							m_vMonsters[iTarget] = m_vMonsters[iTarget + iMovePos];
							m_vMonsters[iTarget + iMovePos] = pObj;
						}
					}
				}
				else
				{
					dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->AttackCreature
					(dynamic_pointer_cast<CCreature>(m_vMonsters[iTarget]), dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->GetSkill(iNum));
					dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->SetAttacking(true, iNum);
				}
			}
			else
			{
				// �ٰ����� ��ų�� ��쿡 �̵��ϰ�
				if (dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->GetSkill(iNum)->IsApproach())
				{
					dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->SetAttackMoving(true);
					dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->SetTargetPos(m_vHeroLocation[iTarget] + m_vApproachingGapL);
					dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->SetTargetPos2(m_vMonsterLocation[iCurrentMonsterIndex]);
					dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->SetMovingSpeed(
						dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->MovingSpeed(m_vHeroes[iTarget]->GetPos() + m_vApproachingGapL, ATTACKSKILLMOVINGINTERVEL));
					m_bAttackSkillMoving = true;
				}

				// ������ ������ ���
				if (dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->GetSkill(iNum)->IsTargetAll())
				{
					for (int i = 0; i < size(m_vHeroes); i++)
					{
						dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->AttackCreature
						(dynamic_pointer_cast<CCreature>(m_vHeroes[i]), dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->GetSkill(iNum));
						dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->SetAttacking(true, iNum);
					}
				}
				// �̵� ������ ���
				else if (dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->GetSkill(iNum)->GetMoveCnt() != 0)
				{
					dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->AttackCreature
					(dynamic_pointer_cast<CCreature>(m_vHeroes[iTarget]), dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->GetSkill(iNum));
					dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->SetAttacking(true, iNum);

					if (dynamic_pointer_cast<CCreature>(m_vHeroes[iTarget])->GetHitted())
					{
						int iMovePos = dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->GetSkill(iNum)->GetMoveCnt();
						if (dynamic_pointer_cast<CCreature>(m_vHeroes[iTarget + iMovePos]) &&
							!dynamic_pointer_cast<CCreature>(m_vHeroes[iTarget + iMovePos])->GetIsDeath() &&
							!dynamic_pointer_cast<CCreature>(m_vHeroes[iTarget])->GetIsDeath())
						{
							dynamic_pointer_cast<CCreature>(m_vHeroes[iTarget])->SetMoving(true);
							dynamic_pointer_cast<CCreature>(m_vHeroes[iTarget])->SetTargetPos(m_vHeroLocation[iTarget + iMovePos]);
							dynamic_pointer_cast<CCreature>(m_vHeroes[iTarget])->SetMovingSpeed(
								dynamic_pointer_cast<CCreature>(m_vHeroes[iTarget])->MovingSpeed(m_vHeroes[iTarget + iMovePos]->GetPos(), SKILLMOVINGINTERVEL));

							dynamic_pointer_cast<CCreature>(m_vHeroes[iTarget + iMovePos])->SetMoving(true);
							dynamic_pointer_cast<CCreature>(m_vHeroes[iTarget + iMovePos])->SetTargetPos(m_vHeroLocation[iTarget]);
							dynamic_pointer_cast<CCreature>(m_vHeroes[iTarget + iMovePos])->SetMovingSpeed(
								dynamic_pointer_cast<CCreature>(m_vHeroes[iTarget + iMovePos])->MovingSpeed(m_vHeroes[iTarget]->GetPos(), SKILLMOVINGINTERVEL));
							m_bSkillMoving = true;
							shared_ptr<CGameObject> pObj = m_vHeroes[iTarget];
							m_vHeroes[iTarget] = m_vHeroes[iTarget + iMovePos];
							m_vHeroes[iTarget + iMovePos] = pObj;
						}
					}
				}
				else
				{
					dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->AttackCreature
					(dynamic_pointer_cast<CCreature>(m_vHeroes[iTarget]), dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->GetSkill(iNum));
					dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->SetAttacking(true, iNum);
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
				iCurrentHeroIndex = j;
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
				iCurrentMonsterIndex = j;
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
	m_bDeadCheck = true;
	//DeadCheck();
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
				dynamic_pointer_cast<CCreature>(m_vHeroes[j])->SetTargetPos(m_vHeroLocation[j - 1]);
				dynamic_pointer_cast<CCreature>(m_vHeroes[j])->SetMovingSpeed(
					dynamic_pointer_cast<CCreature>(m_vHeroes[j])->MovingSpeed(m_vHeroLocation[j - 1], DEATHMOVINGINTERVEL));
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
				dynamic_pointer_cast<CCreature>(m_vMonsters[j])->SetTargetPos(m_vMonsterLocation[j - 1]);
				dynamic_pointer_cast<CCreature>(m_vMonsters[j])->SetMovingSpeed(
					dynamic_pointer_cast<CCreature>(m_vMonsters[j])->MovingSpeed(m_vMonsterLocation[j - 1], DEATHMOVINGINTERVEL));
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
			m_vHeroLocation.push_back(_vec3(_vOrigin.x - 6.f, _vOrigin.y, _vOrigin.z));
			break;
		case 1:
			dynamic_pointer_cast<CCreature>(_vHeroes[1])->SetPos(_vec3(_vOrigin.x - 8.f, _vOrigin.y, _vOrigin.z - 2.f));
			dynamic_pointer_cast<CCreature>(_vHeroes[1])->SetAngle(_vec3(0.f, _fAngle, 0.f));
			m_vHeroLocation.push_back(_vec3(_vOrigin.x - 8.f, _vOrigin.y, _vOrigin.z - 2.f));
			break;
		case 2:
			dynamic_pointer_cast<CCreature>(_vHeroes[2])->SetPos(_vec3(_vOrigin.x - 14.f, _vOrigin.y, _vOrigin.z + 4.f));
			dynamic_pointer_cast<CCreature>(_vHeroes[2])->SetAngle(_vec3(0.f, _fAngle, 0.f));
			m_vHeroLocation.push_back(_vec3(_vOrigin.x - 14.f, _vOrigin.y, _vOrigin.z + 4.f));
			break;
		case 3:
			dynamic_pointer_cast<CCreature>(_vHeroes[3])->SetPos(_vec3(_vOrigin.x - 16.f, _vOrigin.y, _vOrigin.z + 2.f));
			dynamic_pointer_cast<CCreature>(_vHeroes[3])->SetAngle(_vec3(0.f, _fAngle, 0.f));
			m_vHeroLocation.push_back(_vec3(_vOrigin.x - 16.f, _vOrigin.y, _vOrigin.z + 2.f));
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
			m_vMonsterLocation.push_back(_vec3(_vOrigin.x + 6.f, _vOrigin.y, _vOrigin.z));
			break;
		case 1:
			dynamic_pointer_cast<CCreature>(_vMonsters[1])->SetPos(_vec3(_vOrigin.x + 8.f, _vOrigin.y, _vOrigin.z - 2.f));
			dynamic_pointer_cast<CCreature>(_vMonsters[1])->SetAngle(_vec3(0.f, _fAngle + PI, 0.f));
			m_vMonsterLocation.push_back(_vec3(_vOrigin.x + 8.f, _vOrigin.y, _vOrigin.z - 2.f));
		case 2:
			dynamic_pointer_cast<CCreature>(_vMonsters[2])->SetPos(_vec3(_vOrigin.x + 14.f, _vOrigin.y, _vOrigin.z + 4.f));
			dynamic_pointer_cast<CCreature>(_vMonsters[2])->SetAngle(_vec3(0.f, _fAngle + PI, 0.f));
			m_vMonsterLocation.push_back(_vec3(_vOrigin.x + 14.f, _vOrigin.y, _vOrigin.z + 4.f));
			break;
		case 3:
			dynamic_pointer_cast<CCreature>(_vMonsters[3])->SetPos(_vec3(_vOrigin.x + 16.f, _vOrigin.y, _vOrigin.z + 2.f));
			dynamic_pointer_cast<CCreature>(_vMonsters[3])->SetAngle(_vec3(0.f, _fAngle + PI, 0.f));
			m_vMonsterLocation.push_back(_vec3(_vOrigin.x + 16.f, _vOrigin.y, _vOrigin.z + 2.f));
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