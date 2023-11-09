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
	_bool Update(const _float& fTimeDelta); // �� ������

public:
	// �� ���۽� ȣ��
	void StartTurn();

	// �� ũ��ó �� ���۽� ȣ��
	shared_ptr<CGameObject> NextCreature(); // �������� ���ͱ��� ���� �ӵ��� creature�� ��ȯ

	// �� ũ��ó �� ����� ȣ��
	void CreatureTurnEnd();

	// �� ����� ȣ��
	void NextTurn();	// ���� ��

	// ��Ʋ ��
	_bool EndBattle();	// ���� ����

	_bool HeroesAllDead();
	_bool MonstersAllDead();

	// ���� �� ������ �ڷ� ������
	void	DeadCheck();

	// �������� ��ġ
	void	FormBattlePosition(vector<shared_ptr<CGameObject>>& _vHeroes, vector<shared_ptr<CGameObject>>& _vMonsters, _float _fAngle1,
		_float _fAngle2, _vec3 _vOrigin);

	void GetHeroPanelUI(shared_ptr<CBattleHeroUI> _pHeroUI) { m_pHeroUI = _pHeroUI; }

public:
	void SetCenterPos(_vec3 _vPos) { m_vCenterPos = _vPos; }

protected:
	// ��ġ ����
	void SwitchPosition(int _iCurrentIndex, int _iMoveCnt, _bool _bHero = true);
	void CmpBySpeed(vector<shared_ptr<CGameObject>>& _vCreatures);
	_bool SkillKeyInput();		// ��ų ��� Ű
	_bool PositionKeyInput();		// ��ų ��� Ű
	void AutoBattleKeyInput();	// �ڵ� ���� Ű
	void Battle(int _iNum);		// ���� �˰���
	void OffTargetCursor();		// Ÿ�� Ŀ�� �� ����
	void OnTurnUi();			// �� ui �� Ű��

private:
	vector<shared_ptr<CGameObject>> m_vCreatures;		// ����, ���� ���� ����Ʈ
	vector<shared_ptr<CGameObject>> m_vHeroes;			// ���� ����Ʈ		0: ���� ��
	vector<shared_ptr<CGameObject>> m_vMonsters;		// ���� ����Ʈ


	//���ݽ� �ٰ����� �Ÿ� 
	_vec3	m_vApproachingGapR = _vec3{ -2.f, 0.f, -1.f };
	_vec3	m_vApproachingGapL = _vec3{ 2.f, 0.f, -1.f };

	_int m_iTurn = 1;	// ���� ��
	shared_ptr<CGameObject>			m_pCurrentCreature = nullptr;

	_float  m_fBattleTime = BATTLEINTERVEL;		// �� ũ��ó �ϻ��� ����
	_float	m_fDeathMovingTime = DEATHMOVINGINTERVEL;		// ũ��ó ������ �ű�� �ð�
	_float	m_fSkillMovingTime = SKILLMOVINGINTERVEL;		// ��ų�� ���� �����̴� �ð�
	_float	m_fSkillMoveTime = SKILLMOVEINTERVEL;				// ��ų�� ������ �����̴� �ð�
	_float	m_fAttackSkillMovingTime = ATTACKSKILLMOVINGINTERVEL;		// ��ų�� ���� �����̴� �ð�
	_float	m_fBackMovingTime = MOVINGBACKINTERVEL;		// ��ų ���� ���ư��� �ð�
	_float	m_fWhileAttackingTime = WHILEATTACKINTERVEL;	// ���缭 ��ų ��� �ð�
	_float	m_fSwitchingTime = SWITCHINGINTERVEL;	// �ڸ� ��ü �ð�
	_float	m_fKeyInputIntervel = KEYINPUTINTERVEL;
	
	_bool	m_bNext = false;
	_bool	m_bDeathMoving = false;			// ������� ���� �̵�
	_bool	m_bSkillMoving = false;			// ����� ���� �̵�
	_bool	m_bSkillMove = false;			// ����� ���� �������� �̵�
	_bool	m_bAttackSkillMoving = false;	// ���� ��ų �����߿� �ٰ����� �̵�
	_bool	m_bBackMoving = false;			// ���� ��ų ���� ���Ŀ� ���ڸ��� ���ư��� �̵�
	_bool	m_bSwitching = false;			// �ڷ� ��ü ���Ŀ� ���� ��ü�Ǵ� �̵�
	_bool	m_bWhileAttack = false;			// ���� ��ų�� ����ϴ� ��
	_bool	m_bHero = false;				// ���� ��ü�� ��������?
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
	// ��ŷ������Ʈ
	vector<shared_ptr<CGameObject>>		m_vecPickingObject = {};

//UI Test
private:
	shared_ptr<CBattleHeroUI> m_pHeroUI = nullptr;
	


	_int iLiveHeroes = 0;
	_int iLiveMonsters = 0;
};

