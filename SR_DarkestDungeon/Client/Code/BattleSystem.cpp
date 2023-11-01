#include "pch.h"
#include "BattleSystem.h"
#include "Creature.h"
#include "Export_System.h"
#include "Export_Utility.h"
#include"BattleHeroUI.h"
#include"Hero.h"

CBattleSystem::CBattleSystem()
{
}

CBattleSystem::~CBattleSystem()
{
}

void CBattleSystem::Update(const _float& fTimeDelta)
{
	// 자동전투 여부
	AutoBattleKeyInput();

	// 자리 스위칭 이동중
	if (m_bSwitching)
	{
		m_fSwitchingTime -= fTimeDelta;
		for (auto& iter : m_vHeroes)
		{
			if (dynamic_pointer_cast<CCreature>(iter)->GetMoving())
			{
				dynamic_pointer_cast<CCreature>(iter)->MovePos(dynamic_pointer_cast<CCreature>(iter)->GetTargetPos(),
					fTimeDelta, dynamic_pointer_cast<CCreature>(iter)->GetMovingSpeed());
			}
		}

		if (m_fSwitchingTime < 0.f)
		{
			m_bSwitching = false;
			m_fSwitchingTime = SWITCHINGINTERVEL;
			for (auto& iter : m_vHeroes)
			{
				dynamic_pointer_cast<CCreature>(iter)->SetAttackMoving(false);
			}
		}
	}

	// 공격하며 움직이는 스킬 이동중
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

			// 원래 위치로 돌아가는 속도 잡아주기
			for (auto& iter : m_vHeroes)
			{
				if (dynamic_pointer_cast<CCreature>(iter)->GetAttackMoving())
				{
					iter->SetPos(dynamic_pointer_cast<CCreature>(iter)->GetTargetPos());
					dynamic_pointer_cast<CCreature>(iter)->SetMovingSpeed(
						dynamic_pointer_cast<CCreature>(iter)->MovingSpeed
						(dynamic_pointer_cast<CCreature>(iter)->GetTargetPos2(), MOVINGBACKINTERVEL));
				}
			}
			for (auto& iter : m_vMonsters)
			{
				if (dynamic_pointer_cast<CCreature>(iter)->GetAttackMoving())
				{
					iter->SetPos(dynamic_pointer_cast<CCreature>(iter)->GetTargetPos());
					dynamic_pointer_cast<CCreature>(iter)->SetMovingSpeed(
						dynamic_pointer_cast<CCreature>(iter)->MovingSpeed
						(dynamic_pointer_cast<CCreature>(iter)->GetTargetPos2(), MOVINGBACKINTERVEL));
				}
			}
		}
	}

	// 멈춰서 공격하는 시간
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

	// 공격하며 움직이는 스킬 이동 이후에 돌아가기
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

	// 죽은 애들 체크
	if (m_bDeadCheck && !m_bAttackSkillMoving && !m_bWhileAttack)
	{
		DeadCheck();
		m_bDeadCheck = false;
	}

	// 죽은애 있으면 위치 갱신
	if (m_bDeathMoving && !m_bAttackSkillMoving && !m_bWhileAttack)
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

	// 움직이는 스킬 위치 갱신
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

	// 스킬 쓴애가 움직이는 위치 갱신
	if (m_bSkillMove && !m_bAttackSkillMoving && !m_bWhileAttack)
	{
		m_fSkillMoveTime -= fTimeDelta;
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

		if (m_fSkillMoveTime < 0.f)
		{
			m_bSkillMove = false;
			m_fSkillMoveTime = SKILLMOVEINTERVEL;
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

	// 새로운 턴 시작시
	if (m_pCurrentCreature == nullptr) StartTurn();

	// 상태에 따른 턴 사이 간격 설정
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
			// 배틀 ui 끄기, 턴 ui 끄기
			dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->OffTurnCursor();
			if (m_bHero)
			{
				if (m_pHeroUI)
					m_pHeroUI->SetVisible(false);
			}

			m_pCurrentCreature = NextCreature();
			m_bNext = true;
			m_bCounting = false;
		}
	}

	// 크리처의 턴일때
	if (m_bNext && dynamic_pointer_cast<CCreature>(m_pCurrentCreature) &&
		dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->GetTurn()
		&& !m_bDeathMoving && !m_bAttackSkillMoving && !m_bWhileAttack)
	{
		// 턴 커서 키기
		dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->OnTurnCursor();

		// 배틀 ui 키기
		if (m_bHero)
		{
			shared_ptr<CHero> pHero = dynamic_pointer_cast<CHero>(m_pCurrentCreature);
			tstring HeroName;

			//UI Setting
			switch (pHero->GetHeroType())
			{
			case EHeroType::VESTAL:
				HeroName = L"VESTAL";
				break;
			case EHeroType::HIGHWAYMAN:
				HeroName = L"HIGHWAYMAN";
				break;
			case EHeroType::SHILEDBREAKER:
				HeroName = L"SHILEDBREAKER";
				break;
			case EHeroType::JESTER:
				HeroName = L"JESTER";
				break;
			default:
				break;
			}

			if (m_pHeroUI)
			{
				m_pHeroUI->SetVisible(true);
				m_pHeroUI->SettingHeroInfo(pHero->GetHp(), pHero->GetHp(), pHero->GetStress(), HeroName, pHero->GetSkillVector());
			}
		}

		// 영웅 차례이면서 자동전투가 아니면
		if (m_bHero && !m_bAutoBattle)
		{
			// 출혈, 독뎀 반영, 기절이면 기절 줄어들기, 죽으면 죽음상태로
			if (!m_bCalculate)
			{
				dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->StartCalculate();
				m_bCalculate = true;
			}

			// 종료 조건
			if (HeroesAllDead() || MonstersAllDead())
				EndBattle();

			if (dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->GetIsCorpse() ||
				dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->GetIsDeath() ||
				!dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->GetTurn())
			{
				CreatureTurnEnd();
				m_bNext = false;
			}
			else
			{
				// 스킬 입력
				if (!m_bSkillInput)
				{
					m_bSkillInput = SkillKeyInput();
				}
				if (m_bSkillInput)
				{
					m_fKeyInputIntervel -= fTimeDelta;
					if (m_fKeyInputIntervel < 0)
					{
						m_fKeyInputIntervel = KEYINPUTINTERVEL;
						m_bAblePostionInput = true;
					}
				}
				// 스킬 사용 위치 입력
				if (m_bAblePostionInput)
				{
					m_bPositionInput = PositionKeyInput();
				}
			
				if (m_bPositionInput)
				{
					m_bSkillInput = false;
					m_bAblePostionInput = false;
					m_bPositionInput = false;

					Battle(m_iSelectSkill);
					// 크리처 턴 엔드
					CreatureTurnEnd();
					m_bNext = false;
					m_bCalculate = false;
				}
			}
		}
		// 자동전투이면
		else
		{
			// 출혈, 독뎀 반영, 기절이면 기절 줄어들기, 죽으면 죽음상태로
			dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->StartCalculate();

			// 종료 조건
			if (HeroesAllDead() || MonstersAllDead())
				EndBattle();

			if (!dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->GetIsCorpse() &&
				!dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->GetIsDeath() &&
				dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->GetTurn())
			{
				Battle(0);
			}
			// 크리처 턴 엔드
			CreatureTurnEnd();
			m_bNext = false;
		}
	}

	// 종료 조건
	if (HeroesAllDead() || MonstersAllDead())
		EndBattle();

	//UI Update
	m_pHeroUI->UpdateGameObject(fTimeDelta);
}

void CBattleSystem::StartTurn()
{
	// 턴 시작시 나오는 UI등 그런 것들
	if (m_pHeroUI)
	{
		m_pHeroUI->SetVisible(false);
	}
	m_pCurrentCreature = NextCreature();
}

shared_ptr<CGameObject> CBattleSystem::NextCreature()
{
	// 속도를 기준으로 내림차순
	CmpBySpeed(m_vCreatures);
	// 영웅부터 몬스터까지 빠른 속도인 creature를 반환
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
	// 모두 다 했을시에 다음 턴으로
	NextTurn();

	return shared_ptr<CGameObject>();
}

void CBattleSystem::CreatureTurnEnd()
{
	dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->SetTurn(false);
	dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->SetDone(true);
	//m_pCurrentCreature = NextCreature();
	// 죽은애 체크
	m_bDeadCheck = true;
}

void CBattleSystem::NextTurn()
{
	// 모두 초기화
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
	//m_pCurrentCreature = nullptr;

	m_iTurn++;
}

void CBattleSystem::EndBattle()
{
	// 죽은 진영 시체 전부 없애기
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
	// 죽은애 있으면 앞으로 당겨주기
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

void CBattleSystem::FormBattlePosition(vector<shared_ptr<CGameObject>>& _vHeroes, vector<shared_ptr<CGameObject>>& _vMonsters, _float _fAngle1,
	_float _fAngle2, _vec3 _vOrigin)
{
	for (int i = 0; i < size(_vHeroes); i++)
	{
		switch (i)
		{
		case 0:
			dynamic_pointer_cast<CCreature>(_vHeroes[0])->SetPos(_vec3(_vOrigin.x - 6.f, _vOrigin.y, _vOrigin.z));
			dynamic_pointer_cast<CCreature>(_vHeroes[0])->SetAngle(_vec3(0.f, _fAngle1, 0.f));
			m_vHeroLocation.push_back(_vec3(_vOrigin.x - 6.f, _vOrigin.y, _vOrigin.z));
			break;
		case 1:
			dynamic_pointer_cast<CCreature>(_vHeroes[1])->SetPos(_vec3(_vOrigin.x - 8.f, _vOrigin.y, _vOrigin.z - 2.f));
			dynamic_pointer_cast<CCreature>(_vHeroes[1])->SetAngle(_vec3(0.f, _fAngle1, 0.f));
			m_vHeroLocation.push_back(_vec3(_vOrigin.x - 8.f, _vOrigin.y, _vOrigin.z - 2.f));
			break;
		case 2:
			dynamic_pointer_cast<CCreature>(_vHeroes[2])->SetPos(_vec3(_vOrigin.x - 14.f, _vOrigin.y, _vOrigin.z + 4.f));
			dynamic_pointer_cast<CCreature>(_vHeroes[2])->SetAngle(_vec3(0.f, _fAngle1, 0.f));
			m_vHeroLocation.push_back(_vec3(_vOrigin.x - 14.f, _vOrigin.y, _vOrigin.z + 4.f));
			break;
		case 3:
			dynamic_pointer_cast<CCreature>(_vHeroes[3])->SetPos(_vec3(_vOrigin.x - 16.f, _vOrigin.y, _vOrigin.z + 2.f));
			dynamic_pointer_cast<CCreature>(_vHeroes[3])->SetAngle(_vec3(0.f, _fAngle1, 0.f));
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
			dynamic_pointer_cast<CCreature>(_vMonsters[0])->SetAngle(_vec3(0.f, _fAngle2 + PI, 0.f));
			m_vMonsterLocation.push_back(_vec3(_vOrigin.x + 6.f, _vOrigin.y, _vOrigin.z));
			break;
		case 1:
			dynamic_pointer_cast<CCreature>(_vMonsters[1])->SetPos(_vec3(_vOrigin.x + 8.f, _vOrigin.y, _vOrigin.z - 2.f));
			dynamic_pointer_cast<CCreature>(_vMonsters[1])->SetAngle(_vec3(0.f, _fAngle2 + PI, 0.f));
			m_vMonsterLocation.push_back(_vec3(_vOrigin.x + 8.f, _vOrigin.y, _vOrigin.z - 2.f));
		case 2:
			dynamic_pointer_cast<CCreature>(_vMonsters[2])->SetPos(_vec3(_vOrigin.x + 14.f, _vOrigin.y, _vOrigin.z + 4.f));
			dynamic_pointer_cast<CCreature>(_vMonsters[2])->SetAngle(_vec3(0.f, _fAngle2 + PI, 0.f));
			m_vMonsterLocation.push_back(_vec3(_vOrigin.x + 14.f, _vOrigin.y, _vOrigin.z + 4.f));
			break;
		case 3:
			dynamic_pointer_cast<CCreature>(_vMonsters[3])->SetPos(_vec3(_vOrigin.x + 16.f, _vOrigin.y, _vOrigin.z + 2.f));
			dynamic_pointer_cast<CCreature>(_vMonsters[3])->SetAngle(_vec3(0.f, _fAngle2 + PI, 0.f));
			m_vMonsterLocation.push_back(_vec3(_vOrigin.x + 16.f, _vOrigin.y, _vOrigin.z + 2.f));
			break;
		}
	}
}

void CBattleSystem::SwitchPosition(int _iCurrentIndex, int _iMoveCnt, _bool _bHero)
{
	// 영웅의 경우 이동
	if (_bHero)
	{
		// 맨 앞 이동일 경우
		if ((_iCurrentIndex - _iMoveCnt) <= 0)
		{
			dynamic_pointer_cast<CCreature>(m_vHeroes[_iCurrentIndex])->SetMoving(true);
			dynamic_pointer_cast<CCreature>(m_vHeroes[_iCurrentIndex])->SetTargetPos(m_vHeroLocation[0]);
			dynamic_pointer_cast<CCreature>(m_vHeroes[_iCurrentIndex])->SetMovingSpeed(
				dynamic_pointer_cast<CCreature>(m_vHeroes[_iCurrentIndex])->MovingSpeed(m_vHeroLocation[0], SKILLMOVEINTERVEL));

			// 뒤로 한칸씩 이동
			for (int i = 0; i < _iCurrentIndex; i++)
			{
				dynamic_pointer_cast<CCreature>(m_vHeroes[i])->SetMoving(true);
				dynamic_pointer_cast<CCreature>(m_vHeroes[i])->SetTargetPos(m_vHeroLocation[i + 1]);
				dynamic_pointer_cast<CCreature>(m_vHeroes[i])->SetMovingSpeed(
					dynamic_pointer_cast<CCreature>(m_vHeroes[i])->MovingSpeed(m_vHeroLocation[i + 1], SKILLMOVEINTERVEL));
			}
			
			m_bSkillMove = true;
			shared_ptr<CGameObject> pObj1 = m_vHeroes[_iCurrentIndex];
			// swap
			for (int i = _iCurrentIndex - 1; i >= 0; i--)
			{
				shared_ptr<CGameObject> pObj = m_vHeroes[i];
				m_vHeroes[i + 1] = m_vHeroes[i];
				m_vHeroes[i + 1] = pObj;
			}
			m_vHeroes[0] = pObj1;
		}
		// 맨 뒤 이동
		else if ((_iCurrentIndex - _iMoveCnt) >= iLiveHeroes)
		{
			dynamic_pointer_cast<CCreature>(m_vHeroes[_iCurrentIndex])->SetMoving(true);
			dynamic_pointer_cast<CCreature>(m_vHeroes[_iCurrentIndex])->SetTargetPos(m_vHeroLocation[iLiveHeroes - 1]);
			dynamic_pointer_cast<CCreature>(m_vHeroes[_iCurrentIndex])->SetMovingSpeed(
				dynamic_pointer_cast<CCreature>(m_vHeroes[_iCurrentIndex])->MovingSpeed(m_vHeroLocation[iLiveHeroes - 1], SKILLMOVEINTERVEL));
			
			// 앞으로 한칸씩 이동
			for (int i = iLiveHeroes - 1; i > _iCurrentIndex; i--)
			{
				dynamic_pointer_cast<CCreature>(m_vHeroes[i])->SetMoving(true);
				dynamic_pointer_cast<CCreature>(m_vHeroes[i])->SetTargetPos(m_vHeroLocation[i - 1]);
				dynamic_pointer_cast<CCreature>(m_vHeroes[i])->SetMovingSpeed(
					dynamic_pointer_cast<CCreature>(m_vHeroes[i])->MovingSpeed(m_vHeroLocation[i - 1], SKILLMOVEINTERVEL));
			}

			m_bSkillMove = true;
			shared_ptr<CGameObject> pObj1 = m_vHeroes[_iCurrentIndex];
			// swap
			for (int i = _iCurrentIndex; i < iLiveHeroes - 1; i++)
			{
				shared_ptr<CGameObject> pObj = m_vHeroes[i];
				m_vHeroes[i] = m_vHeroes[i + 1];
				m_vHeroes[i + 1] = pObj;
			}
			m_vHeroes[iLiveHeroes - 1] = pObj1;
		}
		// 한 칸 앞으로 이동
		else if (_iMoveCnt == 1)
		{
			dynamic_pointer_cast<CCreature>(m_vHeroes[_iCurrentIndex])->SetMoving(true);
			dynamic_pointer_cast<CCreature>(m_vHeroes[_iCurrentIndex])->SetTargetPos(m_vHeroLocation[_iCurrentIndex - 1]);
			dynamic_pointer_cast<CCreature>(m_vHeroes[_iCurrentIndex])->SetMovingSpeed(
				dynamic_pointer_cast<CCreature>(m_vHeroes[_iCurrentIndex])->MovingSpeed(m_vHeroLocation[_iCurrentIndex - 1], SKILLMOVEINTERVEL));

			// 앞에 있는 애 뒤로 한칸 이동
			dynamic_pointer_cast<CCreature>(m_vHeroes[_iCurrentIndex - 1])->SetMoving(true);
			dynamic_pointer_cast<CCreature>(m_vHeroes[_iCurrentIndex - 1])->SetTargetPos(m_vHeroLocation[_iCurrentIndex]);
			dynamic_pointer_cast<CCreature>(m_vHeroes[_iCurrentIndex - 1])->SetMovingSpeed(
				dynamic_pointer_cast<CCreature>(m_vHeroes[_iCurrentIndex - 1])->MovingSpeed(m_vHeroLocation[_iCurrentIndex], SKILLMOVEINTERVEL));

			m_bSkillMove = true;

			shared_ptr<CGameObject> pObj = m_vHeroes[_iCurrentIndex];
			m_vHeroes[_iCurrentIndex] = m_vHeroes[_iCurrentIndex - 1];
			m_vHeroes[_iCurrentIndex - 1] = pObj;
		}
		// 한 칸 뒤로 이동
		else if (_iMoveCnt == -1)
		{
			dynamic_pointer_cast<CCreature>(m_vHeroes[_iCurrentIndex])->SetMoving(true);
			dynamic_pointer_cast<CCreature>(m_vHeroes[_iCurrentIndex])->SetTargetPos(m_vHeroLocation[_iCurrentIndex + 1]);
			dynamic_pointer_cast<CCreature>(m_vHeroes[_iCurrentIndex])->SetMovingSpeed(
				dynamic_pointer_cast<CCreature>(m_vHeroes[_iCurrentIndex])->MovingSpeed(m_vHeroLocation[_iCurrentIndex + 1], SKILLMOVEINTERVEL));

			// 뒤에 있는 애 앞으로로 한칸 이동
			dynamic_pointer_cast<CCreature>(m_vHeroes[_iCurrentIndex + 1])->SetMoving(true);
			dynamic_pointer_cast<CCreature>(m_vHeroes[_iCurrentIndex + 1])->SetTargetPos(m_vHeroLocation[_iCurrentIndex]);
			dynamic_pointer_cast<CCreature>(m_vHeroes[_iCurrentIndex + 1])->SetMovingSpeed(
				dynamic_pointer_cast<CCreature>(m_vHeroes[_iCurrentIndex + 1])->MovingSpeed(m_vHeroLocation[_iCurrentIndex], SKILLMOVEINTERVEL));

			m_bSkillMove = true;

			shared_ptr<CGameObject> pObj = m_vHeroes[_iCurrentIndex];
			m_vHeroes[_iCurrentIndex] = m_vHeroes[_iCurrentIndex + 1];
			m_vHeroes[_iCurrentIndex + 1] = pObj;
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

_bool CBattleSystem::SkillKeyInput()
{
	if (GetAsyncKeyState('1') & 0x8000) {
		m_iSelectSkill = 1;
		return true;
	}
	if (GetAsyncKeyState('2') & 0x8000) {
		m_iSelectSkill = 2;
		return true;
	}
	if (GetAsyncKeyState('3') & 0x8000) {
		m_iSelectSkill = 3;
		return true;
	}
	if (GetAsyncKeyState('4') & 0x8000) {
		m_iSelectSkill = 4;
		return true;
	}
	if (GetAsyncKeyState('5') & 0x8000) {
		m_iSelectSkill = 5;
		return true;
	}

	m_iSelectSkill = 0;
	return false;
}

_bool CBattleSystem::PositionKeyInput()
{
	if (GetAsyncKeyState('1') & 0x8000) {
		m_iSelectPosition = 1;
		return true;
	}
	if (GetAsyncKeyState('2') & 0x8000) {
		m_iSelectPosition = 2;
		return true;
	}
	if (GetAsyncKeyState('3') & 0x8000) {
		m_iSelectPosition = 3;
		return true;
	}
	if (GetAsyncKeyState('4') & 0x8000) {
		m_iSelectPosition = 4;
		return true;
	}

	m_iSelectPosition = 0;
	return false;
}

void CBattleSystem::AutoBattleKeyInput()
{
	//if (GetAsyncKeyState('7') & 0x8000) {
	//	if (m_bAutoBattle)
	//		m_bAutoBattle = false;
	//	else
	//		m_bAutoBattle = true;
	//}
}

void CBattleSystem::Battle(int _iNum)
{
	int iNum = 0;
	// 위치 변경
	if(_iNum == 5)
	{
		iNum = 5;
	}
	// 스킬 지정
	else if (_iNum >= 1)
	{
		iNum = _iNum - 1;
	}
	// auto battle 일때
	else
		iNum = rand() % dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->GetSkillNum();

	int iTarget = 0;
	int iTargetTeam = 0;

	// 죽은 애들 빼주기
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
	iLiveMonsters = size(m_vMonsters) - iDeathMonsters;
	iLiveHeroes = size(m_vHeroes) - iDeathHeroes;

	// 자동전투가 아닐때
	if (!m_bAutoBattle)
	{
		// 사망하지 않은 적만 때리게하기
		if (m_bHero)
		{
			iTarget = m_iSelectPosition - 1;
			iTargetTeam = m_iSelectPosition - 1;
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
	}
	// 자동전투일때
	else
	{
		// 사망하지 않은 적만 때리게하기
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
			while (1)
			{
				iTargetTeam = rand() % (size(m_vHeroes) - iDeathHeroes);
				if (!dynamic_pointer_cast<CCreature>(m_vHeroes[iTargetTeam])->GetIsDeath() &&
					dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->GetSkill(iNum)->GetTargetPos()[iTargetTeam])
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
	}

	if (m_bHero)
	{
		// 이동할때
		if (iNum == 5)
		{
			m_bSwitching = true;

			dynamic_pointer_cast<CCreature>(m_vHeroes[iCurrentHeroIndex])->SetMoving(true);
			dynamic_pointer_cast<CCreature>(m_vHeroes[iCurrentHeroIndex])->SetTargetPos(m_vHeroLocation[iTarget]);
			dynamic_pointer_cast<CCreature>(m_vHeroes[iCurrentHeroIndex])->SetMovingSpeed(
				dynamic_pointer_cast<CCreature>(m_vHeroes[iCurrentHeroIndex])->MovingSpeed(m_vHeroLocation[iTarget], SWITCHINGINTERVEL));

			dynamic_pointer_cast<CCreature>(m_vHeroes[iTarget])->SetMoving(true);
			dynamic_pointer_cast<CCreature>(m_vHeroes[iTarget])->SetTargetPos(m_vHeroLocation[iCurrentHeroIndex]);
			dynamic_pointer_cast<CCreature>(m_vHeroes[iTarget])->SetMovingSpeed(
				dynamic_pointer_cast<CCreature>(m_vHeroes[iTarget])->MovingSpeed(m_vHeroLocation[iCurrentHeroIndex], SWITCHINGINTERVEL));

			shared_ptr<CGameObject> pObj = m_vHeroes[iCurrentHeroIndex];
			m_vHeroes[iCurrentHeroIndex] = m_vHeroes[iTarget];
			m_vHeroes[iTarget] = pObj;
		}
		else
		{
			// 다가가는 스킬일 경우에 이동하게
			if (dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->GetSkill(iNum)->IsApproach())
			{
				// 다가가는 스킬 이후 이동해야하는 공격일 경우
				if (dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->GetSkill(iNum)->GetMovingCnt() != 0)
				{
					int iMovePos = dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->GetSkill(iNum)->GetMovingCnt();
					// 맨 앞 이동일 경우
					if ((iCurrentHeroIndex - iMovePos) <= 0)
					{
						// 뒤로 한칸씩 이동
						for (int i = 0; i < iCurrentHeroIndex; i++)
						{
							dynamic_pointer_cast<CCreature>(m_vHeroes[i])->SetMoving(true);
							dynamic_pointer_cast<CCreature>(m_vHeroes[i])->SetTargetPos(m_vHeroLocation[i + 1]);
							dynamic_pointer_cast<CCreature>(m_vHeroes[i])->SetMovingSpeed(
								dynamic_pointer_cast<CCreature>(m_vHeroes[i])->MovingSpeed(m_vHeroLocation[i + 1], SKILLMOVEINTERVEL));
						}

						m_bSkillMove = true;
						shared_ptr<CGameObject> pObj1 = m_vHeroes[iCurrentHeroIndex];
						// swap
						for (int i = iCurrentHeroIndex - 1; i >= 0; i--)
						{
							shared_ptr<CGameObject> pObj = m_vHeroes[i];
							m_vHeroes[i + 1] = m_vHeroes[i];
							m_vHeroes[i + 1] = pObj;
						}
						m_vHeroes[0] = pObj1;
						dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->SetTargetPos2(m_vHeroLocation[0]);
					}
					// 맨 뒤 이동
					else if ((iCurrentHeroIndex - iMovePos) >= iLiveHeroes)
					{
						// 앞으로 한칸씩 이동
						for (int i = iLiveHeroes - 1; i > iCurrentHeroIndex; i--)
						{
							dynamic_pointer_cast<CCreature>(m_vHeroes[i])->SetMoving(true);
							dynamic_pointer_cast<CCreature>(m_vHeroes[i])->SetTargetPos(m_vHeroLocation[i - 1]);
							dynamic_pointer_cast<CCreature>(m_vHeroes[i])->SetMovingSpeed(
								dynamic_pointer_cast<CCreature>(m_vHeroes[i])->MovingSpeed(m_vHeroLocation[i - 1], SKILLMOVEINTERVEL));
						}

						m_bSkillMove = true;
						shared_ptr<CGameObject> pObj1 = m_vHeroes[iCurrentHeroIndex];
						// swap
						for (int i = iCurrentHeroIndex; i < iLiveHeroes - 1; i++)
						{
							shared_ptr<CGameObject> pObj = m_vHeroes[i];
							m_vHeroes[i] = m_vHeroes[i + 1];
							m_vHeroes[i + 1] = pObj;
						}
						m_vHeroes[iLiveHeroes - 1] = pObj1;
						dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->SetTargetPos2(m_vHeroLocation[iLiveHeroes - 1]);
					}
					// 한 칸 앞으로 이동
					else if (iMovePos == 1)
					{
						// 앞에 있는 애 뒤로 한칸 이동
						dynamic_pointer_cast<CCreature>(m_vHeroes[iCurrentHeroIndex - 1])->SetMoving(true);
						dynamic_pointer_cast<CCreature>(m_vHeroes[iCurrentHeroIndex - 1])->SetTargetPos(m_vHeroLocation[iCurrentHeroIndex]);
						dynamic_pointer_cast<CCreature>(m_vHeroes[iCurrentHeroIndex - 1])->SetMovingSpeed(
							dynamic_pointer_cast<CCreature>(m_vHeroes[iCurrentHeroIndex - 1])->MovingSpeed(m_vHeroLocation[iCurrentHeroIndex], SKILLMOVEINTERVEL));

						m_bSkillMove = true;

						shared_ptr<CGameObject> pObj = m_vHeroes[iCurrentHeroIndex];
						m_vHeroes[iCurrentHeroIndex] = m_vHeroes[iCurrentHeroIndex - 1];
						m_vHeroes[iCurrentHeroIndex - 1] = pObj;
						dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->SetTargetPos2(m_vHeroLocation[iCurrentHeroIndex - 1]);
					}
					// 한 칸 뒤로 이동
					else if (iMovePos == -1)
					{
						// 뒤에 있는 애 앞으로로 한칸 이동
						dynamic_pointer_cast<CCreature>(m_vHeroes[iCurrentHeroIndex + 1])->SetMoving(true);
						dynamic_pointer_cast<CCreature>(m_vHeroes[iCurrentHeroIndex + 1])->SetTargetPos(m_vHeroLocation[iCurrentHeroIndex]);
						dynamic_pointer_cast<CCreature>(m_vHeroes[iCurrentHeroIndex + 1])->SetMovingSpeed(
							dynamic_pointer_cast<CCreature>(m_vHeroes[iCurrentHeroIndex + 1])->MovingSpeed(m_vHeroLocation[iCurrentHeroIndex], SKILLMOVEINTERVEL));

						m_bSkillMove = true;

						shared_ptr<CGameObject> pObj = m_vHeroes[iCurrentHeroIndex];
						m_vHeroes[iCurrentHeroIndex] = m_vHeroes[iCurrentHeroIndex + 1];
						m_vHeroes[iCurrentHeroIndex + 1] = pObj;
						dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->SetTargetPos2(m_vHeroLocation[iCurrentHeroIndex + 1]);
					}

					dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->SetAttackMoving(true);
					dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->SetTargetPos(m_vMonsterLocation[iTarget] + m_vApproachingGapR);
					dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->SetMovingSpeed(
						dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->MovingSpeed(m_vMonsterLocation[iTarget] + m_vApproachingGapR, ATTACKSKILLMOVINGINTERVEL));
					m_bAttackSkillMoving = true;
				}
				else
				{
					dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->SetAttackMoving(true);
					dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->SetTargetPos(m_vMonsterLocation[iTarget] + m_vApproachingGapR);
					dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->SetTargetPos2(m_pCurrentCreature->GetPos());
					dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->SetMovingSpeed(
						dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->MovingSpeed(m_vMonsterLocation[iTarget] + m_vApproachingGapR, ATTACKSKILLMOVINGINTERVEL));
					m_bAttackSkillMoving = true;
				}
			}

			// 다가가는 스킬이 아니고 이동하는 공격일 경우
			if (dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->GetSkill(iNum)->GetMovingCnt() != 0 &&
				!dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->GetSkill(iNum)->IsApproach())
			{
				int iMovePos = dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->GetSkill(iNum)->GetMovingCnt();

				SwitchPosition(iCurrentHeroIndex, iMovePos, true);
			}

			// 광역기 공격이고 힐이 아닐경우
			if (dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->GetSkill(iNum)->IsTargetAll() &&
				(dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->GetSkill(iNum)->GetHeal() == 0))
			{
				for (int i = 0; i < size(m_vMonsters); i++)
				{
					dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->AttackCreature
					(dynamic_pointer_cast<CCreature>(m_vMonsters[i]), dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->GetSkill(iNum));
					dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->SetAttacking(true, iNum);
				}
			}
			// 힐이고 광역일 경우
			else if ((dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->GetSkill(iNum)->GetHeal() != 0) &&
				dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->GetSkill(iNum)->IsTargetAll())
			{
				for (int i = 0; i < size(m_vHeroes); i++)
				{
					dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->AttackCreature
					(dynamic_pointer_cast<CCreature>(m_vHeroes[i]), dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->GetSkill(iNum));
					dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->SetAttacking(true, iNum);
				}
			}
			// 단일 힐일경우
			else if (dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->GetSkill(iNum)->GetHeal() != 0 &&
				!dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->GetSkill(iNum)->IsTargetAll())
			{
				dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->AttackCreature
				(dynamic_pointer_cast<CCreature>(m_vHeroes[iTargetTeam]), dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->GetSkill(iNum));
				dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->SetAttacking(true, iNum);
			}
			// 이동시키는 공격일 경우
			else if ((dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->GetSkill(iNum)->GetMoveCnt() != 0))
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
							dynamic_pointer_cast<CCreature>(m_vMonsters[iTarget])->MovingSpeed(m_vMonsterLocation[iTarget + iMovePos], SKILLMOVINGINTERVEL));

						dynamic_pointer_cast<CCreature>(m_vMonsters[iTarget + iMovePos])->SetMoving(true);
						dynamic_pointer_cast<CCreature>(m_vMonsters[iTarget + iMovePos])->SetTargetPos(m_vMonsterLocation[iTarget]);
						dynamic_pointer_cast<CCreature>(m_vMonsters[iTarget + iMovePos])->SetMovingSpeed(
							dynamic_pointer_cast<CCreature>(m_vMonsters[iTarget + iMovePos])->MovingSpeed(m_vMonsterLocation[iTarget], SKILLMOVINGINTERVEL));
						m_bSkillMoving = true;
						shared_ptr<CGameObject> pObj = m_vMonsters[iTarget];
						m_vMonsters[iTarget] = m_vMonsters[iTarget + iMovePos];
						m_vMonsters[iTarget + iMovePos] = pObj;
					}
				}
			}
			// 일반 공격일 경우
			else
			{
				dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->AttackCreature
				(dynamic_pointer_cast<CCreature>(m_vMonsters[iTarget]), dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->GetSkill(iNum));
				dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->SetAttacking(true, iNum);
			}
		}
	}
	else
	{
		// 다가가는 스킬일 경우에 이동하게
		if (dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->GetSkill(iNum)->IsApproach())
		{
			dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->SetAttackMoving(true);
			dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->SetTargetPos(m_vHeroLocation[iTarget] + m_vApproachingGapL);
			dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->SetTargetPos2(m_pCurrentCreature->GetPos());
			dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->SetMovingSpeed(
				dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->MovingSpeed(m_vHeroLocation[iTarget] + m_vApproachingGapL, ATTACKSKILLMOVINGINTERVEL));
			m_bAttackSkillMoving = true;
		}

		// 광역기 공격일 경우
		if (dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->GetSkill(iNum)->IsTargetAll())
		{
			for (int i = 0; i < size(m_vHeroes); i++)
			{
				dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->AttackCreature
				(dynamic_pointer_cast<CCreature>(m_vHeroes[i]), dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->GetSkill(iNum));
				dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->SetAttacking(true, iNum);
			}
		}
		// 이동시키는 공격일 경우
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
						dynamic_pointer_cast<CCreature>(m_vHeroes[iTarget])->MovingSpeed(m_vHeroLocation[iTarget + iMovePos], SKILLMOVINGINTERVEL));

					dynamic_pointer_cast<CCreature>(m_vHeroes[iTarget + iMovePos])->SetMoving(true);
					dynamic_pointer_cast<CCreature>(m_vHeroes[iTarget + iMovePos])->SetTargetPos(m_vHeroLocation[iTarget]);
					dynamic_pointer_cast<CCreature>(m_vHeroes[iTarget + iMovePos])->SetMovingSpeed(
						dynamic_pointer_cast<CCreature>(m_vHeroes[iTarget + iMovePos])->MovingSpeed(m_vHeroLocation[iTarget], SKILLMOVINGINTERVEL));
					m_bSkillMoving = true;
					shared_ptr<CGameObject> pObj = m_vHeroes[iTarget];
					m_vHeroes[iTarget] = m_vHeroes[iTarget + iMovePos];
					m_vHeroes[iTarget + iMovePos] = pObj;
				}
			}
		}
		// 일반 공격일 경우
		else
		{
			dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->AttackCreature
			(dynamic_pointer_cast<CCreature>(m_vHeroes[iTarget]), dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->GetSkill(iNum));
			dynamic_pointer_cast<CCreature>(m_pCurrentCreature)->SetAttacking(true, iNum);
		}
	}
}
