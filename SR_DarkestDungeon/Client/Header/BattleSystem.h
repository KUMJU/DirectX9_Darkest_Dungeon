#pragma once
#include"GameObject.h"

class CBattleHeroUI;


class CBattleSystem
{
public:
	CBattleSystem();
	~CBattleSystem();

public:
	vector<shared_ptr<CGameObject>> GetHeroesVector() { return m_vHeroes; }
	vector<shared_ptr<CGameObject>> GetMonstersVector() { return m_vMonsters; }
	void SetAutoBattle(_bool _bAutoBattle) { m_bAutoBattle = _bAutoBattle; }
	void SetCurrentCreature(shared_ptr<CGameObject> _pCrea) { m_pCurrentCreature = _pCrea; }
	void PushCreaturesVector(vector<shared_ptr<CGameObject>>& _vVec)
	{
		m_vCreatures = _vVec;
	}
	void PushHeroesVector(vector<shared_ptr<CGameObject>>& _vVec)
	{
		m_vHeroes = _vVec;
	}
	void PushMonstersVector(vector<shared_ptr<CGameObject>>& _vVec)
	{
		m_vMonsters = _vVec;
	}

public:
	_bool Update(const _float& fTimeDelta); // 턴 진행중

public:
	// 턴 시작시 호출
	void StartTurn();

	// 매 크리처 턴 시작시 호출
	shared_ptr<CGameObject> NextCreature(); // 영웅부터 몬스터까지 빠른 속도인 creature를 반환

	// 매 크리처 턴 종료시 호출
	void CreatureTurnEnd();

	// 턴 종료시 호출
	void NextTurn();	// 다음 턴

	// 배틀 끝
	_bool EndBattle();	// 전투 종료

	_bool HeroesAllDead();
	_bool MonstersAllDead();

	// 죽은 애 있으면 뒤로 보내기
	void	DeadCheck();

	// 전투진형 배치
	void	FormBattlePosition(vector<shared_ptr<CGameObject>>& _vHeroes, vector<shared_ptr<CGameObject>>& _vMonsters, _float _fAngle1,
		_float _fAngle2, _vec3 _vOrigin);

	void GetHeroPanelUI(shared_ptr<CBattleHeroUI> _pHeroUI) { m_pHeroUI = _pHeroUI; }

public:
	void SetCenterPos(_vec3 _vPos) { m_vCenterPos = _vPos; }

protected:
	// 위치 변경
	void SwitchPosition(int _iCurrentIndex, int _iMoveCnt, _bool _bHero = true);
	void CmpBySpeed(vector<shared_ptr<CGameObject>>& _vCreatures);
	_bool SkillKeyInput();		// 스킬 사용 키
	_bool PositionKeyInput();		// 스킬 사용 키
	void AutoBattleKeyInput();	// 자동 전투 키
	void Battle(int _iNum);		// 전투 알고리즘
	void OffTargetCursor();		// 타겟 커서 다 끄기
	void OnTurnUi();			// 턴 ui 다 키기

private:
	vector<shared_ptr<CGameObject>> m_vCreatures;		// 영웅, 몬스터 포함 리스트
	vector<shared_ptr<CGameObject>> m_vHeroes;			// 영웅 리스트		0: 제일 앞
	vector<shared_ptr<CGameObject>> m_vMonsters;		// 몬스터 리스트


	//공격시 다가가는 거리 
	_vec3	m_vApproachingGapR = _vec3{ -2.f, 0.f, -1.f };
	_vec3	m_vApproachingGapL = _vec3{ 2.f, 0.f, -1.f };

	_int m_iTurn = 1;	// 현재 턴
	shared_ptr<CGameObject>			m_pCurrentCreature = nullptr;

	_float  m_fBattleTime = BATTLEINTERVEL;		// 매 크리처 턴사이 간격
	_float	m_fDeathMovingTime = DEATHMOVINGINTERVEL;		// 크리처 죽으면 옮기는 시간
	_float	m_fSkillMovingTime = SKILLMOVINGINTERVEL;		// 스킬로 인한 움직이는 시간
	_float	m_fSkillMoveTime = SKILLMOVEINTERVEL;				// 스킬로 스스로 움직이는 시간
	_float	m_fAttackSkillMovingTime = ATTACKSKILLMOVINGINTERVEL;		// 스킬로 인한 움직이는 시간
	_float	m_fBackMovingTime = MOVINGBACKINTERVEL;		// 스킬 이후 돌아가는 시간
	_float	m_fWhileAttackingTime = WHILEATTACKINTERVEL;	// 멈춰서 스킬 사용 시간
	_float	m_fSwitchingTime = SWITCHINGINTERVEL;	// 자리 교체 시간
	_float	m_fKeyInputIntervel = KEYINPUTINTERVEL;
	
	_bool	m_bNext = false;
	_bool	m_bDeathMoving = false;			// 사망으로 인한 이동
	_bool	m_bSkillMoving = false;			// 기술로 인한 이동
	_bool	m_bSkillMove = false;			// 기술로 인한 스스로의 이동
	_bool	m_bAttackSkillMoving = false;	// 공격 스킬 시전중에 다가가는 이동
	_bool	m_bBackMoving = false;			// 공격 스킬 시전 이후에 제자리로 돌아가는 이동
	_bool	m_bSwitching = false;			// 자료 교체 이후에 서로 교체되는 이동
	_bool	m_bWhileAttack = false;			// 공격 스킬을 사용하는 중
	_bool	m_bHero = false;				// 공격 주체가 영웅인지?
	_bool	m_bCounting = false;
	_bool	m_bDeadCheck = false;

	_bool	m_bAutoBattle = false;
	_bool	m_bSkillInput = false;
	_bool	m_bPositionInput = false;
	_bool	m_bAblePickingInput = false;
	_int	m_iSelectSkill = 0;
	_int	m_iSelectPosition = 0;
	_bool	m_bCalculate = false;
	_bool	m_bUISetDone = false;

	vector<_vec3> m_vHeroLocation = {};			// hero position
	vector<_vec3> m_vMonsterLocation = {};		// monster position
	_int iCurrentHeroIndex = 0;
	_int iCurrentMonsterIndex = 0;

	_vec3		  m_vCenterPos = { 0.f, 0.f, 0.f };

	//Camera Effect Debounce State
	_bool	m_bCamEffectCheck = false;
	_bool	m_bRotationCheck = false;
	_float	m_fAngle = 0.f;
	_float m_fActTime = 0.f;
	// 피킹오브젝트
	vector<shared_ptr<CGameObject>>		m_vecPickingObject = {};

//UI Test
private:
	shared_ptr<CBattleHeroUI> m_pHeroUI = nullptr;
	


	_int iLiveHeroes = 0;
	_int iLiveMonsters = 0;
};

